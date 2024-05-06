# 7ELEVM writeup

## Description

Since my mother kept bothering me about looking for a job, I decided to finally find one and now I work at 7 eleven. 
On my first day, when I arrived at the checkout counter, some weird program was asking me for a bizarre 'flag'.
Can you help me with finding this weird flag?

Usage: `chall rom`

**author**: es3n1n

## Overview

```cpp
  LOBYTE(v66) = 0; //<-- opcode
  v68[0] = (__int64)&off_5373A8;
  v70[0] = 1; //<-- opcode
  v71[0] = (__int64)&off_537428;
  v73[0] = 2; //<-- opcode
  v74[0] = (__int64)&off_5374A8;
  v76[0] = 3; //<-- opcode
  v77[0] = (__int64)&off_537528;
  v79[0] = 4; //<-- opcode
  v80[0] = (__int64)&off_5375A8;
  v82[0] = 5; //<-- opcode
  v83[0] = (__int64)&off_537628;
  v85[0] = 6; //<-- opcode
  v86[0] = (__int64)&off_5376A8;
  v88[0] = 7; //<-- opcode
  v89[0] = (__int64)&off_537728;
  v91[0] = 8; //<-- opcode
  v92[0] = (__int64)&off_5377A8;
  v94[0] = 9; //<-- opcode
  v95[0] = (__int64)&off_537828;
  v97[0] = 10; //<-- opcode
  v98[0] = (__int64)&off_5378A8;
  v100[0] = 11; //<-- opcode
  v101[0] = (__int64)&off_537928;
  v103[0] = 12; //<-- opcode
  v104[0] = (__int64)&off_5379A8;
  v106[0] = 13; //<-- opcode
  v107[0] = (__int64)&off_537A28;
  v109[0] = 14; //<-- opcode
  v110[0] = (__int64)&off_537AA8;
  v112[0] = 15; //<-- opcode
  v113[0] = (__int64)&off_537B28;
  ```

this off stuff is essentially just a bunch of lambdas:
```cpp
.data.rel.ro:0000000000537AA8 off_537AA8      dq offset nullsub_17    ; DATA XREF: sub_307AD0+630↑o
.data.rel.ro:0000000000537AA8                                         ; sub_31FE80+B↑o ...
.data.rel.ro:0000000000537AB0                 dq offset sub_31FE70
.data.rel.ro:0000000000537AB8                 dq offset sub_31FE80
.data.rel.ro:0000000000537AC0                 dq offset sub_31FEA0
.data.rel.ro:0000000000537AC8                 dq offset nullsub_15
.data.rel.ro:0000000000537AD0                 dq offset sub_31FEC0
.data.rel.ro:0000000000537AD8                 dq offset sub_31FED0 // <--- do_call with std::function stub
.data.rel.ro:0000000000537AE0                 dq offset sub_3202D0
.data.rel.ro:0000000000537AE8                 dq offset sub_3202F0
```

by tracing these lambdas and guessing, you can deduce these vm handlers:
```py
class CMD(IntEnum):
    MOV_IMM = 0
    ADD = 1
    CMP = 2
    JZ = 3
    XOR = 4
    SUB = 5
    PUSH = 6
    INPUT_OUTPUT = 7
    EXIT = 8
    CUBE_UP = 9
    CUBE_DOWN = 10
    CUBE_LEFT = 11
    CUBE_RIGHT = 12
    CUBE_FRONT = 13
    CUBE_BACK = 14
    CUBE_CMP_SIDES = 15
```

However, naming the `CUBE_***` instructions could be a very tedious process, which would require debugging it manually and looking at the states before/after the execution, but even if you didn't understand what these handlers are doing, you can trace that these handlers are taking one argument thus by calling with inverted argument you can revert the operation.

Each byte of the payload is encrypted with two 4-bit keys that are xored. Same for u32, two 32bit keys are xored.
```py
def decrypt_u8(val: int) -> int:
    vv = chunks(bin(val)[2:].rjust(8, '0'), 4)
    return int(vv[0], 2) ^ int(vv[1], 2)


def decrypt_u32(lhs, rhs) -> int:
    lhs = le(bytes(lhs))
    rhs = le(bytes(rhs))
    return lhs ^ rhs
```

Each VM command is dumped to bits and serialized as barcodes, so what we need to do is read these barcodes and read them as bytes.
```py
commands = list()
rom_dir = Path(__file__).parent / 'rom'
for file in list(rom_dir.iterdir()):
    detected = decode(cv2.cvtColor(cv2.imread(str(file.absolute()), ), cv2.COLOR_BGR2RGB))
    assert len(detected) == 1

    decoded_bytes = [int(x, 2) for x in chunks(detected[0].data.decode(), 8)]

    opcode = decrypt_u8(decoded_bytes[0])
    op1 = decrypt_u8(decoded_bytes[1])
    op2 = decrypt_u8(decoded_bytes[2])
    imm = decrypt_u32(decoded_bytes[3:7], decoded_bytes[7:])

    commands.append({
        'opcode': opcode,
        'op1': op1,
        'op2': op2,
        'imm': imm
    })
```

Essentially, this cube is implemented in a way that it stores each side of the cube as an `uint32_t`, where each 4 bits of it represents a color per cell.

Each side has its own color, just like the original Rubik's cube.

So, in summary, we have six sides and six colors, all of which are from 0 to 5. 

After we enter the flag, the program uses it to generate the Rubik's cube. There are six possible chars for a flag character, and the flag should consist of 6*8 characters (one character per block cell).

