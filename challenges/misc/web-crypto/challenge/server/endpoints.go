package server

import (
	"chall/collection"
	"fmt"
	"net/http"

	"github.com/gin-gonic/gin"
	"golang.org/x/crypto/bcrypt"
)

func (s Server) Index(c *gin.Context) {
	c.HTML(http.StatusOK, "index.html", nil)
}

func (s Server) Collection(c *gin.Context) {
	secret := c.PostForm("secret")

	debug, _ := c.Cookie("DEBUG")
	err := bcrypt.CompareHashAndPassword(s.debugCookie, []byte(fmt.Sprintf("DEBUG_%s_DEBUG", debug)))

	isDev := err == nil

	if v, ok := collection.Secrets[secret]; !ok {
		c.String(http.StatusBadRequest, "you ok?")
	} else if !isDev {
		c.JSON(http.StatusOK, gin.H{
			"data": s.enc.EncryptPrimeKey([]byte(v)),
		})
	} else {
		// Need to test this before enabling in production
		fmt.Printf("[DEBUG]: Encrypting %s using byte key\n", v)
		c.JSON(http.StatusOK, gin.H{
			"data": s.enc.EncryptByteKey([]byte(v)),
		})
	}
}