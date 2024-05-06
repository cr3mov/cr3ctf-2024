package repositories

import (
	"chall/models"
	"database/sql"
	"fmt"

	sq "github.com/Masterminds/squirrel"
	"github.com/google/uuid"
)

type UsersRepo struct {
	conn   *sql.DB
	suConn *sql.DB
}

func NewUsers(conn, suConn *sql.DB) *UsersRepo {
	return &UsersRepo{conn: conn, suConn: suConn}
}

func (u *UsersRepo) Cleanup(user *models.User) {
	sq.Update("users").
		Set("pp_length", -1).
		Where(sq.Eq{
			"id": user.ID,
		}).
		RunWith(u.suConn).
		PlaceholderFormat(sq.Dollar).
		Exec()
}

func (u *UsersRepo) CreateAdmin(user *models.User) (*models.User, error) {
	row := sq.Insert("users").
		Columns("username", "password").
		Values(user.Username, user.Password).
		Suffix(" RETURNING *").
		RunWith(u.suConn).
		PlaceholderFormat(sq.Dollar).
		QueryRow()

	if err := row.Scan(&user.ID, &user.PPLength, &user.Username, &user.Password); err != nil {
		return nil, err
	}

	_, err := sq.Insert("admins").
		Columns("id").
		Values(user.ID).
		RunWith(u.suConn).
		PlaceholderFormat(sq.Dollar).
		Exec()

	if err != nil {
		return nil, err
	}

	return user, nil
}

func (u *UsersRepo) CreateOne(user *models.User) (*models.User, error) {
	row := sq.Insert("users").
		Columns("username", "password").
		Values(user.Username, user.Password).
		Suffix(" RETURNING *").
		RunWith(u.conn).
		PlaceholderFormat(sq.Dollar).
		QueryRow()

	if err := row.Scan(&user.ID, &user.PPLength, &user.Username, &user.Password); err != nil {
		return nil, err
	}

	return user, nil
}

func (u *UsersRepo) GetOneById(id uuid.UUID) (*models.User, error) {
	user := &models.User{}
	row := sq.Select("id", "username", "password", "pp_length").
		From("users").
		Where(sq.Eq{"id": id}).
		RunWith(u.conn).
		PlaceholderFormat(sq.Dollar).
		QueryRow()

	if err := row.Scan(&user.ID, &user.Username, &user.Password, &user.PPLength); err != nil {
		return nil, err
	}

	return user, nil
}

func (u *UsersRepo) GetOne(user *models.User) (*models.User, error) {
	row := sq.Select("id", "username", "password", "pp_length").
		From("users").
		Where(sq.And{
			sq.Eq{"username": user.Username},
			sq.Eq{"password": user.Password},
		}).
		RunWith(u.conn).
		PlaceholderFormat(sq.Dollar).
		QueryRow()

	if err := row.Scan(&user.ID, &user.Username, &user.Password, &user.PPLength); err != nil {
		return nil, err
	}

	return user, nil
}

func (u *UsersRepo) FindMany(query string) ([]*models.User, error) {
	rows, err := sq.Select("username").
		From("users").
		Where(sq.Expr(fmt.Sprintf("username LIKE '%%%s%%'", query))).
		RunWith(u.conn).
		PlaceholderFormat(sq.Dollar).
		Query()

	if err != nil {
		return nil, err
	}

	users := make([]*models.User, 0)
	for rows.Next() {
		user := &models.User{}

		if err := rows.Scan(&user.Username); err != nil {
			return nil, err
		}

		users = append(users, user)
	}

	return users, nil
}
