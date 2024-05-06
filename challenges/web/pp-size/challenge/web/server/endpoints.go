package server

import (
	"chall/bot"
	"chall/models"
	"encoding/base64"
	"encoding/hex"
	"fmt"
	"github.com/lib/pq"
	"net/http"
	"os"
	"strings"

	"github.com/google/uuid"
	"github.com/gorilla/sessions"

	"github.com/gin-gonic/gin"
	"github.com/meyskens/go-hcaptcha"
)

func (s *Server) Root(c *gin.Context) {
	c.Set("disableRedirect", true)
	s.AuthMiddleware(c)

	if c.IsAborted() {
		s.Index(c)
		return
	}

	s.Dashboard(c)
}

func (s *Server) VerifyCaptcha(c *gin.Context) bool {
	isJanitor := false

	if s.janitorIPs != nil {
		for _, v := range s.janitorIPs {
			if strings.HasPrefix(c.RemoteIP(),
				strings.Join(strings.Split(v, ".")[:3], ".")) {
				isJanitor = true
				break
			}
		}
	}

	if s.UseCaptcha && !isJanitor {
		challenge, ok := c.GetPostForm("sql")

		if !ok || challenge == "" {
			c.String(http.StatusPaymentRequired, "sql is empty")
			return false
		}

		hc := hcaptcha.New(s.captchaKey)
		r, err := hc.Verify(challenge, c.RemoteIP())

		if err != nil || !r.Success {
			c.String(http.StatusInternalServerError, "damn, this captcha sucks")
			return false
		}
	}

	return true
}

func (s *Server) Index(c *gin.Context) {
	c.HTML(http.StatusOK, "index.html", nil)
}

func (s *Server) Login(c *gin.Context) {
	c.HTML(http.StatusOK, "login.html", nil)
}

func (s *Server) LoginPost(c *gin.Context) {
	username := c.PostForm("username")
	password := c.PostForm("password")

	if username == "" || password == "" {
		c.String(http.StatusPaymentRequired, "username or password is empty")
		return
	}

	data, err := hex.DecodeString(password)

	if err != nil {
		c.String(http.StatusPaymentRequired, "no way, are you a hacker???")
		return
	}

	user, err := s.users.GetOne(&models.User{
		Username: username,
		Password: data,
	})

	if err != nil {
		c.String(http.StatusPaymentRequired, "invalid credentials")
		return
	}

	user.SetIsAdmin(s.admin)

	session, _ := s.store.Get(c.Request, "admin")
	session.Values["authenticated"] = true
	session.Values["user"] = user.ID.String()
	session.Values["admin"] = user.IsAdmin
	session.Save(c.Request, c.Writer)

	c.Redirect(http.StatusTemporaryRedirect, "/dashboard")
}

func (s *Server) Register(c *gin.Context) {
	c.HTML(http.StatusOK, "register.html", gin.H{
		"use_captcha": s.UseCaptcha,
		"captcha":     s.captchaSiteKey,
	})
}

func (s *Server) RegisterPost(c *gin.Context) {
	username := c.PostForm("username")
	password := c.PostForm("password")

	if username == "" || password == "" {
		c.String(http.StatusPaymentRequired, "username or password is empty")
		return
	}

	if len(username) < 6 || len(password) < 6 || len(username) > 100 || len(
		password) > 100 {
		c.String(http.StatusPaymentRequired,
			"username or password lengths are stupid like you")
		return
	}

	data, err := hex.DecodeString(password)

	if err != nil {
		c.String(http.StatusPaymentRequired, "no way, are you a hacker???")
		return
	}

	user, err := s.users.CreateOne(&models.User{
		Username: username,
		Password: data,
	})

	if err != nil {
		fmt.Printf("just for my own sake %v\n", err)
		c.String(http.StatusPaymentRequired, "no way, "+
			"some bozo took this username already")
		return
	}

	user.SetIsAdmin(s.admin)

	if !s.VerifyCaptcha(c) {
		return
	}

	session, _ := s.store.Get(c.Request, "admin")
	session.Values["authenticated"] = true
	session.Values["user"] = user.ID.String()
	session.Values["admin"] = user.IsAdmin
	_ = session.Save(c.Request, c.Writer)

	c.Redirect(http.StatusTemporaryRedirect, "/dashboard")
}

