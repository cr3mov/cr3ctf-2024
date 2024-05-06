package models

import (
	"fmt"
	"github.com/google/uuid"
	"strings"
	"time"
)

type Message struct {
	ID              uuid.UUID   `json:"id" db:"id"`
	Author          uuid.UUID   `json:"author" db:"author"`
	CreatedAt       time.Time   `json:"created_at" db:"created_at"`
	Title           string      `json:"title" db:"title"`
	Body            string      `json:"body" db:"body"`
	URL             string      `json:"url" db:"url"`
	Replies         []uuid.UUID `json:"replies" db:"replies"`
	RepliesMessages []Message   `db:"_replies"`
}

// Dumbest scan function ever
func (m *Message) Scan(val interface{}) error {

	var data string

	switch v := val.(type) {
	case []byte:
		data = string(v)
	case string:
		data = v
	default:
		return fmt.Errorf("wow, retarded it is")
	}

	id, rem, _ := strings.Cut(data[1:], ",")
	err := m.ID.Scan(id)

	if err != nil {
		return err
	}

	author, rem, _ := strings.Cut(rem, ",")
	err = m.Author.Scan(author)

	if err != nil {
		return err
	}

	createdAt, rem, _ := strings.Cut(rem, ",")
	// parse time ex. 2024-03-06 13:45:42.388867
	m.CreatedAt, err = time.Parse("2006-01-02 15:04:05.999999", strings.ReplaceAll(createdAt, "\"", ""))

	if err != nil {
		return err
	}

	/*title*/
	_, rem, _ = strings.Cut(rem, ",")

	if rem[0] != '"' {
		m.Body = rem[0 : len(rem)-1]
	} else {
		m.Body = rem[1 : len(rem)-2]
	}

	return nil
}