Here are the characters:
* `|` = `0`
* `l` = `1`
* `_` = `2`
* `I` = `3`
* `1` = `4`
* `i` = `5`

So if we enter `||||||||llllllll________IIIIIIII11111111iiiiiiii`, the cube would look as follows:
```
    |||
    |||
    |||
lll ___ III 111
lll ___ III 111
lll ___ III 111
    iii
    iii
    iii
```

This is how the solved Rubik's cube looks.

But the twist here is that after we enter the flag, the program does 100 spins of this cube in random(predefined, of course) directions.

So if we want to get the payload with which, after 100 spins, the cube would be solved, we first have to dump them.

```py
for insn in data:
    opcode = insn['opcode']
    op1 = insn['op1']
    op2 = insn['op2']
    imm = insn['imm']

    reverse = imm > 0

    if opcode == 9:
        print(f'data_t(UP, {str(reverse).lower()}),')
    elif opcode == 10:
        print(f'data_t(DOWN, {str(reverse).lower()}),')
    elif opcode == 11:
        print(f'data_t(LEFT, {str(reverse).lower()}),')
    elif opcode == 12:
        print(f'data_t(RIGHT, {str(reverse).lower()}),')
    elif opcode == 13:
        print(f'data_t(FRONT, {str(reverse).lower()}),')
    elif opcode == 14:
        print(f'data_t(BACK, {str(reverse).lower()}),')
```

Then, we have to set the initial cube state to the solved state, do all of these operations in reverse order and set `reversed`(1st argument) value to the `!reversed` value of the vm command.

```cpp
Cube cube;
cube.sides[0] = 0;
cube.sides[1] = 0x11111111;
cube.sides[2] = 0x22222222;
cube.sides[3] = 0x33333333;
cube.sides[4] = 0x44444444;
cube.sides[5] = 0x55555555;

std::cout << (cube.solved() ? "true" : "false") << std::endl << cube << std::endl;

auto rot = [&](int type, bool reverse) {
    switch (type) {
    case UP:
        cube.up(reverse);
        return;
    case DOWN:
        cube.down(reverse);
        return;
    case LEFT:
        cube.left(reverse);
        return;
    case RIGHT:
        cube.right(reverse);
        return;
    case FRONT:
        cube.front(reverse);
        return;
    case BACK:
        cube.back(reverse);
        return;
    };

    __debugbreak();
    };

for (int i = 100 - 1; i >= 0; --i) {
    auto& it = rotations[i];
    rot(it.type, !it.reverse);
}

std::cout << (cube.solved() ? "true" : "false") << std::endl << cube << std::endl;
```

Then, serializing the state of the Rubik's cube in the same format as the flag checker wants.

```cpp
    for (auto& side : cube.sides) {
        std::vector<std::uint32_t> side_bits = {};
        for (int i = 0; i < 8; ++i) {
            side_bits.push_back((side >> (i * 4)) & 0xF);
        }

        //std::reverse(side_bits.begin(), side_bits.end());

        for (auto v : side_bits) {
            printf("%c", stringify(v));
        }
    }
```

Which prints us `1Il|Il_il_i_ll11Iiii_|_l1I_l1i|1|_iI|_||i1IIl|I1`.

```
⠀⠀⢀⡀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⠀⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⢠⢿⣳⠋⣿⢋⡇⠀⠀⠸⡈⡼⣿⠢⡈⢦⡀⠀⢱⢤⠀⠀⠀⠀⠮⠊⠱⠂
⠀⡞⢡⡏⣾⠸⠎⢀⡆⡼⣄⠑⢱⣿⡖⠈⢪⣷⡄⠀⣷⢣⠀⠀⠀⠀⡠⡬⠂
⠰⢀⣾⣴⠛⡀⠀⢸⣧⡏⢻⣷⢸⣿⡧⣰⣶⠿⣹⡄⢉⠻⣇⠀⠀⣀⡋⠀⠀
⠀⠜⡱⠏⠀⡇⢠⣾⢹⣁⣨⡽⣿⣿⡀⠉⣿⡶⠉⢻⣿⡄⡿⡄⠀⠙⠃⠀⠀
⠀⢸⡀⣦⢴⣷⣼⣿⠸⣿⣿⣧⠈⠃⠀⠀⠀⠀⠀⠀⣿⣇⡇⠀⠀⠀⠀⠀⠀
⢸⣿⣿⣿⣾⣿⣿⣷⠈⠂⠙⠊⠀⠀⠀⠀⠀⠀⠀⢀⣿⣿⡇⠀⠀⠀⠀⠀⠀
⢸⣿⣿⣿⣿⣿⣿⣿⣷⣆⠀⠀⠀⠀⠀⠀⠀⠀⣠⢿⣿⡏⠀⠀⠀⠀⠀⠀⠀
⠆⣿⣿⣿⣿⣿⣿⣿⣿⠙⢷⣦⣤⡄⠀⠀⠀⠈⢼⠸⡿⠀⠀⠀⠀⠀⠀⠀⠀
⠀⢿⢹⣿⣿⣿⣿⣿⣿⡇⠈⠉⠻⠟⠄⠀⠄⠀⠀⠠⠇⠀⠀⠀⠀⠀⠀
1Il|Il_il_i_ll11Iiii_|_l1I_l1i|1|_iI|_||i1IIl|I1
Correct
*** end of execution
```

Wrapping it in `cr3{...}` and submitting the flag.

`cr3{1Il|Il_il_i_ll11Iiii_|_l1I_l1i|1|_iI|_||i1IIl|I1}`
