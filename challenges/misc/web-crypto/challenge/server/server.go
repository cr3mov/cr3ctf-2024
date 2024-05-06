package server

import (
	"chall/encryption"

	"github.com/gin-gonic/gin"
)

type Server struct {
	enc *encryption.Encryption
	Router *gin.Engine
	listenOn string
	debugCookie []byte
}

func NewServer(listenOn string, debugCookie []byte) *Server {
	return &Server{
		Router: gin.Default(),
		listenOn: listenOn,
		enc: encryption.NewEncryption(),
		debugCookie: debugCookie,
	}
}

func (s *Server) Start() error {
	return s.Router.Run(s.listenOn)
}