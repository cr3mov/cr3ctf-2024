package main

import (
	"chall/server"
	"log"
)

func main() {
	web := server.NewServer(":1337", []byte("$2a$04$5FGCCcPg1sELCWwnSqWYZemUYOqNIpjwo0l2KAw.1jmP0jLTEqcg."))
	web.Router.LoadHTMLGlob("templates/*")
	public := web.Router.Group("/")

	public.GET("/", web.Index)
	public.POST("/collection", web.Collection)

	if err := web.Start(); err != nil {
		log.Fatal(err)
	}
}