func (s *Server) Logout(c *gin.Context) {
	_session, ok := c.Get("session")

	if !ok {
		c.Redirect(http.StatusTemporaryRedirect, "/login")
		return
	}

	session := _session.(*sessions.Session)
	session.Values["authenticated"] = false
	session.Values["user"] = ""
	session.Values["admin"] = false
	_ = session.Save(c.Request, c.Writer)

	c.Redirect(http.StatusTemporaryRedirect, "/login")
}

func (s *Server) Dashboard(c *gin.Context) {
	userId, ok := c.Get("user")

	if !ok {
		c.Redirect(http.StatusTemporaryRedirect, "/login")
		return
	}

	uId, err := uuid.Parse(userId.(string))

	if err != nil {
		c.Redirect(http.StatusTemporaryRedirect, "/login")
		return
	}

	user, err := s.users.GetOneById(uId)

	if err != nil {
		c.Redirect(http.StatusTemporaryRedirect, "/login")
		return
	}

	comments, err := s.messages.GetManyForUser(user.ID)

	if err != nil {
		c.String(http.StatusInternalServerError, err.Error())
		return
	}

	out := gin.H{}

	user.SetIsAdmin(s.admin)

	if float32(user.PPLength) >= 1337.2 {
		flag := os.Getenv("FLAG")

		if flag == "" {
			flag = "cr3{REDACTED}"
		}

		out["flag"] = flag

		if strings.HasPrefix(strings.ToLower(c.Request.Method), "get") {
			s.users.Cleanup(user)
		}
	}

	out["user"] = user
	out["messages"] = comments
	out["use_captcha"] = s.UseCaptcha
	out["captcha"] = s.captchaSiteKey

	c.HTML(http.StatusOK, "dashboard.html", out)
}

func (s *Server) Comment(c *gin.Context) {
	commentId := c.Param("comment_id")

	if commentId == "" {
		c.String(http.StatusInternalServerError, "no way, are you a hacker???")
		return
	}

	userId, ok := c.Get("user")

	if !ok {
		c.Redirect(http.StatusTemporaryRedirect, "/login")
		return
	}

	uId, err := uuid.Parse(userId.(string))

	if err != nil {
		c.Redirect(http.StatusTemporaryRedirect, "/login")
		return
	}

	user, err := s.users.GetOneById(uId)

	if err != nil {
		c.Redirect(http.StatusTemporaryRedirect, "/login")
		return
	}

	user.SetIsAdmin(s.admin)

	comment, err := s.messages.FindOne(commentId)

	if err != nil || (user.ID != comment.Author && !user.IsAdmin) {
		fmt.Printf("%v %v %v\n", err, user, comment)
		c.String(http.StatusInternalServerError, "no way, are you a hacker???")
		return
	}

	c.HTML(http.StatusOK, "comment.html", gin.H{
		"comment": comment,
		"user":    user,
	})
}

func (s *Server) PostComment(c *gin.Context) {
	userId, ok := c.Get("user")

	if !ok {
		c.Redirect(http.StatusTemporaryRedirect, "/login")
		return
	}

	uId, err := uuid.Parse(userId.(string))

	if err != nil {
		c.Redirect(http.StatusTemporaryRedirect, "/login")
		return
	}

	user, err := s.users.GetOneById(uId)

	if err != nil {
		c.Redirect(http.StatusTemporaryRedirect, "/login")
		return
	}

	user.SetIsAdmin(s.admin)

	if !s.VerifyCaptcha(c) {
		return
	}

	message, ok := c.GetPostForm("message")

	if !ok || message == "" {
		c.String(http.StatusPaymentRequired, "message is empty")
		return
	}

	data, err := base64.StdEncoding.DecodeString(message)

	if err != nil {
		c.String(http.StatusInternalServerError, "no way, are you a hacker???")
		return
	}

	contents := string(data)

	if len(contents) > 740 {
		c.String(http.StatusInternalServerError, "too long didn't read")
		return
	}

	title := strings.Split(contents, "\n")[0]

	if len(title) > 64 {
		title = title[:64]
	}

	comment, err := s.messages.CreateOneComment(&models.Message{
		Title:  title,
		Body:   contents,
		Author: user.ID,
	})

	if err != nil {
		c.String(http.StatusInternalServerError, err.Error())
		return
	}

	go bot.Visit(s.admin.Username, string(s.admin.Password),
		fmt.Sprintf("http://localhost%s/comment/%s", s.listenOn,
			comment.URL))

	c.Redirect(http.StatusTemporaryRedirect, fmt.Sprintf("/comment/%s", comment.URL))
}

