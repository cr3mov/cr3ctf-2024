package main

import (
	"chall/server"
	"database/sql"
	"fmt"
	"log"
	"time"

	_ "github.com/lib/pq"
)

func GetConnect(uname, pwd string) (*sql.DB, error) {
	db, err := sql.Open("postgres", fmt.Sprintf(
		"host=db.loc port=5432 user=%s password=%s dbname=challenge sslmode=disable",
		uname, pwd,
	))

	if err != nil {
		return nil, err
	}

	err = db.Ping()

	if err != nil {
		return nil, err
	}

	return db, nil
}

func main() {
	conn, err := GetConnect("dummy", "dummy")
	if err != nil {
		log.Printf("error connecting to database: %v", err)
		time.Sleep(5 * time.Second)

		conn, err = GetConnect("dummy", "dummy")

		if err != nil {
			log.Fatal(err)
		}
	}

	suConn, err := GetConnect("challenge", "challenge")
	if err != nil {
		log.Printf("error connecting to database: %v", err)
		time.Sleep(5 * time.Second)

		suConn, err = GetConnect("challenge", "challenge")

		if err != nil {
			log.Fatal(err)
		}
	}

	web := server.NewServer(":1337", "./static", conn, suConn)

	web.Router.LoadHTMLGlob("templates/*")

	web.Router.GET("/", web.Root)

	publicGroup := web.Router.Group("/")
	publicGroup.GET("/login", web.Login)
	publicGroup.GET("/register", web.Register)

	publicGroup.POST("/login", web.LoginPost)
	publicGroup.POST("/register", web.RegisterPost)

	authorizedGroup := web.Router.Group("/", web.AuthMiddleware)

	// crutch
	authorizedGroup.POST("/dashboard", web.Dashboard)
	authorizedGroup.GET("/dashboard", web.Dashboard)
	authorizedGroup.GET("/logout", web.Logout)
	authorizedGroup.POST("/comment/:comment_id", web.Comment)
	authorizedGroup.GET("/comment/:comment_id", web.Comment)

	authorizedGroup.POST("/comment", web.PostComment)

	adminGroup := authorizedGroup.Group("/", web.AdminMiddleware)

	adminGroup.GET("/search", web.Search)

	adminGroup.POST("/reply/:comment_id", web.Reply)
	adminGroup.POST("/search", web.PerformSearch)

	if err := web.Start(); err != nil {
		log.Fatal(err)
	}
}
