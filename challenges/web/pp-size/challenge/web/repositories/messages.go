package repositories

import (
	"chall/models"
	"database/sql"
	"encoding/hex"
	"fmt"

	"github.com/google/uuid"
	"github.com/gorilla/securecookie"
	"github.com/lib/pq"

	sq "github.com/Masterminds/squirrel"
)

type MessagesRepo struct {
	conn *sql.DB
}

func NewMessagesRepo(conn *sql.DB) *MessagesRepo {
	return &MessagesRepo{conn: conn}
}

func (m *MessagesRepo) GetManyForUser(id uuid.UUID) ([]*models.Message, error) {
	sqlQuery, _, _ := sq.Select("(id, author, created_at, title, body)").
		From("messages").
		Where(sq.Expr("id = ANY(m.replies)")).
		ToSql()

	rows, err := sq.Select("id", "author", "created_at", "title", "body", "url",
		fmt.Sprintf("ARRAY(%s) as _replies", sqlQuery)).
		From("messages m").
		Where(sq.And{
			sq.Eq{
				"author": id,
			},
			sq.NotEq{
				"url": "",
			},
		}).
		OrderBy("created_at DESC").
		RunWith(m.conn).
		PlaceholderFormat(sq.Dollar).
		Query()

	if err != nil {
		return nil, err
	}

	messages := make([]*models.Message, 0)
	for rows.Next() {
		msg := &models.Message{}

		if err := rows.Scan(&msg.ID, &msg.Author, &msg.CreatedAt,
			&msg.Title, &msg.Body, &msg.URL, pq.Array(&msg.RepliesMessages)); err != nil {
			return nil, err
		}

		messages = append(messages, msg)
	}

	return messages, nil
}

func (m *MessagesRepo) FindOne(url string) (*models.Message, error) {
	sqlQuery, _, _ := sq.Select("(id, author, created_at, title, body)").
		From("messages").
		Where(sq.Expr("id = ANY(m.replies)")).
		ToSql()

	row := sq.Select("id", "author", "created_at", "title", "body", "url",
		fmt.Sprintf("ARRAY(%s) as _replies", sqlQuery)).
		From("messages m").
		Where(sq.Eq{
			"url": url,
		}).
		OrderBy("created_at DESC").
		Limit(1).
		RunWith(m.conn).
		PlaceholderFormat(sq.Dollar).
		QueryRow()

	msg := &models.Message{}
	if err := row.Scan(&msg.ID, &msg.Author, &msg.CreatedAt,
		&msg.Title, &msg.Body, &msg.URL, pq.Array(&msg.RepliesMessages)); err != nil {
		return nil, err
	}

	return msg, nil
}

func (m *MessagesRepo) CreateOneComment(message *models.Message) (*models.
	Message,
	error) {
	row := sq.Insert("messages").
		Columns("author", "title", "body", "url").
		Values(message.Author, message.Title, message.Body, hex.EncodeToString(securecookie.GenerateRandomKey(32))).
		Suffix(" RETURNING *").
		RunWith(m.conn).
		PlaceholderFormat(sq.Dollar).
		QueryRow()

	if err := row.Scan(&message.ID, &message.Author, &message.CreatedAt,
		&message.Title, &message.Body, &message.URL,
		pq.Array(&message.Replies)); err != nil {
		return nil, err
	}

	return message, nil
}

func (m *MessagesRepo) CreateOne(message *models.Message) (*models.
	Message,
	error) {
	row := sq.Insert("messages").
		Columns("author", "title", "body", "url").
		Values(message.Author, message.Title, message.Body, message.URL).
		Suffix(" RETURNING id, author, created_at, title, body, url").
		RunWith(m.conn).
		PlaceholderFormat(sq.Dollar).
		QueryRow()

	if err := row.Scan(&message.ID, &message.Author, &message.CreatedAt,
		&message.Title, &message.Body, &message.URL); err != nil {
		return nil, err
	}

	return message, nil
}

func (m *MessagesRepo) CreateReplyTo(id uuid.UUID, message *models.Message) error {
	newMsg, err := m.CreateOne(message)

	if err != nil {
		return err
	}

	tx, err := m.conn.Begin()
	if err != nil {
		return err
	}

	_, err = sq.Update("messages").
		Set("replies", sq.Expr("array_append(replies::uuid[], ?)", newMsg.ID)).
		Where(sq.Eq{"id": id}).
		RunWith(tx).
		PlaceholderFormat(sq.Dollar).
		Exec()

	if err != nil {
		if err := tx.Rollback(); err != nil {
			return err
		}
		return err
	}

	return tx.Commit()
}