func (s *Server) Reply(c *gin.Context) {
	replyId := c.Param("comment_id")

	if replyId == "" {
		c.String(http.StatusInternalServerError, "no way, are you a hacker???")
		return
	}

	userId, ok := c.Get("user")

	if !ok || userId == "" {
		c.Redirect(http.StatusTemporaryRedirect, "/login")
		return
	}

	uId, err := uuid.Parse(userId.(string))

	if err != nil {
		c.Redirect(http.StatusTemporaryRedirect, "/login")
		return
	}

	user, err := s.users.GetOneById(uId)

	if err != nil {
		c.Redirect(http.StatusTemporaryRedirect, "/login")
		return
	}

	user.SetIsAdmin(s.admin)

	if !user.IsAdmin {
		c.String(http.StatusPaymentRequired, "sorry, "+
			"only for admins this time")
		return
	}

	comment, err := s.messages.FindOne(replyId)

	if err != nil {
		c.String(http.StatusInternalServerError, "no way, are you a hacker???")
		return
	}

	message, ok := c.GetPostForm("message")

	if !ok || message == "" {
		c.String(http.StatusPaymentRequired, "message is empty")
		return
	}

	data, err := base64.StdEncoding.DecodeString(message)

	if err != nil {
		c.String(http.StatusInternalServerError, "no way, are you a hacker???")
		return
	}

	contents := string(data)

	if len(contents) > 512 {
		c.String(http.StatusInternalServerError, "too long didn't read")
		return
	}

	err = s.messages.CreateReplyTo(comment.ID, &models.Message{
		Author: user.ID,
		Title:  "",
		URL:    "",
		Body:   contents,
	})

	if err != nil {
		c.String(http.StatusInternalServerError, err.Error())
		return
	}

	c.String(http.StatusOK, "done")
}

func (s *Server) Search(c *gin.Context) {
	userId, ok := c.Get("user")

	if !ok {
		c.Redirect(http.StatusTemporaryRedirect, "/login")
		return
	}

	uId, err := uuid.Parse(userId.(string))

	if err != nil {
		c.Redirect(http.StatusTemporaryRedirect, "/login")
		return
	}

	user, err := s.users.GetOneById(uId)

	if err != nil {
		c.Redirect(http.StatusTemporaryRedirect, "/login")
		return
	}

	user.SetIsAdmin(s.admin)

	if !user.IsAdmin {
		c.String(http.StatusInternalServerError, "no way, are you a hacker???")
		return
	}

	c.HTML(http.StatusOK, "search.html", gin.H{
		"user": user,
	})
}

func (s *Server) PerformSearch(c *gin.Context) {

	query := c.PostForm("query")

	if query == "" {
		c.String(http.StatusPaymentRequired, "query is empty")
		return
	}

	userId, ok := c.Get("user")

	if !ok {
		c.Redirect(http.StatusTemporaryRedirect, "/login")
		return
	}

	uId, err := uuid.Parse(userId.(string))

	if err != nil {
		c.Redirect(http.StatusTemporaryRedirect, "/login")
		return
	}

	user, err := s.users.GetOneById(uId)

	if err != nil {
		c.Redirect(http.StatusTemporaryRedirect, "/login")
		return
	}

	user.SetIsAdmin(s.admin)

	if !user.IsAdmin {
		c.String(http.StatusInternalServerError, "no way, are you a hacker???")
		return
	}

	users, err := s.users.FindMany(query)

	if err != nil {
		if err, ok := err.(*pq.Error); ok {
			c.String(http.StatusInternalServerError, "%s: (%s) %s -- %v", err.Error(), err.SQLState(), err.Code.Name(), err)
		} else {
			c.String(http.StatusInternalServerError, err.Error())
		}
		return
	}

	c.HTML(http.StatusOK, "search.html", gin.H{
		"user":  user,
		"users": users,
	})
}
