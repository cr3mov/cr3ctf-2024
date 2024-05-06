CREATE TABLE users(
    id uuid NOT NULL DEFAULT gen_random_uuid(),
    pp_length int NOT NULL DEFAULT floor(random() * 7 + 1)::int,
    username text NOT NULL,
    password bytea NOT NULL
);

CREATE UNIQUE INDEX users_username_idx ON users(username);
CREATE UNIQUE INDEX users_id_idx ON users(id);

CREATE TABLE admins(
    id uuid NOT NULL references users(id) ON DELETE CASCADE
);

CREATE TABLE messages(
    id uuid NOT NULL DEFAULT gen_random_uuid(),
    author uuid NOT NULL references users(id) ON DELETE CASCADE,
    created_at timestamp NOT NULL DEFAULT now(),
    title text NOT NULL,
    body text NOT NULL,
    url text NOT NULL,
    replies uuid[] NOT NULL DEFAULT '{}'
);

CREATE UNIQUE INDEX messages_url_idx ON messages(url) where url <> '';
CREATE UNIQUE INDEX messages_id_idx ON messages(id);
CREATE INDEX messages_author_idx ON messages(author);

CREATE TABLE insertion_memo(
  id uuid NOT NULL references users(id) ON DELETE CASCADE,
  pp_length int NOT NULL,
  created_at timestamp NOT NULL DEFAULT now()
);

CREATE OR REPLACE FUNCTION store_on_insert()
    RETURNS TRIGGER AS $$
BEGIN
    INSERT INTO insertion_memo(id, pp_length)
        VALUES (NEW.id, NEW.pp_length);

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;
CREATE TRIGGER users_pp_trigger AFTER INSERT ON users FOR EACH ROW EXECUTE PROCEDURE store_on_insert();

CREATE USER dummy WITH PASSWORD 'dummy' LOGIN;
GRANT INSERT, SELECT ON users TO dummy;
GRANT INSERT ON insertion_memo TO dummy;