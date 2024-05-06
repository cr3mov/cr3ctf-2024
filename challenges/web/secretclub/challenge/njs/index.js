const express = require('express');
const hpp = require('hpp');

const app = express();
app.use(hpp());

app.get('/', (req, res) => {
	res.status(400).send(`
		<html>
		<head>
			<title>Error! You're not a club member!</title>
		</head>
		<body>
			<h1>Access for ${req.query.role} Denied</h1>
			<p>You are attempting to access this super duper cool fan club but you're not a member.</p>
			<p>Your (not-so) unique ID is: ${Math.floor(Math.random() * 13371337)}</p>
		</body>
		</html>
	`);
});

const PORT = process.env.PORT || 3000;

app.listen({ port: PORT, host: '0.0.0.0' }, () => {
  console.log(`Listening at ${PORT}`);
});
