package main

import (
	"context"
	"database/sql"
	"fmt"
	"github.com/google/uuid"
	"github.com/lib/pq"
	"log"
	"os"
	"os/signal"
	"time"

	sq "github.com/Masterminds/squirrel"
	_ "github.com/lib/pq"
)

func GetConnect(uname, pwd string) (*sql.DB, error) {
	db, err := sql.Open("postgres", fmt.Sprintf(
		"host=db.loc port=5432 user=%s password=%s dbname=challenge sslmode=disable",
		uname, pwd))

	if err != nil {
		return nil, err
	}

	err = db.Ping()

	if err != nil {
		return nil, err
	}

	return db, nil
}

func deleteStaleUsers(conn *sql.DB, where interface{}) error {
	rows, err := sq.Select("id").
		From("insertion_memo").
		Where(where).
		RunWith(conn).
		PlaceholderFormat(sq.Dollar).
		Query()

	if err != nil {
		fmt.Printf("error on select: %v", err)
		return err
	}

	staleUsers := make([]uuid.UUID, 0)
	for rows.Next() {
		u := uuid.UUID{}

		if err := rows.Scan(&u); err != nil {
			fmt.Printf("error on select rows scan: %v", err)
			return err
		}

		staleUsers = append(staleUsers, u)
	}

	sqlQuery, args, _ := sq.Select("array_agg(replies) filter(where replies <> '{}' and replies <> NULL)").
		From("messages").
		Where(sq.Expr("author = ANY(?)", pq.Array(staleUsers))).
		ToSql()

	_, err = sq.Delete("messages").
		Where(sq.Expr(fmt.Sprintf("id = ANY(ARRAY[]::uuid[] || (%s))", sqlQuery), args...)).
		RunWith(conn).
		PlaceholderFormat(sq.Dollar).
		Exec()

	_, err = sq.Delete("users").
		Where(sq.Expr("id = ANY(?)", pq.Array(staleUsers))).
		RunWith(conn).
		PlaceholderFormat(sq.Dollar).
		Exec()

	return err
}

func main() {
	suConn, err := GetConnect("challenge", "challenge")
	if err != nil {
		log.Printf("error connecting to database: %v", err)
		time.Sleep(5 * time.Second)

		suConn, err = GetConnect("challenge", "challenge")

		if err != nil {
			log.Fatal(err)
		}
	}

	ctx, cancel := context.WithCancel(context.Background())

	c := make(chan os.Signal, 1)
	signal.Notify(c, os.Interrupt)
	go func() {
		for sig := range c {
			fmt.Printf("signal recieved %v\n", sig)
			cancel()
		}
	}()

	for counter := 0; ; counter++ {
		select {
		case <-ctx.Done():
			return
		default:
		}

		_ = deleteStaleUsers(suConn, sq.And{
			sq.Expr("created_at < now() - interval '2 minutes'"),
			sq.Gt{
				"pp_length": 1337,
			},
		})

		if counter%30 == 0 {
			sqlQuery, args, _ := sq.Select("*").
				From("admins").
				ToSql()

			_ = deleteStaleUsers(suConn, sq.And{
				sq.Expr("created_at < now() - interval '30 minutes'"),
				sq.Expr(fmt.Sprintf("id <> ANY(%s)", sqlQuery), args...),
			})
		}

		time.Sleep(time.Minute)
	}
}
