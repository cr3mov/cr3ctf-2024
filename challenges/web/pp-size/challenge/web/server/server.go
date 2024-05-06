package server

import (
	"chall/models"
	"chall/repositories"
	"crypto/sha256"
	"database/sql"
	"encoding/hex"
	"fmt"
	"html/template"
	"net"
	"os"
	"time"

	"github.com/gin-contrib/static"
	"github.com/gin-gonic/gin"
	"github.com/gorilla/securecookie"
	"github.com/gorilla/sessions"
)

type Server struct {
	Router         *gin.Engine
	listenOn       string
	UseCaptcha     bool
	captchaKey     string
	captchaSiteKey string

	janitorIPs []string

	messages *repositories.MessagesRepo
	users    *repositories.UsersRepo
	store    *sessions.CookieStore
	admin    *models.User
}

func NewServer(listenOn, staticDir string, conn, suConn *sql.DB) *Server {
	s := &Server{
		Router:   gin.Default(),
		listenOn: listenOn,
		users:    repositories.NewUsers(conn, suConn),
		messages: repositories.NewMessagesRepo(suConn),
		store: sessions.NewCookieStore(securecookie.GenerateRandomKey(64),
			securecookie.GenerateRandomKey(32)),
		captchaKey:     os.Getenv("CAPTCHA_KEY"),
		captchaSiteKey: os.Getenv("CAPTCHA_SITE_KEY"),
	}

	ips, _ := net.LookupHost("janitor.loc")
	s.janitorIPs = ips

	s.store.Options.HttpOnly = true

	s.UseCaptcha = s.captchaKey != ""

	s.Router.Use(static.Serve("/", static.LocalFile(staticDir, false)))
	s.Router.FuncMap["render"] = func(s string) template.HTML { return template.HTML(s) }
	s.Router.FuncMap["length"] = func(s []models.Message) template.HTML { return template.HTML(fmt.Sprintf("%d", len(s))) }
	s.Router.FuncMap["time"] = func(s time.Time) template.HTML {
		d := time.Since(s)
		return template.HTML(fmt.Sprintf("%d minutes ago", int(d.Minutes())))
	}

	password := hex.EncodeToString(securecookie.GenerateRandomKey(32))
	digest := sha256.New()
	digest.Write([]byte(password))

	a, err := s.users.CreateAdmin(&models.User{
		Username: hex.EncodeToString(securecookie.GenerateRandomKey(32)),
		Password: digest.Sum(nil),
	})

	if err != nil {
		panic(err)
	}

	a.Password = []byte(password)
	s.admin = a

	return s
}

func (s *Server) Start() error {
	return s.Router.Run(s.listenOn)
}
