package server

import (
	"net/http"

	"github.com/gin-gonic/gin"
	"github.com/gorilla/sessions"
)

func (s *Server) AuthMiddleware(c *gin.Context) {
	session, err := s.store.Get(c.Request, "admin")

	if err != nil {
		if !c.GetBool("disableRedirect") {
			c.Redirect(http.StatusTemporaryRedirect, "/login")
			c.Writer.WriteHeaderNow()
		}
		c.Abort()
		return
	}

	if v, ok := session.Values["authenticated"].(bool); !ok || !v {
		if !c.GetBool("disableRedirect") {
			c.Redirect(http.StatusTemporaryRedirect, "/login")
			c.Writer.WriteHeaderNow()
		}
		c.Abort()
		return
	}

	c.Set("session", session)
	c.Set("user", session.Values["user"])
	c.Next()
}

func (s *Server) AdminMiddleware(c *gin.Context) {
	v, ok := c.Get("session")
	if !ok {
		c.String(http.StatusPaymentRequired, "sorry, this is private territory over here")
		c.Writer.WriteHeaderNow()
		c.Abort()
		return
	}

	session, ok := v.(*sessions.Session)
	if !ok || session == nil {
		c.String(http.StatusPaymentRequired, "sorry, this is private territory over here")
		c.Writer.WriteHeaderNow()
		c.Abort()
		return
	}

	if v, ok := session.Values["admin"].(bool); !ok || !v {
		c.String(http.StatusPaymentRequired, "sorry, this is private territory over here")
		c.Writer.WriteHeaderNow()
		c.Abort()
		return
	}

	c.Next()
}
