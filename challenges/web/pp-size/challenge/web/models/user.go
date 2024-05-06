package models

import (
	"github.com/google/uuid"
)

type User struct {
	ID       uuid.UUID `json:"id" db:"id"`
	PPLength int       `json:"pp_length" db:"pp_length"`
	Username string    `json:"username" db:"username"`
	Password []byte    `json:"password" db:"password"`
	IsAdmin  bool
}

func (u *User) SetIsAdmin(admin *User) bool {
	u.IsAdmin = u.ID == admin.ID
	return u.IsAdmin
}
