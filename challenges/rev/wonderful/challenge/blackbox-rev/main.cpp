#include <cstdint>
#include <cstdio>
#include <iostream>
#include <Windows.h>

// #define _NO_VIRT

#if defined(NDEBUG) && !defined(_NO_VIRT)
    #include "themida/ThemidaSDK.h"
#else
    #define VM_TIGER_WHITE_START /* */
    #define VM_TIGER_WHITE_END /* */
#endif

int err(int code) {
    ::exit(printf("error %d\n", code));
}

#define NUMARGS(...) (sizeof((int[]){__VA_ARGS__}) / sizeof(int))
#define INVOKE_CODE_STUB(n, ...)                                                  \
    volatile uint8_t data_##n[NUMARGS(__VA_ARGS__)] = {};                         \
    for (std::size_t kk = 0; kk < sizeof(data_##n); ++kk) {                       \
        data_##n[kk] = ((uint8_t[]){__VA_ARGS__})[kk];                            \
    }                                                                             \
    counter += ((uint64_t(*)(uint64_t))data_##n)(static_cast<uint64_t>(flag[n])); \
    memset((void*)data_##n, 0, NUMARGS(__VA_ARGS__));

int main() {
    VM_TIGER_WHITE_START;
    size_t counter = 0;

    MEMORY_BASIC_INFORMATION mbi = {};
    DWORD old_prot;

    printf("[*] reprotecting stack through VirtualQuery&VirtualProtect...\n");
    if (!VirtualQuery(&mbi, &mbi, sizeof(mbi))) {
        err(1);
    }
    if (!VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &old_prot)) {
        err(2);
    }

    uint64_t res = 0;
    char flag[128] = {0};

    printf("[i] please enter the flag: ");
    std::cin >> flag;

    if (flag[0] != 'c' || flag[1] != 'r' || flag[2] != '3' || flag[3] != '{') {
        printf("[!] flag should start with `cr3{`\n");
        return 1;
    }

    printf("[i] hint: can you figure what am i calling?\n");

    // peb_check;xor;0x4f
    volatile uint8_t data_15[59] = {};
    // clang-format off
    data_15[0] = 0x8a; data_15[1] = 0x9; data_15[2] = 0x48; data_15[3] = 0x81; data_15[4] = 0xe1; data_15[5] = 0xff; data_15[6] = 0x0; data_15[7] = 0x0; data_15[8] = 0x0; data_15[9] = 0x65; data_15[10] = 0x4c; data_15[11] = 0x8b; data_15[12] = 0x14; data_15[13] = 0x25; data_15[14] = 0x60; data_15[15] = 0x0; data_15[16] = 0x0; data_15[17] = 0x0; data_15[18] = 0x48; data_15[19] = 0x31; data_15[20] = 0xd2; data_15[21] = 0x41; data_15[22] = 0x8a; data_15[23] = 0x52; data_15[24] = 0x2; data_15[25] = 0x83; data_15[26] = 0xc2; data_15[27] = 0x1; data_15[28] = 0x48; data_15[29] = 0xc1; data_15[30] = 0xe2; data_15[31] = 0x4; data_15[32] = 0x48; data_15[33] = 0x89; data_15[34] = 0xc8; data_15[35] = 0x48; data_15[36] = 0x31; data_15[37] = 0xd0; data_15[38] = 0x49; data_15[39] = 0xbb; data_15[40] = 0x4f; data_15[41] = 0x0; data_15[42] = 0x0; data_15[43] = 0x0; data_15[44] = 0x0; data_15[45] = 0x0; data_15[46] = 0x0; data_15[47] = 0x0; data_15[48] = 0x4c; data_15[49] = 0x39; data_15[50] = 0xd8; data_15[51] = 0xf; data_15[52] = 0x94; data_15[53] = 0xc0; data_15[54] = 0x48; data_15[55] = 0x83; data_15[56] = 0xe0; data_15[57] = 0x1; data_15[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_15)(&flag[15]);
    counter += res;
    memset((void*)data_15, 0, 59);

    // peb_check;sub;0x5b
    volatile uint8_t data_72[59] = {};
    // clang-format off
    data_72[0] = 0x8a; data_72[1] = 0x9; data_72[2] = 0x48; data_72[3] = 0x81; data_72[4] = 0xe1; data_72[5] = 0xff; data_72[6] = 0x0; data_72[7] = 0x0; data_72[8] = 0x0; data_72[9] = 0x65; data_72[10] = 0x4c; data_72[11] = 0x8b; data_72[12] = 0x14; data_72[13] = 0x25; data_72[14] = 0x60; data_72[15] = 0x0; data_72[16] = 0x0; data_72[17] = 0x0; data_72[18] = 0x48; data_72[19] = 0x31; data_72[20] = 0xd2; data_72[21] = 0x41; data_72[22] = 0x8a; data_72[23] = 0x52; data_72[24] = 0x2; data_72[25] = 0x83; data_72[26] = 0xc2; data_72[27] = 0x1; data_72[28] = 0x48; data_72[29] = 0xc1; data_72[30] = 0xe2; data_72[31] = 0x4; data_72[32] = 0x48; data_72[33] = 0x89; data_72[34] = 0xc8; data_72[35] = 0x48; data_72[36] = 0x29; data_72[37] = 0xd0; data_72[38] = 0x49; data_72[39] = 0xbb; data_72[40] = 0x5b; data_72[41] = 0x0; data_72[42] = 0x0; data_72[43] = 0x0; data_72[44] = 0x0; data_72[45] = 0x0; data_72[46] = 0x0; data_72[47] = 0x0; data_72[48] = 0x4c; data_72[49] = 0x39; data_72[50] = 0xd8; data_72[51] = 0xf; data_72[52] = 0x94; data_72[53] = 0xc0; data_72[54] = 0x48; data_72[55] = 0x83; data_72[56] = 0xe0; data_72[57] = 0x1; data_72[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_72)(&flag[72]);
    counter += res;
    memset((void*)data_72, 0, 59);

    // imm64;xor;0x17
    volatile uint8_t data_65[46] = {};
    // clang-format off
    data_65[0] = 0x8a; data_65[1] = 0x9; data_65[2] = 0x48; data_65[3] = 0x81; data_65[4] = 0xe1; data_65[5] = 0xff; data_65[6] = 0x0; data_65[7] = 0x0; data_65[8] = 0x0; data_65[9] = 0x48; data_65[10] = 0xba; data_65[11] = 0x7f; data_65[12] = 0x0; data_65[13] = 0x0; data_65[14] = 0x0; data_65[15] = 0x0; data_65[16] = 0x0; data_65[17] = 0x0; data_65[18] = 0x0; data_65[19] = 0x48; data_65[20] = 0x89; data_65[21] = 0xc8; data_65[22] = 0x48; data_65[23] = 0x31; data_65[24] = 0xd0; data_65[25] = 0x49; data_65[26] = 0xbb; data_65[27] = 0x17; data_65[28] = 0x0; data_65[29] = 0x0; data_65[30] = 0x0; data_65[31] = 0x0; data_65[32] = 0x0; data_65[33] = 0x0; data_65[34] = 0x0; data_65[35] = 0x4c; data_65[36] = 0x39; data_65[37] = 0xd8; data_65[38] = 0xf; data_65[39] = 0x94; data_65[40] = 0xc0; data_65[41] = 0x48; data_65[42] = 0x83; data_65[43] = 0xe0; data_65[44] = 0x1; data_65[45] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_65)(&flag[65]);
    counter += res;
    memset((void*)data_65, 0, 46);

    // peb_check;xor;0x23
    volatile uint8_t data_38[59] = {};
    // clang-format off
    data_38[0] = 0x8a; data_38[1] = 0x9; data_38[2] = 0x48; data_38[3] = 0x81; data_38[4] = 0xe1; data_38[5] = 0xff; data_38[6] = 0x0; data_38[7] = 0x0; data_38[8] = 0x0; data_38[9] = 0x65; data_38[10] = 0x4c; data_38[11] = 0x8b; data_38[12] = 0x14; data_38[13] = 0x25; data_38[14] = 0x60; data_38[15] = 0x0; data_38[16] = 0x0; data_38[17] = 0x0; data_38[18] = 0x48; data_38[19] = 0x31; data_38[20] = 0xd2; data_38[21] = 0x41; data_38[22] = 0x8a; data_38[23] = 0x52; data_38[24] = 0x2; data_38[25] = 0x83; data_38[26] = 0xc2; data_38[27] = 0x1; data_38[28] = 0x48; data_38[29] = 0xc1; data_38[30] = 0xe2; data_38[31] = 0x4; data_38[32] = 0x48; data_38[33] = 0x89; data_38[34] = 0xc8; data_38[35] = 0x48; data_38[36] = 0x31; data_38[37] = 0xd0; data_38[38] = 0x49; data_38[39] = 0xbb; data_38[40] = 0x23; data_38[41] = 0x0; data_38[42] = 0x0; data_38[43] = 0x0; data_38[44] = 0x0; data_38[45] = 0x0; data_38[46] = 0x0; data_38[47] = 0x0; data_38[48] = 0x4c; data_38[49] = 0x39; data_38[50] = 0xd8; data_38[51] = 0xf; data_38[52] = 0x94; data_38[53] = 0xc0; data_38[54] = 0x48; data_38[55] = 0x83; data_38[56] = 0xe0; data_38[57] = 0x1; data_38[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_38)(&flag[38]);
    counter += res;
    memset((void*)data_38, 0, 59);

    // peb_check;add;0x85
    volatile uint8_t data_91[59] = {};
    // clang-format off
    data_91[0] = 0x8a; data_91[1] = 0x9; data_91[2] = 0x48; data_91[3] = 0x81; data_91[4] = 0xe1; data_91[5] = 0xff; data_91[6] = 0x0; data_91[7] = 0x0; data_91[8] = 0x0; data_91[9] = 0x65; data_91[10] = 0x4c; data_91[11] = 0x8b; data_91[12] = 0x14; data_91[13] = 0x25; data_91[14] = 0x60; data_91[15] = 0x0; data_91[16] = 0x0; data_91[17] = 0x0; data_91[18] = 0x48; data_91[19] = 0x31; data_91[20] = 0xd2; data_91[21] = 0x41; data_91[22] = 0x8a; data_91[23] = 0x52; data_91[24] = 0x2; data_91[25] = 0x83; data_91[26] = 0xc2; data_91[27] = 0x1; data_91[28] = 0x48; data_91[29] = 0xc1; data_91[30] = 0xe2; data_91[31] = 0x4; data_91[32] = 0x48; data_91[33] = 0x89; data_91[34] = 0xc8; data_91[35] = 0x48; data_91[36] = 0x1; data_91[37] = 0xd0; data_91[38] = 0x49; data_91[39] = 0xbb; data_91[40] = 0x85; data_91[41] = 0x0; data_91[42] = 0x0; data_91[43] = 0x0; data_91[44] = 0x0; data_91[45] = 0x0; data_91[46] = 0x0; data_91[47] = 0x0; data_91[48] = 0x4c; data_91[49] = 0x39; data_91[50] = 0xd8; data_91[51] = 0xf; data_91[52] = 0x94; data_91[53] = 0xc0; data_91[54] = 0x48; data_91[55] = 0x83; data_91[56] = 0xe0; data_91[57] = 0x1; data_91[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_91)(&flag[91]);
    counter += res;
    memset((void*)data_91, 0, 59);

    // kusd_check;add;0x78
    volatile uint8_t data_21[53] = {};
    // clang-format off
    data_21[0] = 0x8a; data_21[1] = 0x9; data_21[2] = 0x48; data_21[3] = 0x81; data_21[4] = 0xe1; data_21[5] = 0xff; data_21[6] = 0x0; data_21[7] = 0x0; data_21[8] = 0x0; data_21[9] = 0x48; data_21[10] = 0x31; data_21[11] = 0xd2; data_21[12] = 0x8a; data_21[13] = 0x14; data_21[14] = 0x25; data_21[15] = 0xd4; data_21[16] = 0x2; data_21[17] = 0xfe; data_21[18] = 0x7f; data_21[19] = 0x83; data_21[20] = 0xc2; data_21[21] = 0x1; data_21[22] = 0x48; data_21[23] = 0xc1; data_21[24] = 0xe2; data_21[25] = 0x4; data_21[26] = 0x48; data_21[27] = 0x89; data_21[28] = 0xc8; data_21[29] = 0x48; data_21[30] = 0x1; data_21[31] = 0xd0; data_21[32] = 0x49; data_21[33] = 0xbb; data_21[34] = 0x78; data_21[35] = 0x0; data_21[36] = 0x0; data_21[37] = 0x0; data_21[38] = 0x0; data_21[39] = 0x0; data_21[40] = 0x0; data_21[41] = 0x0; data_21[42] = 0x4c; data_21[43] = 0x39; data_21[44] = 0xd8; data_21[45] = 0xf; data_21[46] = 0x94; data_21[47] = 0xc0; data_21[48] = 0x48; data_21[49] = 0x83; data_21[50] = 0xe0; data_21[51] = 0x1; data_21[52] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_21)(&flag[21]);
    counter += res;
    memset((void*)data_21, 0, 53);

    // peb_check;xor;0x78
    volatile uint8_t data_35[59] = {};
    // clang-format off
    data_35[0] = 0x8a; data_35[1] = 0x9; data_35[2] = 0x48; data_35[3] = 0x81; data_35[4] = 0xe1; data_35[5] = 0xff; data_35[6] = 0x0; data_35[7] = 0x0; data_35[8] = 0x0; data_35[9] = 0x65; data_35[10] = 0x4c; data_35[11] = 0x8b; data_35[12] = 0x14; data_35[13] = 0x25; data_35[14] = 0x60; data_35[15] = 0x0; data_35[16] = 0x0; data_35[17] = 0x0; data_35[18] = 0x48; data_35[19] = 0x31; data_35[20] = 0xd2; data_35[21] = 0x41; data_35[22] = 0x8a; data_35[23] = 0x52; data_35[24] = 0x2; data_35[25] = 0x83; data_35[26] = 0xc2; data_35[27] = 0x1; data_35[28] = 0x48; data_35[29] = 0xc1; data_35[30] = 0xe2; data_35[31] = 0x4; data_35[32] = 0x48; data_35[33] = 0x89; data_35[34] = 0xc8; data_35[35] = 0x48; data_35[36] = 0x31; data_35[37] = 0xd0; data_35[38] = 0x49; data_35[39] = 0xbb; data_35[40] = 0x78; data_35[41] = 0x0; data_35[42] = 0x0; data_35[43] = 0x0; data_35[44] = 0x0; data_35[45] = 0x0; data_35[46] = 0x0; data_35[47] = 0x0; data_35[48] = 0x4c; data_35[49] = 0x39; data_35[50] = 0xd8; data_35[51] = 0xf; data_35[52] = 0x94; data_35[53] = 0xc0; data_35[54] = 0x48; data_35[55] = 0x83; data_35[56] = 0xe0; data_35[57] = 0x1; data_35[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_35)(&flag[35]);
    counter += res;
    memset((void*)data_35, 0, 59);

    // peb_check;xor;0x78
    volatile uint8_t data_5[59] = {};
    // clang-format off
    data_5[0] = 0x8a; data_5[1] = 0x9; data_5[2] = 0x48; data_5[3] = 0x81; data_5[4] = 0xe1; data_5[5] = 0xff; data_5[6] = 0x0; data_5[7] = 0x0; data_5[8] = 0x0; data_5[9] = 0x65; data_5[10] = 0x4c; data_5[11] = 0x8b; data_5[12] = 0x14; data_5[13] = 0x25; data_5[14] = 0x60; data_5[15] = 0x0; data_5[16] = 0x0; data_5[17] = 0x0; data_5[18] = 0x48; data_5[19] = 0x31; data_5[20] = 0xd2; data_5[21] = 0x41; data_5[22] = 0x8a; data_5[23] = 0x52; data_5[24] = 0x2; data_5[25] = 0x83; data_5[26] = 0xc2; data_5[27] = 0x1; data_5[28] = 0x48; data_5[29] = 0xc1; data_5[30] = 0xe2; data_5[31] = 0x4; data_5[32] = 0x48; data_5[33] = 0x89; data_5[34] = 0xc8; data_5[35] = 0x48; data_5[36] = 0x31; data_5[37] = 0xd0; data_5[38] = 0x49; data_5[39] = 0xbb; data_5[40] = 0x78; data_5[41] = 0x0; data_5[42] = 0x0; data_5[43] = 0x0; data_5[44] = 0x0; data_5[45] = 0x0; data_5[46] = 0x0; data_5[47] = 0x0; data_5[48] = 0x4c; data_5[49] = 0x39; data_5[50] = 0xd8; data_5[51] = 0xf; data_5[52] = 0x94; data_5[53] = 0xc0; data_5[54] = 0x48; data_5[55] = 0x83; data_5[56] = 0xe0; data_5[57] = 0x1; data_5[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_5)(&flag[5]);
    counter += res;
    memset((void*)data_5, 0, 59);

    // peb_check;xor;0x20
    volatile uint8_t data_4[59] = {};
    // clang-format off
    data_4[0] = 0x8a; data_4[1] = 0x9; data_4[2] = 0x48; data_4[3] = 0x81; data_4[4] = 0xe1; data_4[5] = 0xff; data_4[6] = 0x0; data_4[7] = 0x0; data_4[8] = 0x0; data_4[9] = 0x65; data_4[10] = 0x4c; data_4[11] = 0x8b; data_4[12] = 0x14; data_4[13] = 0x25; data_4[14] = 0x60; data_4[15] = 0x0; data_4[16] = 0x0; data_4[17] = 0x0; data_4[18] = 0x48; data_4[19] = 0x31; data_4[20] = 0xd2; data_4[21] = 0x41; data_4[22] = 0x8a; data_4[23] = 0x52; data_4[24] = 0x2; data_4[25] = 0x83; data_4[26] = 0xc2; data_4[27] = 0x1; data_4[28] = 0x48; data_4[29] = 0xc1; data_4[30] = 0xe2; data_4[31] = 0x4; data_4[32] = 0x48; data_4[33] = 0x89; data_4[34] = 0xc8; data_4[35] = 0x48; data_4[36] = 0x31; data_4[37] = 0xd0; data_4[38] = 0x49; data_4[39] = 0xbb; data_4[40] = 0x20; data_4[41] = 0x0; data_4[42] = 0x0; data_4[43] = 0x0; data_4[44] = 0x0; data_4[45] = 0x0; data_4[46] = 0x0; data_4[47] = 0x0; data_4[48] = 0x4c; data_4[49] = 0x39; data_4[50] = 0xd8; data_4[51] = 0xf; data_4[52] = 0x94; data_4[53] = 0xc0; data_4[54] = 0x48; data_4[55] = 0x83; data_4[56] = 0xe0; data_4[57] = 0x1; data_4[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_4)(&flag[4]);
    counter += res;
    memset((void*)data_4, 0, 59);

    // peb_check;sub;0x57
    volatile uint8_t data_75[59] = {};
    // clang-format off
    data_75[0] = 0x8a; data_75[1] = 0x9; data_75[2] = 0x48; data_75[3] = 0x81; data_75[4] = 0xe1; data_75[5] = 0xff; data_75[6] = 0x0; data_75[7] = 0x0; data_75[8] = 0x0; data_75[9] = 0x65; data_75[10] = 0x4c; data_75[11] = 0x8b; data_75[12] = 0x14; data_75[13] = 0x25; data_75[14] = 0x60; data_75[15] = 0x0; data_75[16] = 0x0; data_75[17] = 0x0; data_75[18] = 0x48; data_75[19] = 0x31; data_75[20] = 0xd2; data_75[21] = 0x41; data_75[22] = 0x8a; data_75[23] = 0x52; data_75[24] = 0x2; data_75[25] = 0x83; data_75[26] = 0xc2; data_75[27] = 0x1; data_75[28] = 0x48; data_75[29] = 0xc1; data_75[30] = 0xe2; data_75[31] = 0x4; data_75[32] = 0x48; data_75[33] = 0x89; data_75[34] = 0xc8; data_75[35] = 0x48; data_75[36] = 0x29; data_75[37] = 0xd0; data_75[38] = 0x49; data_75[39] = 0xbb; data_75[40] = 0x57; data_75[41] = 0x0; data_75[42] = 0x0; data_75[43] = 0x0; data_75[44] = 0x0; data_75[45] = 0x0; data_75[46] = 0x0; data_75[47] = 0x0; data_75[48] = 0x4c; data_75[49] = 0x39; data_75[50] = 0xd8; data_75[51] = 0xf; data_75[52] = 0x94; data_75[53] = 0xc0; data_75[54] = 0x48; data_75[55] = 0x83; data_75[56] = 0xe0; data_75[57] = 0x1; data_75[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_75)(&flag[75]);
    counter += res;
    memset((void*)data_75, 0, 59);

    // imm64;sub;0x1b
    volatile uint8_t data_55[46] = {};
    // clang-format off
    data_55[0] = 0x8a; data_55[1] = 0x9; data_55[2] = 0x48; data_55[3] = 0x81; data_55[4] = 0xe1; data_55[5] = 0xff; data_55[6] = 0x0; data_55[7] = 0x0; data_55[8] = 0x0; data_55[9] = 0x48; data_55[10] = 0xba; data_55[11] = 0x4a; data_55[12] = 0x0; data_55[13] = 0x0; data_55[14] = 0x0; data_55[15] = 0x0; data_55[16] = 0x0; data_55[17] = 0x0; data_55[18] = 0x0; data_55[19] = 0x48; data_55[20] = 0x89; data_55[21] = 0xc8; data_55[22] = 0x48; data_55[23] = 0x29; data_55[24] = 0xd0; data_55[25] = 0x49; data_55[26] = 0xbb; data_55[27] = 0x1b; data_55[28] = 0x0; data_55[29] = 0x0; data_55[30] = 0x0; data_55[31] = 0x0; data_55[32] = 0x0; data_55[33] = 0x0; data_55[34] = 0x0; data_55[35] = 0x4c; data_55[36] = 0x39; data_55[37] = 0xd8; data_55[38] = 0xf; data_55[39] = 0x94; data_55[40] = 0xc0; data_55[41] = 0x48; data_55[42] = 0x83; data_55[43] = 0xe0; data_55[44] = 0x1; data_55[45] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_55)(&flag[55]);
    counter += res;
    memset((void*)data_55, 0, 46);

    // kusd_check;sub;0x60
    volatile uint8_t data_68[53] = {};
    // clang-format off
    data_68[0] = 0x8a; data_68[1] = 0x9; data_68[2] = 0x48; data_68[3] = 0x81; data_68[4] = 0xe1; data_68[5] = 0xff; data_68[6] = 0x0; data_68[7] = 0x0; data_68[8] = 0x0; data_68[9] = 0x48; data_68[10] = 0x31; data_68[11] = 0xd2; data_68[12] = 0x8a; data_68[13] = 0x14; data_68[14] = 0x25; data_68[15] = 0xd4; data_68[16] = 0x2; data_68[17] = 0xfe; data_68[18] = 0x7f; data_68[19] = 0x83; data_68[20] = 0xc2; data_68[21] = 0x1; data_68[22] = 0x48; data_68[23] = 0xc1; data_68[24] = 0xe2; data_68[25] = 0x4; data_68[26] = 0x48; data_68[27] = 0x89; data_68[28] = 0xc8; data_68[29] = 0x48; data_68[30] = 0x29; data_68[31] = 0xd0; data_68[32] = 0x49; data_68[33] = 0xbb; data_68[34] = 0x60; data_68[35] = 0x0; data_68[36] = 0x0; data_68[37] = 0x0; data_68[38] = 0x0; data_68[39] = 0x0; data_68[40] = 0x0; data_68[41] = 0x0; data_68[42] = 0x4c; data_68[43] = 0x39; data_68[44] = 0xd8; data_68[45] = 0xf; data_68[46] = 0x94; data_68[47] = 0xc0; data_68[48] = 0x48; data_68[49] = 0x83; data_68[50] = 0xe0; data_68[51] = 0x1; data_68[52] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_68)(&flag[68]);
    counter += res;
    memset((void*)data_68, 0, 53);

    // kusd_check;sub;0x66
    volatile uint8_t data_98[53] = {};
    // clang-format off
    data_98[0] = 0x8a; data_98[1] = 0x9; data_98[2] = 0x48; data_98[3] = 0x81; data_98[4] = 0xe1; data_98[5] = 0xff; data_98[6] = 0x0; data_98[7] = 0x0; data_98[8] = 0x0; data_98[9] = 0x48; data_98[10] = 0x31; data_98[11] = 0xd2; data_98[12] = 0x8a; data_98[13] = 0x14; data_98[14] = 0x25; data_98[15] = 0xd4; data_98[16] = 0x2; data_98[17] = 0xfe; data_98[18] = 0x7f; data_98[19] = 0x83; data_98[20] = 0xc2; data_98[21] = 0x1; data_98[22] = 0x48; data_98[23] = 0xc1; data_98[24] = 0xe2; data_98[25] = 0x4; data_98[26] = 0x48; data_98[27] = 0x89; data_98[28] = 0xc8; data_98[29] = 0x48; data_98[30] = 0x29; data_98[31] = 0xd0; data_98[32] = 0x49; data_98[33] = 0xbb; data_98[34] = 0x66; data_98[35] = 0x0; data_98[36] = 0x0; data_98[37] = 0x0; data_98[38] = 0x0; data_98[39] = 0x0; data_98[40] = 0x0; data_98[41] = 0x0; data_98[42] = 0x4c; data_98[43] = 0x39; data_98[44] = 0xd8; data_98[45] = 0xf; data_98[46] = 0x94; data_98[47] = 0xc0; data_98[48] = 0x48; data_98[49] = 0x83; data_98[50] = 0xe0; data_98[51] = 0x1; data_98[52] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_98)(&flag[98]);
    counter += res;
    memset((void*)data_98, 0, 53);

    // peb_check;add;0x82
    volatile uint8_t data_70[59] = {};
    // clang-format off
    data_70[0] = 0x8a; data_70[1] = 0x9; data_70[2] = 0x48; data_70[3] = 0x81; data_70[4] = 0xe1; data_70[5] = 0xff; data_70[6] = 0x0; data_70[7] = 0x0; data_70[8] = 0x0; data_70[9] = 0x65; data_70[10] = 0x4c; data_70[11] = 0x8b; data_70[12] = 0x14; data_70[13] = 0x25; data_70[14] = 0x60; data_70[15] = 0x0; data_70[16] = 0x0; data_70[17] = 0x0; data_70[18] = 0x48; data_70[19] = 0x31; data_70[20] = 0xd2; data_70[21] = 0x41; data_70[22] = 0x8a; data_70[23] = 0x52; data_70[24] = 0x2; data_70[25] = 0x83; data_70[26] = 0xc2; data_70[27] = 0x1; data_70[28] = 0x48; data_70[29] = 0xc1; data_70[30] = 0xe2; data_70[31] = 0x4; data_70[32] = 0x48; data_70[33] = 0x89; data_70[34] = 0xc8; data_70[35] = 0x48; data_70[36] = 0x1; data_70[37] = 0xd0; data_70[38] = 0x49; data_70[39] = 0xbb; data_70[40] = 0x82; data_70[41] = 0x0; data_70[42] = 0x0; data_70[43] = 0x0; data_70[44] = 0x0; data_70[45] = 0x0; data_70[46] = 0x0; data_70[47] = 0x0; data_70[48] = 0x4c; data_70[49] = 0x39; data_70[50] = 0xd8; data_70[51] = 0xf; data_70[52] = 0x94; data_70[53] = 0xc0; data_70[54] = 0x48; data_70[55] = 0x83; data_70[56] = 0xe0; data_70[57] = 0x1; data_70[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_70)(&flag[70]);
    counter += res;
    memset((void*)data_70, 0, 59);

    // peb_check;add;0x87
    volatile uint8_t data_8[59] = {};
    // clang-format off
    data_8[0] = 0x8a; data_8[1] = 0x9; data_8[2] = 0x48; data_8[3] = 0x81; data_8[4] = 0xe1; data_8[5] = 0xff; data_8[6] = 0x0; data_8[7] = 0x0; data_8[8] = 0x0; data_8[9] = 0x65; data_8[10] = 0x4c; data_8[11] = 0x8b; data_8[12] = 0x14; data_8[13] = 0x25; data_8[14] = 0x60; data_8[15] = 0x0; data_8[16] = 0x0; data_8[17] = 0x0; data_8[18] = 0x48; data_8[19] = 0x31; data_8[20] = 0xd2; data_8[21] = 0x41; data_8[22] = 0x8a; data_8[23] = 0x52; data_8[24] = 0x2; data_8[25] = 0x83; data_8[26] = 0xc2; data_8[27] = 0x1; data_8[28] = 0x48; data_8[29] = 0xc1; data_8[30] = 0xe2; data_8[31] = 0x4; data_8[32] = 0x48; data_8[33] = 0x89; data_8[34] = 0xc8; data_8[35] = 0x48; data_8[36] = 0x1; data_8[37] = 0xd0; data_8[38] = 0x49; data_8[39] = 0xbb; data_8[40] = 0x87; data_8[41] = 0x0; data_8[42] = 0x0; data_8[43] = 0x0; data_8[44] = 0x0; data_8[45] = 0x0; data_8[46] = 0x0; data_8[47] = 0x0; data_8[48] = 0x4c; data_8[49] = 0x39; data_8[50] = 0xd8; data_8[51] = 0xf; data_8[52] = 0x94; data_8[53] = 0xc0; data_8[54] = 0x48; data_8[55] = 0x83; data_8[56] = 0xe0; data_8[57] = 0x1; data_8[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_8)(&flag[8]);
    counter += res;
    memset((void*)data_8, 0, 59);

    // kusd_check;add;0x77
    volatile uint8_t data_92[53] = {};
    // clang-format off
    data_92[0] = 0x8a; data_92[1] = 0x9; data_92[2] = 0x48; data_92[3] = 0x81; data_92[4] = 0xe1; data_92[5] = 0xff; data_92[6] = 0x0; data_92[7] = 0x0; data_92[8] = 0x0; data_92[9] = 0x48; data_92[10] = 0x31; data_92[11] = 0xd2; data_92[12] = 0x8a; data_92[13] = 0x14; data_92[14] = 0x25; data_92[15] = 0xd4; data_92[16] = 0x2; data_92[17] = 0xfe; data_92[18] = 0x7f; data_92[19] = 0x83; data_92[20] = 0xc2; data_92[21] = 0x1; data_92[22] = 0x48; data_92[23] = 0xc1; data_92[24] = 0xe2; data_92[25] = 0x4; data_92[26] = 0x48; data_92[27] = 0x89; data_92[28] = 0xc8; data_92[29] = 0x48; data_92[30] = 0x1; data_92[31] = 0xd0; data_92[32] = 0x49; data_92[33] = 0xbb; data_92[34] = 0x77; data_92[35] = 0x0; data_92[36] = 0x0; data_92[37] = 0x0; data_92[38] = 0x0; data_92[39] = 0x0; data_92[40] = 0x0; data_92[41] = 0x0; data_92[42] = 0x4c; data_92[43] = 0x39; data_92[44] = 0xd8; data_92[45] = 0xf; data_92[46] = 0x94; data_92[47] = 0xc0; data_92[48] = 0x48; data_92[49] = 0x83; data_92[50] = 0xe0; data_92[51] = 0x1; data_92[52] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_92)(&flag[92]);
    counter += res;
    memset((void*)data_92, 0, 53);

    // kusd_check;sub;0x53
    volatile uint8_t data_0[53] = {};
    // clang-format off
    data_0[0] = 0x8a; data_0[1] = 0x9; data_0[2] = 0x48; data_0[3] = 0x81; data_0[4] = 0xe1; data_0[5] = 0xff; data_0[6] = 0x0; data_0[7] = 0x0; data_0[8] = 0x0; data_0[9] = 0x48; data_0[10] = 0x31; data_0[11] = 0xd2; data_0[12] = 0x8a; data_0[13] = 0x14; data_0[14] = 0x25; data_0[15] = 0xd4; data_0[16] = 0x2; data_0[17] = 0xfe; data_0[18] = 0x7f; data_0[19] = 0x83; data_0[20] = 0xc2; data_0[21] = 0x1; data_0[22] = 0x48; data_0[23] = 0xc1; data_0[24] = 0xe2; data_0[25] = 0x4; data_0[26] = 0x48; data_0[27] = 0x89; data_0[28] = 0xc8; data_0[29] = 0x48; data_0[30] = 0x29; data_0[31] = 0xd0; data_0[32] = 0x49; data_0[33] = 0xbb; data_0[34] = 0x53; data_0[35] = 0x0; data_0[36] = 0x0; data_0[37] = 0x0; data_0[38] = 0x0; data_0[39] = 0x0; data_0[40] = 0x0; data_0[41] = 0x0; data_0[42] = 0x4c; data_0[43] = 0x39; data_0[44] = 0xd8; data_0[45] = 0xf; data_0[46] = 0x94; data_0[47] = 0xc0; data_0[48] = 0x48; data_0[49] = 0x83; data_0[50] = 0xe0; data_0[51] = 0x1; data_0[52] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_0)(&flag[0]);
    counter += res;
    memset((void*)data_0, 0, 53);

    // imm64;xor;0x8e
    volatile uint8_t data_69[46] = {};
    // clang-format off
    data_69[0] = 0x8a; data_69[1] = 0x9; data_69[2] = 0x48; data_69[3] = 0x81; data_69[4] = 0xe1; data_69[5] = 0xff; data_69[6] = 0x0; data_69[7] = 0x0; data_69[8] = 0x0; data_69[9] = 0x48; data_69[10] = 0xba; data_69[11] = 0xe8; data_69[12] = 0x0; data_69[13] = 0x0; data_69[14] = 0x0; data_69[15] = 0x0; data_69[16] = 0x0; data_69[17] = 0x0; data_69[18] = 0x0; data_69[19] = 0x48; data_69[20] = 0x89; data_69[21] = 0xc8; data_69[22] = 0x48; data_69[23] = 0x31; data_69[24] = 0xd0; data_69[25] = 0x49; data_69[26] = 0xbb; data_69[27] = 0x8e; data_69[28] = 0x0; data_69[29] = 0x0; data_69[30] = 0x0; data_69[31] = 0x0; data_69[32] = 0x0; data_69[33] = 0x0; data_69[34] = 0x0; data_69[35] = 0x4c; data_69[36] = 0x39; data_69[37] = 0xd8; data_69[38] = 0xf; data_69[39] = 0x94; data_69[40] = 0xc0; data_69[41] = 0x48; data_69[42] = 0x83; data_69[43] = 0xe0; data_69[44] = 0x1; data_69[45] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_69)(&flag[69]);
    counter += res;
    memset((void*)data_69, 0, 46);

    // peb_check;sub;0x54
    volatile uint8_t data_71[59] = {};
    // clang-format off
    data_71[0] = 0x8a; data_71[1] = 0x9; data_71[2] = 0x48; data_71[3] = 0x81; data_71[4] = 0xe1; data_71[5] = 0xff; data_71[6] = 0x0; data_71[7] = 0x0; data_71[8] = 0x0; data_71[9] = 0x65; data_71[10] = 0x4c; data_71[11] = 0x8b; data_71[12] = 0x14; data_71[13] = 0x25; data_71[14] = 0x60; data_71[15] = 0x0; data_71[16] = 0x0; data_71[17] = 0x0; data_71[18] = 0x48; data_71[19] = 0x31; data_71[20] = 0xd2; data_71[21] = 0x41; data_71[22] = 0x8a; data_71[23] = 0x52; data_71[24] = 0x2; data_71[25] = 0x83; data_71[26] = 0xc2; data_71[27] = 0x1; data_71[28] = 0x48; data_71[29] = 0xc1; data_71[30] = 0xe2; data_71[31] = 0x4; data_71[32] = 0x48; data_71[33] = 0x89; data_71[34] = 0xc8; data_71[35] = 0x48; data_71[36] = 0x29; data_71[37] = 0xd0; data_71[38] = 0x49; data_71[39] = 0xbb; data_71[40] = 0x54; data_71[41] = 0x0; data_71[42] = 0x0; data_71[43] = 0x0; data_71[44] = 0x0; data_71[45] = 0x0; data_71[46] = 0x0; data_71[47] = 0x0; data_71[48] = 0x4c; data_71[49] = 0x39; data_71[50] = 0xd8; data_71[51] = 0xf; data_71[52] = 0x94; data_71[53] = 0xc0; data_71[54] = 0x48; data_71[55] = 0x83; data_71[56] = 0xe0; data_71[57] = 0x1; data_71[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_71)(&flag[71]);
    counter += res;
    memset((void*)data_71, 0, 59);

    // imm64;sub;0xffffffffffffffca
    volatile uint8_t data_67[46] = {};
    // clang-format off
    data_67[0] = 0x8a; data_67[1] = 0x9; data_67[2] = 0x48; data_67[3] = 0x81; data_67[4] = 0xe1; data_67[5] = 0xff; data_67[6] = 0x0; data_67[7] = 0x0; data_67[8] = 0x0; data_67[9] = 0x48; data_67[10] = 0xba; data_67[11] = 0xa3; data_67[12] = 0x0; data_67[13] = 0x0; data_67[14] = 0x0; data_67[15] = 0x0; data_67[16] = 0x0; data_67[17] = 0x0; data_67[18] = 0x0; data_67[19] = 0x48; data_67[20] = 0x89; data_67[21] = 0xc8; data_67[22] = 0x48; data_67[23] = 0x29; data_67[24] = 0xd0; data_67[25] = 0x49; data_67[26] = 0xbb; data_67[27] = 0xca; data_67[28] = 0xff; data_67[29] = 0xff; data_67[30] = 0xff; data_67[31] = 0xff; data_67[32] = 0xff; data_67[33] = 0xff; data_67[34] = 0xff; data_67[35] = 0x4c; data_67[36] = 0x39; data_67[37] = 0xd8; data_67[38] = 0xf; data_67[39] = 0x94; data_67[40] = 0xc0; data_67[41] = 0x48; data_67[42] = 0x83; data_67[43] = 0xe0; data_67[44] = 0x1; data_67[45] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_67)(&flag[67]);
    counter += res;
    memset((void*)data_67, 0, 46);

    // kusd_check;xor;0x74
    volatile uint8_t data_96[53] = {};
    // clang-format off
    data_96[0] = 0x8a; data_96[1] = 0x9; data_96[2] = 0x48; data_96[3] = 0x81; data_96[4] = 0xe1; data_96[5] = 0xff; data_96[6] = 0x0; data_96[7] = 0x0; data_96[8] = 0x0; data_96[9] = 0x48; data_96[10] = 0x31; data_96[11] = 0xd2; data_96[12] = 0x8a; data_96[13] = 0x14; data_96[14] = 0x25; data_96[15] = 0xd4; data_96[16] = 0x2; data_96[17] = 0xfe; data_96[18] = 0x7f; data_96[19] = 0x83; data_96[20] = 0xc2; data_96[21] = 0x1; data_96[22] = 0x48; data_96[23] = 0xc1; data_96[24] = 0xe2; data_96[25] = 0x4; data_96[26] = 0x48; data_96[27] = 0x89; data_96[28] = 0xc8; data_96[29] = 0x48; data_96[30] = 0x31; data_96[31] = 0xd0; data_96[32] = 0x49; data_96[33] = 0xbb; data_96[34] = 0x74; data_96[35] = 0x0; data_96[36] = 0x0; data_96[37] = 0x0; data_96[38] = 0x0; data_96[39] = 0x0; data_96[40] = 0x0; data_96[41] = 0x0; data_96[42] = 0x4c; data_96[43] = 0x39; data_96[44] = 0xd8; data_96[45] = 0xf; data_96[46] = 0x94; data_96[47] = 0xc0; data_96[48] = 0x48; data_96[49] = 0x83; data_96[50] = 0xe0; data_96[51] = 0x1; data_96[52] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_96)(&flag[96]);
    counter += res;
    memset((void*)data_96, 0, 53);

    // imm64;xor;0x18
    volatile uint8_t data_88[46] = {};
    // clang-format off
    data_88[0] = 0x8a; data_88[1] = 0x9; data_88[2] = 0x48; data_88[3] = 0x81; data_88[4] = 0xe1; data_88[5] = 0xff; data_88[6] = 0x0; data_88[7] = 0x0; data_88[8] = 0x0; data_88[9] = 0x48; data_88[10] = 0xba; data_88[11] = 0x2b; data_88[12] = 0x0; data_88[13] = 0x0; data_88[14] = 0x0; data_88[15] = 0x0; data_88[16] = 0x0; data_88[17] = 0x0; data_88[18] = 0x0; data_88[19] = 0x48; data_88[20] = 0x89; data_88[21] = 0xc8; data_88[22] = 0x48; data_88[23] = 0x31; data_88[24] = 0xd0; data_88[25] = 0x49; data_88[26] = 0xbb; data_88[27] = 0x18; data_88[28] = 0x0; data_88[29] = 0x0; data_88[30] = 0x0; data_88[31] = 0x0; data_88[32] = 0x0; data_88[33] = 0x0; data_88[34] = 0x0; data_88[35] = 0x4c; data_88[36] = 0x39; data_88[37] = 0xd8; data_88[38] = 0xf; data_88[39] = 0x94; data_88[40] = 0xc0; data_88[41] = 0x48; data_88[42] = 0x83; data_88[43] = 0xe0; data_88[44] = 0x1; data_88[45] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_88)(&flag[88]);
    counter += res;
    memset((void*)data_88, 0, 46);

    // peb_check;xor;0x20
    volatile uint8_t data_41[59] = {};
    // clang-format off
    data_41[0] = 0x8a; data_41[1] = 0x9; data_41[2] = 0x48; data_41[3] = 0x81; data_41[4] = 0xe1; data_41[5] = 0xff; data_41[6] = 0x0; data_41[7] = 0x0; data_41[8] = 0x0; data_41[9] = 0x65; data_41[10] = 0x4c; data_41[11] = 0x8b; data_41[12] = 0x14; data_41[13] = 0x25; data_41[14] = 0x60; data_41[15] = 0x0; data_41[16] = 0x0; data_41[17] = 0x0; data_41[18] = 0x48; data_41[19] = 0x31; data_41[20] = 0xd2; data_41[21] = 0x41; data_41[22] = 0x8a; data_41[23] = 0x52; data_41[24] = 0x2; data_41[25] = 0x83; data_41[26] = 0xc2; data_41[27] = 0x1; data_41[28] = 0x48; data_41[29] = 0xc1; data_41[30] = 0xe2; data_41[31] = 0x4; data_41[32] = 0x48; data_41[33] = 0x89; data_41[34] = 0xc8; data_41[35] = 0x48; data_41[36] = 0x31; data_41[37] = 0xd0; data_41[38] = 0x49; data_41[39] = 0xbb; data_41[40] = 0x20; data_41[41] = 0x0; data_41[42] = 0x0; data_41[43] = 0x0; data_41[44] = 0x0; data_41[45] = 0x0; data_41[46] = 0x0; data_41[47] = 0x0; data_41[48] = 0x4c; data_41[49] = 0x39; data_41[50] = 0xd8; data_41[51] = 0xf; data_41[52] = 0x94; data_41[53] = 0xc0; data_41[54] = 0x48; data_41[55] = 0x83; data_41[56] = 0xe0; data_41[57] = 0x1; data_41[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_41)(&flag[41]);
    counter += res;
    memset((void*)data_41, 0, 59);

    // peb_check;sub;0x45
    volatile uint8_t data_14[59] = {};
    // clang-format off
    data_14[0] = 0x8a; data_14[1] = 0x9; data_14[2] = 0x48; data_14[3] = 0x81; data_14[4] = 0xe1; data_14[5] = 0xff; data_14[6] = 0x0; data_14[7] = 0x0; data_14[8] = 0x0; data_14[9] = 0x65; data_14[10] = 0x4c; data_14[11] = 0x8b; data_14[12] = 0x14; data_14[13] = 0x25; data_14[14] = 0x60; data_14[15] = 0x0; data_14[16] = 0x0; data_14[17] = 0x0; data_14[18] = 0x48; data_14[19] = 0x31; data_14[20] = 0xd2; data_14[21] = 0x41; data_14[22] = 0x8a; data_14[23] = 0x52; data_14[24] = 0x2; data_14[25] = 0x83; data_14[26] = 0xc2; data_14[27] = 0x1; data_14[28] = 0x48; data_14[29] = 0xc1; data_14[30] = 0xe2; data_14[31] = 0x4; data_14[32] = 0x48; data_14[33] = 0x89; data_14[34] = 0xc8; data_14[35] = 0x48; data_14[36] = 0x29; data_14[37] = 0xd0; data_14[38] = 0x49; data_14[39] = 0xbb; data_14[40] = 0x45; data_14[41] = 0x0; data_14[42] = 0x0; data_14[43] = 0x0; data_14[44] = 0x0; data_14[45] = 0x0; data_14[46] = 0x0; data_14[47] = 0x0; data_14[48] = 0x4c; data_14[49] = 0x39; data_14[50] = 0xd8; data_14[51] = 0xf; data_14[52] = 0x94; data_14[53] = 0xc0; data_14[54] = 0x48; data_14[55] = 0x83; data_14[56] = 0xe0; data_14[57] = 0x1; data_14[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_14)(&flag[14]);
    counter += res;
    memset((void*)data_14, 0, 59);

    // kusd_check;sub;0x55
    volatile uint8_t data_18[53] = {};
    // clang-format off
    data_18[0] = 0x8a; data_18[1] = 0x9; data_18[2] = 0x48; data_18[3] = 0x81; data_18[4] = 0xe1; data_18[5] = 0xff; data_18[6] = 0x0; data_18[7] = 0x0; data_18[8] = 0x0; data_18[9] = 0x48; data_18[10] = 0x31; data_18[11] = 0xd2; data_18[12] = 0x8a; data_18[13] = 0x14; data_18[14] = 0x25; data_18[15] = 0xd4; data_18[16] = 0x2; data_18[17] = 0xfe; data_18[18] = 0x7f; data_18[19] = 0x83; data_18[20] = 0xc2; data_18[21] = 0x1; data_18[22] = 0x48; data_18[23] = 0xc1; data_18[24] = 0xe2; data_18[25] = 0x4; data_18[26] = 0x48; data_18[27] = 0x89; data_18[28] = 0xc8; data_18[29] = 0x48; data_18[30] = 0x29; data_18[31] = 0xd0; data_18[32] = 0x49; data_18[33] = 0xbb; data_18[34] = 0x55; data_18[35] = 0x0; data_18[36] = 0x0; data_18[37] = 0x0; data_18[38] = 0x0; data_18[39] = 0x0; data_18[40] = 0x0; data_18[41] = 0x0; data_18[42] = 0x4c; data_18[43] = 0x39; data_18[44] = 0xd8; data_18[45] = 0xf; data_18[46] = 0x94; data_18[47] = 0xc0; data_18[48] = 0x48; data_18[49] = 0x83; data_18[50] = 0xe0; data_18[51] = 0x1; data_18[52] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_18)(&flag[18]);
    counter += res;
    memset((void*)data_18, 0, 53);

    // kusd_check;sub;0x44
    volatile uint8_t data_20[53] = {};
    // clang-format off
    data_20[0] = 0x8a; data_20[1] = 0x9; data_20[2] = 0x48; data_20[3] = 0x81; data_20[4] = 0xe1; data_20[5] = 0xff; data_20[6] = 0x0; data_20[7] = 0x0; data_20[8] = 0x0; data_20[9] = 0x48; data_20[10] = 0x31; data_20[11] = 0xd2; data_20[12] = 0x8a; data_20[13] = 0x14; data_20[14] = 0x25; data_20[15] = 0xd4; data_20[16] = 0x2; data_20[17] = 0xfe; data_20[18] = 0x7f; data_20[19] = 0x83; data_20[20] = 0xc2; data_20[21] = 0x1; data_20[22] = 0x48; data_20[23] = 0xc1; data_20[24] = 0xe2; data_20[25] = 0x4; data_20[26] = 0x48; data_20[27] = 0x89; data_20[28] = 0xc8; data_20[29] = 0x48; data_20[30] = 0x29; data_20[31] = 0xd0; data_20[32] = 0x49; data_20[33] = 0xbb; data_20[34] = 0x44; data_20[35] = 0x0; data_20[36] = 0x0; data_20[37] = 0x0; data_20[38] = 0x0; data_20[39] = 0x0; data_20[40] = 0x0; data_20[41] = 0x0; data_20[42] = 0x4c; data_20[43] = 0x39; data_20[44] = 0xd8; data_20[45] = 0xf; data_20[46] = 0x94; data_20[47] = 0xc0; data_20[48] = 0x48; data_20[49] = 0x83; data_20[50] = 0xe0; data_20[51] = 0x1; data_20[52] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_20)(&flag[20]);
    counter += res;
    memset((void*)data_20, 0, 53);

    // peb_check;sub;0x55
    volatile uint8_t data_84[59] = {};
    // clang-format off
    data_84[0] = 0x8a; data_84[1] = 0x9; data_84[2] = 0x48; data_84[3] = 0x81; data_84[4] = 0xe1; data_84[5] = 0xff; data_84[6] = 0x0; data_84[7] = 0x0; data_84[8] = 0x0; data_84[9] = 0x65; data_84[10] = 0x4c; data_84[11] = 0x8b; data_84[12] = 0x14; data_84[13] = 0x25; data_84[14] = 0x60; data_84[15] = 0x0; data_84[16] = 0x0; data_84[17] = 0x0; data_84[18] = 0x48; data_84[19] = 0x31; data_84[20] = 0xd2; data_84[21] = 0x41; data_84[22] = 0x8a; data_84[23] = 0x52; data_84[24] = 0x2; data_84[25] = 0x83; data_84[26] = 0xc2; data_84[27] = 0x1; data_84[28] = 0x48; data_84[29] = 0xc1; data_84[30] = 0xe2; data_84[31] = 0x4; data_84[32] = 0x48; data_84[33] = 0x89; data_84[34] = 0xc8; data_84[35] = 0x48; data_84[36] = 0x29; data_84[37] = 0xd0; data_84[38] = 0x49; data_84[39] = 0xbb; data_84[40] = 0x55; data_84[41] = 0x0; data_84[42] = 0x0; data_84[43] = 0x0; data_84[44] = 0x0; data_84[45] = 0x0; data_84[46] = 0x0; data_84[47] = 0x0; data_84[48] = 0x4c; data_84[49] = 0x39; data_84[50] = 0xd8; data_84[51] = 0xf; data_84[52] = 0x94; data_84[53] = 0xc0; data_84[54] = 0x48; data_84[55] = 0x83; data_84[56] = 0xe0; data_84[57] = 0x1; data_84[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_84)(&flag[84]);
    counter += res;
    memset((void*)data_84, 0, 59);

    // imm64;sub;0xffffffffffffff7c
    volatile uint8_t data_40[46] = {};
    // clang-format off
    data_40[0] = 0x8a; data_40[1] = 0x9; data_40[2] = 0x48; data_40[3] = 0x81; data_40[4] = 0xe1; data_40[5] = 0xff; data_40[6] = 0x0; data_40[7] = 0x0; data_40[8] = 0x0; data_40[9] = 0x48; data_40[10] = 0xba; data_40[11] = 0xfd; data_40[12] = 0x0; data_40[13] = 0x0; data_40[14] = 0x0; data_40[15] = 0x0; data_40[16] = 0x0; data_40[17] = 0x0; data_40[18] = 0x0; data_40[19] = 0x48; data_40[20] = 0x89; data_40[21] = 0xc8; data_40[22] = 0x48; data_40[23] = 0x29; data_40[24] = 0xd0; data_40[25] = 0x49; data_40[26] = 0xbb; data_40[27] = 0x7c; data_40[28] = 0xff; data_40[29] = 0xff; data_40[30] = 0xff; data_40[31] = 0xff; data_40[32] = 0xff; data_40[33] = 0xff; data_40[34] = 0xff; data_40[35] = 0x4c; data_40[36] = 0x39; data_40[37] = 0xd8; data_40[38] = 0xf; data_40[39] = 0x94; data_40[40] = 0xc0; data_40[41] = 0x48; data_40[42] = 0x83; data_40[43] = 0xe0; data_40[44] = 0x1; data_40[45] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_40)(&flag[40]);
    counter += res;
    memset((void*)data_40, 0, 46);

    // peb_check;sub;0x62
    volatile uint8_t data_24[59] = {};
    // clang-format off
    data_24[0] = 0x8a; data_24[1] = 0x9; data_24[2] = 0x48; data_24[3] = 0x81; data_24[4] = 0xe1; data_24[5] = 0xff; data_24[6] = 0x0; data_24[7] = 0x0; data_24[8] = 0x0; data_24[9] = 0x65; data_24[10] = 0x4c; data_24[11] = 0x8b; data_24[12] = 0x14; data_24[13] = 0x25; data_24[14] = 0x60; data_24[15] = 0x0; data_24[16] = 0x0; data_24[17] = 0x0; data_24[18] = 0x48; data_24[19] = 0x31; data_24[20] = 0xd2; data_24[21] = 0x41; data_24[22] = 0x8a; data_24[23] = 0x52; data_24[24] = 0x2; data_24[25] = 0x83; data_24[26] = 0xc2; data_24[27] = 0x1; data_24[28] = 0x48; data_24[29] = 0xc1; data_24[30] = 0xe2; data_24[31] = 0x4; data_24[32] = 0x48; data_24[33] = 0x89; data_24[34] = 0xc8; data_24[35] = 0x48; data_24[36] = 0x29; data_24[37] = 0xd0; data_24[38] = 0x49; data_24[39] = 0xbb; data_24[40] = 0x62; data_24[41] = 0x0; data_24[42] = 0x0; data_24[43] = 0x0; data_24[44] = 0x0; data_24[45] = 0x0; data_24[46] = 0x0; data_24[47] = 0x0; data_24[48] = 0x4c; data_24[49] = 0x39; data_24[50] = 0xd8; data_24[51] = 0xf; data_24[52] = 0x94; data_24[53] = 0xc0; data_24[54] = 0x48; data_24[55] = 0x83; data_24[56] = 0xe0; data_24[57] = 0x1; data_24[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_24)(&flag[24]);
    counter += res;
    memset((void*)data_24, 0, 59);

    // imm64;xor;0x66
    volatile uint8_t data_77[46] = {};
    // clang-format off
    data_77[0] = 0x8a; data_77[1] = 0x9; data_77[2] = 0x48; data_77[3] = 0x81; data_77[4] = 0xe1; data_77[5] = 0xff; data_77[6] = 0x0; data_77[7] = 0x0; data_77[8] = 0x0; data_77[9] = 0x48; data_77[10] = 0xba; data_77[11] = 0x15; data_77[12] = 0x0; data_77[13] = 0x0; data_77[14] = 0x0; data_77[15] = 0x0; data_77[16] = 0x0; data_77[17] = 0x0; data_77[18] = 0x0; data_77[19] = 0x48; data_77[20] = 0x89; data_77[21] = 0xc8; data_77[22] = 0x48; data_77[23] = 0x31; data_77[24] = 0xd0; data_77[25] = 0x49; data_77[26] = 0xbb; data_77[27] = 0x66; data_77[28] = 0x0; data_77[29] = 0x0; data_77[30] = 0x0; data_77[31] = 0x0; data_77[32] = 0x0; data_77[33] = 0x0; data_77[34] = 0x0; data_77[35] = 0x4c; data_77[36] = 0x39; data_77[37] = 0xd8; data_77[38] = 0xf; data_77[39] = 0x94; data_77[40] = 0xc0; data_77[41] = 0x48; data_77[42] = 0x83; data_77[43] = 0xe0; data_77[44] = 0x1; data_77[45] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_77)(&flag[77]);
    counter += res;
    memset((void*)data_77, 0, 46);

    // kusd_check;sub;0x4f
    volatile uint8_t data_63[53] = {};
    // clang-format off
    data_63[0] = 0x8a; data_63[1] = 0x9; data_63[2] = 0x48; data_63[3] = 0x81; data_63[4] = 0xe1; data_63[5] = 0xff; data_63[6] = 0x0; data_63[7] = 0x0; data_63[8] = 0x0; data_63[9] = 0x48; data_63[10] = 0x31; data_63[11] = 0xd2; data_63[12] = 0x8a; data_63[13] = 0x14; data_63[14] = 0x25; data_63[15] = 0xd4; data_63[16] = 0x2; data_63[17] = 0xfe; data_63[18] = 0x7f; data_63[19] = 0x83; data_63[20] = 0xc2; data_63[21] = 0x1; data_63[22] = 0x48; data_63[23] = 0xc1; data_63[24] = 0xe2; data_63[25] = 0x4; data_63[26] = 0x48; data_63[27] = 0x89; data_63[28] = 0xc8; data_63[29] = 0x48; data_63[30] = 0x29; data_63[31] = 0xd0; data_63[32] = 0x49; data_63[33] = 0xbb; data_63[34] = 0x4f; data_63[35] = 0x0; data_63[36] = 0x0; data_63[37] = 0x0; data_63[38] = 0x0; data_63[39] = 0x0; data_63[40] = 0x0; data_63[41] = 0x0; data_63[42] = 0x4c; data_63[43] = 0x39; data_63[44] = 0xd8; data_63[45] = 0xf; data_63[46] = 0x94; data_63[47] = 0xc0; data_63[48] = 0x48; data_63[49] = 0x83; data_63[50] = 0xe0; data_63[51] = 0x1; data_63[52] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_63)(&flag[63]);
    counter += res;
    memset((void*)data_63, 0, 53);

    // peb_check;sub;0x4f
    volatile uint8_t data_50[59] = {};
    // clang-format off
    data_50[0] = 0x8a; data_50[1] = 0x9; data_50[2] = 0x48; data_50[3] = 0x81; data_50[4] = 0xe1; data_50[5] = 0xff; data_50[6] = 0x0; data_50[7] = 0x0; data_50[8] = 0x0; data_50[9] = 0x65; data_50[10] = 0x4c; data_50[11] = 0x8b; data_50[12] = 0x14; data_50[13] = 0x25; data_50[14] = 0x60; data_50[15] = 0x0; data_50[16] = 0x0; data_50[17] = 0x0; data_50[18] = 0x48; data_50[19] = 0x31; data_50[20] = 0xd2; data_50[21] = 0x41; data_50[22] = 0x8a; data_50[23] = 0x52; data_50[24] = 0x2; data_50[25] = 0x83; data_50[26] = 0xc2; data_50[27] = 0x1; data_50[28] = 0x48; data_50[29] = 0xc1; data_50[30] = 0xe2; data_50[31] = 0x4; data_50[32] = 0x48; data_50[33] = 0x89; data_50[34] = 0xc8; data_50[35] = 0x48; data_50[36] = 0x29; data_50[37] = 0xd0; data_50[38] = 0x49; data_50[39] = 0xbb; data_50[40] = 0x4f; data_50[41] = 0x0; data_50[42] = 0x0; data_50[43] = 0x0; data_50[44] = 0x0; data_50[45] = 0x0; data_50[46] = 0x0; data_50[47] = 0x0; data_50[48] = 0x4c; data_50[49] = 0x39; data_50[50] = 0xd8; data_50[51] = 0xf; data_50[52] = 0x94; data_50[53] = 0xc0; data_50[54] = 0x48; data_50[55] = 0x83; data_50[56] = 0xe0; data_50[57] = 0x1; data_50[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_50)(&flag[50]);
    counter += res;
    memset((void*)data_50, 0, 59);

    // kusd_check;xor;0x62
    volatile uint8_t data_17[53] = {};
    // clang-format off
    data_17[0] = 0x8a; data_17[1] = 0x9; data_17[2] = 0x48; data_17[3] = 0x81; data_17[4] = 0xe1; data_17[5] = 0xff; data_17[6] = 0x0; data_17[7] = 0x0; data_17[8] = 0x0; data_17[9] = 0x48; data_17[10] = 0x31; data_17[11] = 0xd2; data_17[12] = 0x8a; data_17[13] = 0x14; data_17[14] = 0x25; data_17[15] = 0xd4; data_17[16] = 0x2; data_17[17] = 0xfe; data_17[18] = 0x7f; data_17[19] = 0x83; data_17[20] = 0xc2; data_17[21] = 0x1; data_17[22] = 0x48; data_17[23] = 0xc1; data_17[24] = 0xe2; data_17[25] = 0x4; data_17[26] = 0x48; data_17[27] = 0x89; data_17[28] = 0xc8; data_17[29] = 0x48; data_17[30] = 0x31; data_17[31] = 0xd0; data_17[32] = 0x49; data_17[33] = 0xbb; data_17[34] = 0x62; data_17[35] = 0x0; data_17[36] = 0x0; data_17[37] = 0x0; data_17[38] = 0x0; data_17[39] = 0x0; data_17[40] = 0x0; data_17[41] = 0x0; data_17[42] = 0x4c; data_17[43] = 0x39; data_17[44] = 0xd8; data_17[45] = 0xf; data_17[46] = 0x94; data_17[47] = 0xc0; data_17[48] = 0x48; data_17[49] = 0x83; data_17[50] = 0xe0; data_17[51] = 0x1; data_17[52] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_17)(&flag[17]);
    counter += res;
    memset((void*)data_17, 0, 53);

    // kusd_check;sub;0x53
    volatile uint8_t data_52[53] = {};
    // clang-format off
    data_52[0] = 0x8a; data_52[1] = 0x9; data_52[2] = 0x48; data_52[3] = 0x81; data_52[4] = 0xe1; data_52[5] = 0xff; data_52[6] = 0x0; data_52[7] = 0x0; data_52[8] = 0x0; data_52[9] = 0x48; data_52[10] = 0x31; data_52[11] = 0xd2; data_52[12] = 0x8a; data_52[13] = 0x14; data_52[14] = 0x25; data_52[15] = 0xd4; data_52[16] = 0x2; data_52[17] = 0xfe; data_52[18] = 0x7f; data_52[19] = 0x83; data_52[20] = 0xc2; data_52[21] = 0x1; data_52[22] = 0x48; data_52[23] = 0xc1; data_52[24] = 0xe2; data_52[25] = 0x4; data_52[26] = 0x48; data_52[27] = 0x89; data_52[28] = 0xc8; data_52[29] = 0x48; data_52[30] = 0x29; data_52[31] = 0xd0; data_52[32] = 0x49; data_52[33] = 0xbb; data_52[34] = 0x53; data_52[35] = 0x0; data_52[36] = 0x0; data_52[37] = 0x0; data_52[38] = 0x0; data_52[39] = 0x0; data_52[40] = 0x0; data_52[41] = 0x0; data_52[42] = 0x4c; data_52[43] = 0x39; data_52[44] = 0xd8; data_52[45] = 0xf; data_52[46] = 0x94; data_52[47] = 0xc0; data_52[48] = 0x48; data_52[49] = 0x83; data_52[50] = 0xe0; data_52[51] = 0x1; data_52[52] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_52)(&flag[52]);
    counter += res;
    memset((void*)data_52, 0, 53);

    // imm64;xor;0xa5
    volatile uint8_t data_19[46] = {};
    // clang-format off
    data_19[0] = 0x8a; data_19[1] = 0x9; data_19[2] = 0x48; data_19[3] = 0x81; data_19[4] = 0xe1; data_19[5] = 0xff; data_19[6] = 0x0; data_19[7] = 0x0; data_19[8] = 0x0; data_19[9] = 0x48; data_19[10] = 0xba; data_19[11] = 0xfa; data_19[12] = 0x0; data_19[13] = 0x0; data_19[14] = 0x0; data_19[15] = 0x0; data_19[16] = 0x0; data_19[17] = 0x0; data_19[18] = 0x0; data_19[19] = 0x48; data_19[20] = 0x89; data_19[21] = 0xc8; data_19[22] = 0x48; data_19[23] = 0x31; data_19[24] = 0xd0; data_19[25] = 0x49; data_19[26] = 0xbb; data_19[27] = 0xa5; data_19[28] = 0x0; data_19[29] = 0x0; data_19[30] = 0x0; data_19[31] = 0x0; data_19[32] = 0x0; data_19[33] = 0x0; data_19[34] = 0x0; data_19[35] = 0x4c; data_19[36] = 0x39; data_19[37] = 0xd8; data_19[38] = 0xf; data_19[39] = 0x94; data_19[40] = 0xc0; data_19[41] = 0x48; data_19[42] = 0x83; data_19[43] = 0xe0; data_19[44] = 0x1; data_19[45] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_19)(&flag[19]);
    counter += res;
    memset((void*)data_19, 0, 46);

    // peb_check;xor;0x26
    volatile uint8_t data_86[59] = {};
    // clang-format off
    data_86[0] = 0x8a; data_86[1] = 0x9; data_86[2] = 0x48; data_86[3] = 0x81; data_86[4] = 0xe1; data_86[5] = 0xff; data_86[6] = 0x0; data_86[7] = 0x0; data_86[8] = 0x0; data_86[9] = 0x65; data_86[10] = 0x4c; data_86[11] = 0x8b; data_86[12] = 0x14; data_86[13] = 0x25; data_86[14] = 0x60; data_86[15] = 0x0; data_86[16] = 0x0; data_86[17] = 0x0; data_86[18] = 0x48; data_86[19] = 0x31; data_86[20] = 0xd2; data_86[21] = 0x41; data_86[22] = 0x8a; data_86[23] = 0x52; data_86[24] = 0x2; data_86[25] = 0x83; data_86[26] = 0xc2; data_86[27] = 0x1; data_86[28] = 0x48; data_86[29] = 0xc1; data_86[30] = 0xe2; data_86[31] = 0x4; data_86[32] = 0x48; data_86[33] = 0x89; data_86[34] = 0xc8; data_86[35] = 0x48; data_86[36] = 0x31; data_86[37] = 0xd0; data_86[38] = 0x49; data_86[39] = 0xbb; data_86[40] = 0x26; data_86[41] = 0x0; data_86[42] = 0x0; data_86[43] = 0x0; data_86[44] = 0x0; data_86[45] = 0x0; data_86[46] = 0x0; data_86[47] = 0x0; data_86[48] = 0x4c; data_86[49] = 0x39; data_86[50] = 0xd8; data_86[51] = 0xf; data_86[52] = 0x94; data_86[53] = 0xc0; data_86[54] = 0x48; data_86[55] = 0x83; data_86[56] = 0xe0; data_86[57] = 0x1; data_86[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_86)(&flag[86]);
    counter += res;
    memset((void*)data_86, 0, 59);

    // imm64;sub;0x17
    volatile uint8_t data_73[46] = {};
    // clang-format off
    data_73[0] = 0x8a; data_73[1] = 0x9; data_73[2] = 0x48; data_73[3] = 0x81; data_73[4] = 0xe1; data_73[5] = 0xff; data_73[6] = 0x0; data_73[7] = 0x0; data_73[8] = 0x0; data_73[9] = 0x48; data_73[10] = 0xba; data_73[11] = 0x4f; data_73[12] = 0x0; data_73[13] = 0x0; data_73[14] = 0x0; data_73[15] = 0x0; data_73[16] = 0x0; data_73[17] = 0x0; data_73[18] = 0x0; data_73[19] = 0x48; data_73[20] = 0x89; data_73[21] = 0xc8; data_73[22] = 0x48; data_73[23] = 0x29; data_73[24] = 0xd0; data_73[25] = 0x49; data_73[26] = 0xbb; data_73[27] = 0x17; data_73[28] = 0x0; data_73[29] = 0x0; data_73[30] = 0x0; data_73[31] = 0x0; data_73[32] = 0x0; data_73[33] = 0x0; data_73[34] = 0x0; data_73[35] = 0x4c; data_73[36] = 0x39; data_73[37] = 0xd8; data_73[38] = 0xf; data_73[39] = 0x94; data_73[40] = 0xc0; data_73[41] = 0x48; data_73[42] = 0x83; data_73[43] = 0xe0; data_73[44] = 0x1; data_73[45] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_73)(&flag[73]);
    counter += res;
    memset((void*)data_73, 0, 46);

    // peb_check;sub;0x65
    volatile uint8_t data_42[59] = {};
    // clang-format off
    data_42[0] = 0x8a; data_42[1] = 0x9; data_42[2] = 0x48; data_42[3] = 0x81; data_42[4] = 0xe1; data_42[5] = 0xff; data_42[6] = 0x0; data_42[7] = 0x0; data_42[8] = 0x0; data_42[9] = 0x65; data_42[10] = 0x4c; data_42[11] = 0x8b; data_42[12] = 0x14; data_42[13] = 0x25; data_42[14] = 0x60; data_42[15] = 0x0; data_42[16] = 0x0; data_42[17] = 0x0; data_42[18] = 0x48; data_42[19] = 0x31; data_42[20] = 0xd2; data_42[21] = 0x41; data_42[22] = 0x8a; data_42[23] = 0x52; data_42[24] = 0x2; data_42[25] = 0x83; data_42[26] = 0xc2; data_42[27] = 0x1; data_42[28] = 0x48; data_42[29] = 0xc1; data_42[30] = 0xe2; data_42[31] = 0x4; data_42[32] = 0x48; data_42[33] = 0x89; data_42[34] = 0xc8; data_42[35] = 0x48; data_42[36] = 0x29; data_42[37] = 0xd0; data_42[38] = 0x49; data_42[39] = 0xbb; data_42[40] = 0x65; data_42[41] = 0x0; data_42[42] = 0x0; data_42[43] = 0x0; data_42[44] = 0x0; data_42[45] = 0x0; data_42[46] = 0x0; data_42[47] = 0x0; data_42[48] = 0x4c; data_42[49] = 0x39; data_42[50] = 0xd8; data_42[51] = 0xf; data_42[52] = 0x94; data_42[53] = 0xc0; data_42[54] = 0x48; data_42[55] = 0x83; data_42[56] = 0xe0; data_42[57] = 0x1; data_42[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_42)(&flag[42]);
    counter += res;
    memset((void*)data_42, 0, 59);

    // imm64;sub;0x51
    volatile uint8_t data_93[46] = {};
    // clang-format off
    data_93[0] = 0x8a; data_93[1] = 0x9; data_93[2] = 0x48; data_93[3] = 0x81; data_93[4] = 0xe1; data_93[5] = 0xff; data_93[6] = 0x0; data_93[7] = 0x0; data_93[8] = 0x0; data_93[9] = 0x48; data_93[10] = 0xba; data_93[11] = 0x1c; data_93[12] = 0x0; data_93[13] = 0x0; data_93[14] = 0x0; data_93[15] = 0x0; data_93[16] = 0x0; data_93[17] = 0x0; data_93[18] = 0x0; data_93[19] = 0x48; data_93[20] = 0x89; data_93[21] = 0xc8; data_93[22] = 0x48; data_93[23] = 0x29; data_93[24] = 0xd0; data_93[25] = 0x49; data_93[26] = 0xbb; data_93[27] = 0x51; data_93[28] = 0x0; data_93[29] = 0x0; data_93[30] = 0x0; data_93[31] = 0x0; data_93[32] = 0x0; data_93[33] = 0x0; data_93[34] = 0x0; data_93[35] = 0x4c; data_93[36] = 0x39; data_93[37] = 0xd8; data_93[38] = 0xf; data_93[39] = 0x94; data_93[40] = 0xc0; data_93[41] = 0x48; data_93[42] = 0x83; data_93[43] = 0xe0; data_93[44] = 0x1; data_93[45] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_93)(&flag[93]);
    counter += res;
    memset((void*)data_93, 0, 46);

    // imm64;add;0x105
    volatile uint8_t data_23[46] = {};
    // clang-format off
    data_23[0] = 0x8a; data_23[1] = 0x9; data_23[2] = 0x48; data_23[3] = 0x81; data_23[4] = 0xe1; data_23[5] = 0xff; data_23[6] = 0x0; data_23[7] = 0x0; data_23[8] = 0x0; data_23[9] = 0x48; data_23[10] = 0xba; data_23[11] = 0xa6; data_23[12] = 0x0; data_23[13] = 0x0; data_23[14] = 0x0; data_23[15] = 0x0; data_23[16] = 0x0; data_23[17] = 0x0; data_23[18] = 0x0; data_23[19] = 0x48; data_23[20] = 0x89; data_23[21] = 0xc8; data_23[22] = 0x48; data_23[23] = 0x1; data_23[24] = 0xd0; data_23[25] = 0x49; data_23[26] = 0xbb; data_23[27] = 0x5; data_23[28] = 0x1; data_23[29] = 0x0; data_23[30] = 0x0; data_23[31] = 0x0; data_23[32] = 0x0; data_23[33] = 0x0; data_23[34] = 0x0; data_23[35] = 0x4c; data_23[36] = 0x39; data_23[37] = 0xd8; data_23[38] = 0xf; data_23[39] = 0x94; data_23[40] = 0xc0; data_23[41] = 0x48; data_23[42] = 0x83; data_23[43] = 0xe0; data_23[44] = 0x1; data_23[45] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_23)(&flag[23]);
    counter += res;
    memset((void*)data_23, 0, 46);

    // kusd_check;add;0x44
    volatile uint8_t data_16[53] = {};
    // clang-format off
    data_16[0] = 0x8a; data_16[1] = 0x9; data_16[2] = 0x48; data_16[3] = 0x81; data_16[4] = 0xe1; data_16[5] = 0xff; data_16[6] = 0x0; data_16[7] = 0x0; data_16[8] = 0x0; data_16[9] = 0x48; data_16[10] = 0x31; data_16[11] = 0xd2; data_16[12] = 0x8a; data_16[13] = 0x14; data_16[14] = 0x25; data_16[15] = 0xd4; data_16[16] = 0x2; data_16[17] = 0xfe; data_16[18] = 0x7f; data_16[19] = 0x83; data_16[20] = 0xc2; data_16[21] = 0x1; data_16[22] = 0x48; data_16[23] = 0xc1; data_16[24] = 0xe2; data_16[25] = 0x4; data_16[26] = 0x48; data_16[27] = 0x89; data_16[28] = 0xc8; data_16[29] = 0x48; data_16[30] = 0x1; data_16[31] = 0xd0; data_16[32] = 0x49; data_16[33] = 0xbb; data_16[34] = 0x44; data_16[35] = 0x0; data_16[36] = 0x0; data_16[37] = 0x0; data_16[38] = 0x0; data_16[39] = 0x0; data_16[40] = 0x0; data_16[41] = 0x0; data_16[42] = 0x4c; data_16[43] = 0x39; data_16[44] = 0xd8; data_16[45] = 0xf; data_16[46] = 0x94; data_16[47] = 0xc0; data_16[48] = 0x48; data_16[49] = 0x83; data_16[50] = 0xe0; data_16[51] = 0x1; data_16[52] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_16)(&flag[16]);
    counter += res;
    memset((void*)data_16, 0, 53);

    // imm64;sub;0xffffffffffffffa5
    volatile uint8_t data_3[46] = {};
    // clang-format off
    data_3[0] = 0x8a; data_3[1] = 0x9; data_3[2] = 0x48; data_3[3] = 0x81; data_3[4] = 0xe1; data_3[5] = 0xff; data_3[6] = 0x0; data_3[7] = 0x0; data_3[8] = 0x0; data_3[9] = 0x48; data_3[10] = 0xba; data_3[11] = 0xd6; data_3[12] = 0x0; data_3[13] = 0x0; data_3[14] = 0x0; data_3[15] = 0x0; data_3[16] = 0x0; data_3[17] = 0x0; data_3[18] = 0x0; data_3[19] = 0x48; data_3[20] = 0x89; data_3[21] = 0xc8; data_3[22] = 0x48; data_3[23] = 0x29; data_3[24] = 0xd0; data_3[25] = 0x49; data_3[26] = 0xbb; data_3[27] = 0xa5; data_3[28] = 0xff; data_3[29] = 0xff; data_3[30] = 0xff; data_3[31] = 0xff; data_3[32] = 0xff; data_3[33] = 0xff; data_3[34] = 0xff; data_3[35] = 0x4c; data_3[36] = 0x39; data_3[37] = 0xd8; data_3[38] = 0xf; data_3[39] = 0x94; data_3[40] = 0xc0; data_3[41] = 0x48; data_3[42] = 0x83; data_3[43] = 0xe0; data_3[44] = 0x1; data_3[45] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_3)(&flag[3]);
    counter += res;
    memset((void*)data_3, 0, 46);

    // peb_check;sub;0x4f
    volatile uint8_t data_39[59] = {};
    // clang-format off
    data_39[0] = 0x8a; data_39[1] = 0x9; data_39[2] = 0x48; data_39[3] = 0x81; data_39[4] = 0xe1; data_39[5] = 0xff; data_39[6] = 0x0; data_39[7] = 0x0; data_39[8] = 0x0; data_39[9] = 0x65; data_39[10] = 0x4c; data_39[11] = 0x8b; data_39[12] = 0x14; data_39[13] = 0x25; data_39[14] = 0x60; data_39[15] = 0x0; data_39[16] = 0x0; data_39[17] = 0x0; data_39[18] = 0x48; data_39[19] = 0x31; data_39[20] = 0xd2; data_39[21] = 0x41; data_39[22] = 0x8a; data_39[23] = 0x52; data_39[24] = 0x2; data_39[25] = 0x83; data_39[26] = 0xc2; data_39[27] = 0x1; data_39[28] = 0x48; data_39[29] = 0xc1; data_39[30] = 0xe2; data_39[31] = 0x4; data_39[32] = 0x48; data_39[33] = 0x89; data_39[34] = 0xc8; data_39[35] = 0x48; data_39[36] = 0x29; data_39[37] = 0xd0; data_39[38] = 0x49; data_39[39] = 0xbb; data_39[40] = 0x4f; data_39[41] = 0x0; data_39[42] = 0x0; data_39[43] = 0x0; data_39[44] = 0x0; data_39[45] = 0x0; data_39[46] = 0x0; data_39[47] = 0x0; data_39[48] = 0x4c; data_39[49] = 0x39; data_39[50] = 0xd8; data_39[51] = 0xf; data_39[52] = 0x94; data_39[53] = 0xc0; data_39[54] = 0x48; data_39[55] = 0x83; data_39[56] = 0xe0; data_39[57] = 0x1; data_39[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_39)(&flag[39]);
    counter += res;
    memset((void*)data_39, 0, 59);

    // imm64;xor;0x4e
    volatile uint8_t data_87[46] = {};
    // clang-format off
    data_87[0] = 0x8a; data_87[1] = 0x9; data_87[2] = 0x48; data_87[3] = 0x81; data_87[4] = 0xe1; data_87[5] = 0xff; data_87[6] = 0x0; data_87[7] = 0x0; data_87[8] = 0x0; data_87[9] = 0x48; data_87[10] = 0xba; data_87[11] = 0x3d; data_87[12] = 0x0; data_87[13] = 0x0; data_87[14] = 0x0; data_87[15] = 0x0; data_87[16] = 0x0; data_87[17] = 0x0; data_87[18] = 0x0; data_87[19] = 0x48; data_87[20] = 0x89; data_87[21] = 0xc8; data_87[22] = 0x48; data_87[23] = 0x31; data_87[24] = 0xd0; data_87[25] = 0x49; data_87[26] = 0xbb; data_87[27] = 0x4e; data_87[28] = 0x0; data_87[29] = 0x0; data_87[30] = 0x0; data_87[31] = 0x0; data_87[32] = 0x0; data_87[33] = 0x0; data_87[34] = 0x0; data_87[35] = 0x4c; data_87[36] = 0x39; data_87[37] = 0xd8; data_87[38] = 0xf; data_87[39] = 0x94; data_87[40] = 0xc0; data_87[41] = 0x48; data_87[42] = 0x83; data_87[43] = 0xe0; data_87[44] = 0x1; data_87[45] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_87)(&flag[87]);
    counter += res;
    memset((void*)data_87, 0, 46);

    // kusd_check;sub;0x49
    volatile uint8_t data_12[53] = {};
    // clang-format off
    data_12[0] = 0x8a; data_12[1] = 0x9; data_12[2] = 0x48; data_12[3] = 0x81; data_12[4] = 0xe1; data_12[5] = 0xff; data_12[6] = 0x0; data_12[7] = 0x0; data_12[8] = 0x0; data_12[9] = 0x48; data_12[10] = 0x31; data_12[11] = 0xd2; data_12[12] = 0x8a; data_12[13] = 0x14; data_12[14] = 0x25; data_12[15] = 0xd4; data_12[16] = 0x2; data_12[17] = 0xfe; data_12[18] = 0x7f; data_12[19] = 0x83; data_12[20] = 0xc2; data_12[21] = 0x1; data_12[22] = 0x48; data_12[23] = 0xc1; data_12[24] = 0xe2; data_12[25] = 0x4; data_12[26] = 0x48; data_12[27] = 0x89; data_12[28] = 0xc8; data_12[29] = 0x48; data_12[30] = 0x29; data_12[31] = 0xd0; data_12[32] = 0x49; data_12[33] = 0xbb; data_12[34] = 0x49; data_12[35] = 0x0; data_12[36] = 0x0; data_12[37] = 0x0; data_12[38] = 0x0; data_12[39] = 0x0; data_12[40] = 0x0; data_12[41] = 0x0; data_12[42] = 0x4c; data_12[43] = 0x39; data_12[44] = 0xd8; data_12[45] = 0xf; data_12[46] = 0x94; data_12[47] = 0xc0; data_12[48] = 0x48; data_12[49] = 0x83; data_12[50] = 0xe0; data_12[51] = 0x1; data_12[52] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_12)(&flag[12]);
    counter += res;
    memset((void*)data_12, 0, 53);

    // imm64;sub;0xffffffffffffffed
    volatile uint8_t data_31[46] = {};
    // clang-format off
    data_31[0] = 0x8a; data_31[1] = 0x9; data_31[2] = 0x48; data_31[3] = 0x81; data_31[4] = 0xe1; data_31[5] = 0xff; data_31[6] = 0x0; data_31[7] = 0x0; data_31[8] = 0x0; data_31[9] = 0x48; data_31[10] = 0xba; data_31[11] = 0x47; data_31[12] = 0x0; data_31[13] = 0x0; data_31[14] = 0x0; data_31[15] = 0x0; data_31[16] = 0x0; data_31[17] = 0x0; data_31[18] = 0x0; data_31[19] = 0x48; data_31[20] = 0x89; data_31[21] = 0xc8; data_31[22] = 0x48; data_31[23] = 0x29; data_31[24] = 0xd0; data_31[25] = 0x49; data_31[26] = 0xbb; data_31[27] = 0xed; data_31[28] = 0xff; data_31[29] = 0xff; data_31[30] = 0xff; data_31[31] = 0xff; data_31[32] = 0xff; data_31[33] = 0xff; data_31[34] = 0xff; data_31[35] = 0x4c; data_31[36] = 0x39; data_31[37] = 0xd8; data_31[38] = 0xf; data_31[39] = 0x94; data_31[40] = 0xc0; data_31[41] = 0x48; data_31[42] = 0x83; data_31[43] = 0xe0; data_31[44] = 0x1; data_31[45] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_31)(&flag[31]);
    counter += res;
    memset((void*)data_31, 0, 46);

    // peb_check;sub;0x11
    volatile uint8_t data_60[59] = {};
    // clang-format off
    data_60[0] = 0x8a; data_60[1] = 0x9; data_60[2] = 0x48; data_60[3] = 0x81; data_60[4] = 0xe1; data_60[5] = 0xff; data_60[6] = 0x0; data_60[7] = 0x0; data_60[8] = 0x0; data_60[9] = 0x65; data_60[10] = 0x4c; data_60[11] = 0x8b; data_60[12] = 0x14; data_60[13] = 0x25; data_60[14] = 0x60; data_60[15] = 0x0; data_60[16] = 0x0; data_60[17] = 0x0; data_60[18] = 0x48; data_60[19] = 0x31; data_60[20] = 0xd2; data_60[21] = 0x41; data_60[22] = 0x8a; data_60[23] = 0x52; data_60[24] = 0x2; data_60[25] = 0x83; data_60[26] = 0xc2; data_60[27] = 0x1; data_60[28] = 0x48; data_60[29] = 0xc1; data_60[30] = 0xe2; data_60[31] = 0x4; data_60[32] = 0x48; data_60[33] = 0x89; data_60[34] = 0xc8; data_60[35] = 0x48; data_60[36] = 0x29; data_60[37] = 0xd0; data_60[38] = 0x49; data_60[39] = 0xbb; data_60[40] = 0x11; data_60[41] = 0x0; data_60[42] = 0x0; data_60[43] = 0x0; data_60[44] = 0x0; data_60[45] = 0x0; data_60[46] = 0x0; data_60[47] = 0x0; data_60[48] = 0x4c; data_60[49] = 0x39; data_60[50] = 0xd8; data_60[51] = 0xf; data_60[52] = 0x94; data_60[53] = 0xc0; data_60[54] = 0x48; data_60[55] = 0x83; data_60[56] = 0xe0; data_60[57] = 0x1; data_60[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_60)(&flag[60]);
    counter += res;
    memset((void*)data_60, 0, 59);

    // peb_check;add;0x75
    volatile uint8_t data_97[59] = {};
    // clang-format off
    data_97[0] = 0x8a; data_97[1] = 0x9; data_97[2] = 0x48; data_97[3] = 0x81; data_97[4] = 0xe1; data_97[5] = 0xff; data_97[6] = 0x0; data_97[7] = 0x0; data_97[8] = 0x0; data_97[9] = 0x65; data_97[10] = 0x4c; data_97[11] = 0x8b; data_97[12] = 0x14; data_97[13] = 0x25; data_97[14] = 0x60; data_97[15] = 0x0; data_97[16] = 0x0; data_97[17] = 0x0; data_97[18] = 0x48; data_97[19] = 0x31; data_97[20] = 0xd2; data_97[21] = 0x41; data_97[22] = 0x8a; data_97[23] = 0x52; data_97[24] = 0x2; data_97[25] = 0x83; data_97[26] = 0xc2; data_97[27] = 0x1; data_97[28] = 0x48; data_97[29] = 0xc1; data_97[30] = 0xe2; data_97[31] = 0x4; data_97[32] = 0x48; data_97[33] = 0x89; data_97[34] = 0xc8; data_97[35] = 0x48; data_97[36] = 0x1; data_97[37] = 0xd0; data_97[38] = 0x49; data_97[39] = 0xbb; data_97[40] = 0x75; data_97[41] = 0x0; data_97[42] = 0x0; data_97[43] = 0x0; data_97[44] = 0x0; data_97[45] = 0x0; data_97[46] = 0x0; data_97[47] = 0x0; data_97[48] = 0x4c; data_97[49] = 0x39; data_97[50] = 0xd8; data_97[51] = 0xf; data_97[52] = 0x94; data_97[53] = 0xc0; data_97[54] = 0x48; data_97[55] = 0x83; data_97[56] = 0xe0; data_97[57] = 0x1; data_97[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_97)(&flag[97]);
    counter += res;
    memset((void*)data_97, 0, 59);

    // peb_check;add;0x78
    volatile uint8_t data_64[59] = {};
    // clang-format off
    data_64[0] = 0x8a; data_64[1] = 0x9; data_64[2] = 0x48; data_64[3] = 0x81; data_64[4] = 0xe1; data_64[5] = 0xff; data_64[6] = 0x0; data_64[7] = 0x0; data_64[8] = 0x0; data_64[9] = 0x65; data_64[10] = 0x4c; data_64[11] = 0x8b; data_64[12] = 0x14; data_64[13] = 0x25; data_64[14] = 0x60; data_64[15] = 0x0; data_64[16] = 0x0; data_64[17] = 0x0; data_64[18] = 0x48; data_64[19] = 0x31; data_64[20] = 0xd2; data_64[21] = 0x41; data_64[22] = 0x8a; data_64[23] = 0x52; data_64[24] = 0x2; data_64[25] = 0x83; data_64[26] = 0xc2; data_64[27] = 0x1; data_64[28] = 0x48; data_64[29] = 0xc1; data_64[30] = 0xe2; data_64[31] = 0x4; data_64[32] = 0x48; data_64[33] = 0x89; data_64[34] = 0xc8; data_64[35] = 0x48; data_64[36] = 0x1; data_64[37] = 0xd0; data_64[38] = 0x49; data_64[39] = 0xbb; data_64[40] = 0x78; data_64[41] = 0x0; data_64[42] = 0x0; data_64[43] = 0x0; data_64[44] = 0x0; data_64[45] = 0x0; data_64[46] = 0x0; data_64[47] = 0x0; data_64[48] = 0x4c; data_64[49] = 0x39; data_64[50] = 0xd8; data_64[51] = 0xf; data_64[52] = 0x94; data_64[53] = 0xc0; data_64[54] = 0x48; data_64[55] = 0x83; data_64[56] = 0xe0; data_64[57] = 0x1; data_64[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_64)(&flag[64]);
    counter += res;
    memset((void*)data_64, 0, 59);

    // kusd_check;sub;0x69
    volatile uint8_t data_83[53] = {};
    // clang-format off
    data_83[0] = 0x8a; data_83[1] = 0x9; data_83[2] = 0x48; data_83[3] = 0x81; data_83[4] = 0xe1; data_83[5] = 0xff; data_83[6] = 0x0; data_83[7] = 0x0; data_83[8] = 0x0; data_83[9] = 0x48; data_83[10] = 0x31; data_83[11] = 0xd2; data_83[12] = 0x8a; data_83[13] = 0x14; data_83[14] = 0x25; data_83[15] = 0xd4; data_83[16] = 0x2; data_83[17] = 0xfe; data_83[18] = 0x7f; data_83[19] = 0x83; data_83[20] = 0xc2; data_83[21] = 0x1; data_83[22] = 0x48; data_83[23] = 0xc1; data_83[24] = 0xe2; data_83[25] = 0x4; data_83[26] = 0x48; data_83[27] = 0x89; data_83[28] = 0xc8; data_83[29] = 0x48; data_83[30] = 0x29; data_83[31] = 0xd0; data_83[32] = 0x49; data_83[33] = 0xbb; data_83[34] = 0x69; data_83[35] = 0x0; data_83[36] = 0x0; data_83[37] = 0x0; data_83[38] = 0x0; data_83[39] = 0x0; data_83[40] = 0x0; data_83[41] = 0x0; data_83[42] = 0x4c; data_83[43] = 0x39; data_83[44] = 0xd8; data_83[45] = 0xf; data_83[46] = 0x94; data_83[47] = 0xc0; data_83[48] = 0x48; data_83[49] = 0x83; data_83[50] = 0xe0; data_83[51] = 0x1; data_83[52] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_83)(&flag[83]);
    counter += res;
    memset((void*)data_83, 0, 53);

    // peb_check;add;0x75
    volatile uint8_t data_45[59] = {};
    // clang-format off
    data_45[0] = 0x8a; data_45[1] = 0x9; data_45[2] = 0x48; data_45[3] = 0x81; data_45[4] = 0xe1; data_45[5] = 0xff; data_45[6] = 0x0; data_45[7] = 0x0; data_45[8] = 0x0; data_45[9] = 0x65; data_45[10] = 0x4c; data_45[11] = 0x8b; data_45[12] = 0x14; data_45[13] = 0x25; data_45[14] = 0x60; data_45[15] = 0x0; data_45[16] = 0x0; data_45[17] = 0x0; data_45[18] = 0x48; data_45[19] = 0x31; data_45[20] = 0xd2; data_45[21] = 0x41; data_45[22] = 0x8a; data_45[23] = 0x52; data_45[24] = 0x2; data_45[25] = 0x83; data_45[26] = 0xc2; data_45[27] = 0x1; data_45[28] = 0x48; data_45[29] = 0xc1; data_45[30] = 0xe2; data_45[31] = 0x4; data_45[32] = 0x48; data_45[33] = 0x89; data_45[34] = 0xc8; data_45[35] = 0x48; data_45[36] = 0x1; data_45[37] = 0xd0; data_45[38] = 0x49; data_45[39] = 0xbb; data_45[40] = 0x75; data_45[41] = 0x0; data_45[42] = 0x0; data_45[43] = 0x0; data_45[44] = 0x0; data_45[45] = 0x0; data_45[46] = 0x0; data_45[47] = 0x0; data_45[48] = 0x4c; data_45[49] = 0x39; data_45[50] = 0xd8; data_45[51] = 0xf; data_45[52] = 0x94; data_45[53] = 0xc0; data_45[54] = 0x48; data_45[55] = 0x83; data_45[56] = 0xe0; data_45[57] = 0x1; data_45[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_45)(&flag[45]);
    counter += res;
    memset((void*)data_45, 0, 59);

    // peb_check;sub;0x53
    volatile uint8_t data_80[59] = {};
    // clang-format off
    data_80[0] = 0x8a; data_80[1] = 0x9; data_80[2] = 0x48; data_80[3] = 0x81; data_80[4] = 0xe1; data_80[5] = 0xff; data_80[6] = 0x0; data_80[7] = 0x0; data_80[8] = 0x0; data_80[9] = 0x65; data_80[10] = 0x4c; data_80[11] = 0x8b; data_80[12] = 0x14; data_80[13] = 0x25; data_80[14] = 0x60; data_80[15] = 0x0; data_80[16] = 0x0; data_80[17] = 0x0; data_80[18] = 0x48; data_80[19] = 0x31; data_80[20] = 0xd2; data_80[21] = 0x41; data_80[22] = 0x8a; data_80[23] = 0x52; data_80[24] = 0x2; data_80[25] = 0x83; data_80[26] = 0xc2; data_80[27] = 0x1; data_80[28] = 0x48; data_80[29] = 0xc1; data_80[30] = 0xe2; data_80[31] = 0x4; data_80[32] = 0x48; data_80[33] = 0x89; data_80[34] = 0xc8; data_80[35] = 0x48; data_80[36] = 0x29; data_80[37] = 0xd0; data_80[38] = 0x49; data_80[39] = 0xbb; data_80[40] = 0x53; data_80[41] = 0x0; data_80[42] = 0x0; data_80[43] = 0x0; data_80[44] = 0x0; data_80[45] = 0x0; data_80[46] = 0x0; data_80[47] = 0x0; data_80[48] = 0x4c; data_80[49] = 0x39; data_80[50] = 0xd8; data_80[51] = 0xf; data_80[52] = 0x94; data_80[53] = 0xc0; data_80[54] = 0x48; data_80[55] = 0x83; data_80[56] = 0xe0; data_80[57] = 0x1; data_80[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_80)(&flag[80]);
    counter += res;
    memset((void*)data_80, 0, 59);

    // peb_check;add;0x8d
    volatile uint8_t data_99[59] = {};
    // clang-format off
    data_99[0] = 0x8a; data_99[1] = 0x9; data_99[2] = 0x48; data_99[3] = 0x81; data_99[4] = 0xe1; data_99[5] = 0xff; data_99[6] = 0x0; data_99[7] = 0x0; data_99[8] = 0x0; data_99[9] = 0x65; data_99[10] = 0x4c; data_99[11] = 0x8b; data_99[12] = 0x14; data_99[13] = 0x25; data_99[14] = 0x60; data_99[15] = 0x0; data_99[16] = 0x0; data_99[17] = 0x0; data_99[18] = 0x48; data_99[19] = 0x31; data_99[20] = 0xd2; data_99[21] = 0x41; data_99[22] = 0x8a; data_99[23] = 0x52; data_99[24] = 0x2; data_99[25] = 0x83; data_99[26] = 0xc2; data_99[27] = 0x1; data_99[28] = 0x48; data_99[29] = 0xc1; data_99[30] = 0xe2; data_99[31] = 0x4; data_99[32] = 0x48; data_99[33] = 0x89; data_99[34] = 0xc8; data_99[35] = 0x48; data_99[36] = 0x1; data_99[37] = 0xd0; data_99[38] = 0x49; data_99[39] = 0xbb; data_99[40] = 0x8d; data_99[41] = 0x0; data_99[42] = 0x0; data_99[43] = 0x0; data_99[44] = 0x0; data_99[45] = 0x0; data_99[46] = 0x0; data_99[47] = 0x0; data_99[48] = 0x4c; data_99[49] = 0x39; data_99[50] = 0xd8; data_99[51] = 0xf; data_99[52] = 0x94; data_99[53] = 0xc0; data_99[54] = 0x48; data_99[55] = 0x83; data_99[56] = 0xe0; data_99[57] = 0x1; data_99[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_99)(&flag[99]);
    counter += res;
    memset((void*)data_99, 0, 59);

    // peb_check;add;0x71
    volatile uint8_t data_26[59] = {};
    // clang-format off
    data_26[0] = 0x8a; data_26[1] = 0x9; data_26[2] = 0x48; data_26[3] = 0x81; data_26[4] = 0xe1; data_26[5] = 0xff; data_26[6] = 0x0; data_26[7] = 0x0; data_26[8] = 0x0; data_26[9] = 0x65; data_26[10] = 0x4c; data_26[11] = 0x8b; data_26[12] = 0x14; data_26[13] = 0x25; data_26[14] = 0x60; data_26[15] = 0x0; data_26[16] = 0x0; data_26[17] = 0x0; data_26[18] = 0x48; data_26[19] = 0x31; data_26[20] = 0xd2; data_26[21] = 0x41; data_26[22] = 0x8a; data_26[23] = 0x52; data_26[24] = 0x2; data_26[25] = 0x83; data_26[26] = 0xc2; data_26[27] = 0x1; data_26[28] = 0x48; data_26[29] = 0xc1; data_26[30] = 0xe2; data_26[31] = 0x4; data_26[32] = 0x48; data_26[33] = 0x89; data_26[34] = 0xc8; data_26[35] = 0x48; data_26[36] = 0x1; data_26[37] = 0xd0; data_26[38] = 0x49; data_26[39] = 0xbb; data_26[40] = 0x71; data_26[41] = 0x0; data_26[42] = 0x0; data_26[43] = 0x0; data_26[44] = 0x0; data_26[45] = 0x0; data_26[46] = 0x0; data_26[47] = 0x0; data_26[48] = 0x4c; data_26[49] = 0x39; data_26[50] = 0xd8; data_26[51] = 0xf; data_26[52] = 0x94; data_26[53] = 0xc0; data_26[54] = 0x48; data_26[55] = 0x83; data_26[56] = 0xe0; data_26[57] = 0x1; data_26[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_26)(&flag[26]);
    counter += res;
    memset((void*)data_26, 0, 59);

    // peb_check;add;0x7d
    volatile uint8_t data_95[59] = {};
    // clang-format off
    data_95[0] = 0x8a; data_95[1] = 0x9; data_95[2] = 0x48; data_95[3] = 0x81; data_95[4] = 0xe1; data_95[5] = 0xff; data_95[6] = 0x0; data_95[7] = 0x0; data_95[8] = 0x0; data_95[9] = 0x65; data_95[10] = 0x4c; data_95[11] = 0x8b; data_95[12] = 0x14; data_95[13] = 0x25; data_95[14] = 0x60; data_95[15] = 0x0; data_95[16] = 0x0; data_95[17] = 0x0; data_95[18] = 0x48; data_95[19] = 0x31; data_95[20] = 0xd2; data_95[21] = 0x41; data_95[22] = 0x8a; data_95[23] = 0x52; data_95[24] = 0x2; data_95[25] = 0x83; data_95[26] = 0xc2; data_95[27] = 0x1; data_95[28] = 0x48; data_95[29] = 0xc1; data_95[30] = 0xe2; data_95[31] = 0x4; data_95[32] = 0x48; data_95[33] = 0x89; data_95[34] = 0xc8; data_95[35] = 0x48; data_95[36] = 0x1; data_95[37] = 0xd0; data_95[38] = 0x49; data_95[39] = 0xbb; data_95[40] = 0x7d; data_95[41] = 0x0; data_95[42] = 0x0; data_95[43] = 0x0; data_95[44] = 0x0; data_95[45] = 0x0; data_95[46] = 0x0; data_95[47] = 0x0; data_95[48] = 0x4c; data_95[49] = 0x39; data_95[50] = 0xd8; data_95[51] = 0xf; data_95[52] = 0x94; data_95[53] = 0xc0; data_95[54] = 0x48; data_95[55] = 0x83; data_95[56] = 0xe0; data_95[57] = 0x1; data_95[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_95)(&flag[95]);
    counter += res;
    memset((void*)data_95, 0, 59);

    // kusd_check;sub;0x5f
    volatile uint8_t data_13[53] = {};
    // clang-format off
    data_13[0] = 0x8a; data_13[1] = 0x9; data_13[2] = 0x48; data_13[3] = 0x81; data_13[4] = 0xe1; data_13[5] = 0xff; data_13[6] = 0x0; data_13[7] = 0x0; data_13[8] = 0x0; data_13[9] = 0x48; data_13[10] = 0x31; data_13[11] = 0xd2; data_13[12] = 0x8a; data_13[13] = 0x14; data_13[14] = 0x25; data_13[15] = 0xd4; data_13[16] = 0x2; data_13[17] = 0xfe; data_13[18] = 0x7f; data_13[19] = 0x83; data_13[20] = 0xc2; data_13[21] = 0x1; data_13[22] = 0x48; data_13[23] = 0xc1; data_13[24] = 0xe2; data_13[25] = 0x4; data_13[26] = 0x48; data_13[27] = 0x89; data_13[28] = 0xc8; data_13[29] = 0x48; data_13[30] = 0x29; data_13[31] = 0xd0; data_13[32] = 0x49; data_13[33] = 0xbb; data_13[34] = 0x5f; data_13[35] = 0x0; data_13[36] = 0x0; data_13[37] = 0x0; data_13[38] = 0x0; data_13[39] = 0x0; data_13[40] = 0x0; data_13[41] = 0x0; data_13[42] = 0x4c; data_13[43] = 0x39; data_13[44] = 0xd8; data_13[45] = 0xf; data_13[46] = 0x94; data_13[47] = 0xc0; data_13[48] = 0x48; data_13[49] = 0x83; data_13[50] = 0xe0; data_13[51] = 0x1; data_13[52] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_13)(&flag[13]);
    counter += res;
    memset((void*)data_13, 0, 53);

    // imm64;sub;0x6f
    volatile uint8_t data_10[46] = {};
    // clang-format off
    data_10[0] = 0x8a; data_10[1] = 0x9; data_10[2] = 0x48; data_10[3] = 0x81; data_10[4] = 0xe1; data_10[5] = 0xff; data_10[6] = 0x0; data_10[7] = 0x0; data_10[8] = 0x0; data_10[9] = 0x48; data_10[10] = 0xba; data_10[11] = 0x8; data_10[12] = 0x0; data_10[13] = 0x0; data_10[14] = 0x0; data_10[15] = 0x0; data_10[16] = 0x0; data_10[17] = 0x0; data_10[18] = 0x0; data_10[19] = 0x48; data_10[20] = 0x89; data_10[21] = 0xc8; data_10[22] = 0x48; data_10[23] = 0x29; data_10[24] = 0xd0; data_10[25] = 0x49; data_10[26] = 0xbb; data_10[27] = 0x6f; data_10[28] = 0x0; data_10[29] = 0x0; data_10[30] = 0x0; data_10[31] = 0x0; data_10[32] = 0x0; data_10[33] = 0x0; data_10[34] = 0x0; data_10[35] = 0x4c; data_10[36] = 0x39; data_10[37] = 0xd8; data_10[38] = 0xf; data_10[39] = 0x94; data_10[40] = 0xc0; data_10[41] = 0x48; data_10[42] = 0x83; data_10[43] = 0xe0; data_10[44] = 0x1; data_10[45] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_10)(&flag[10]);
    counter += res;
    memset((void*)data_10, 0, 46);

    // imm64;add;0x122
    volatile uint8_t data_46[46] = {};
    // clang-format off
    data_46[0] = 0x8a; data_46[1] = 0x9; data_46[2] = 0x48; data_46[3] = 0x81; data_46[4] = 0xe1; data_46[5] = 0xff; data_46[6] = 0x0; data_46[7] = 0x0; data_46[8] = 0x0; data_46[9] = 0x48; data_46[10] = 0xba; data_46[11] = 0xb0; data_46[12] = 0x0; data_46[13] = 0x0; data_46[14] = 0x0; data_46[15] = 0x0; data_46[16] = 0x0; data_46[17] = 0x0; data_46[18] = 0x0; data_46[19] = 0x48; data_46[20] = 0x89; data_46[21] = 0xc8; data_46[22] = 0x48; data_46[23] = 0x1; data_46[24] = 0xd0; data_46[25] = 0x49; data_46[26] = 0xbb; data_46[27] = 0x22; data_46[28] = 0x1; data_46[29] = 0x0; data_46[30] = 0x0; data_46[31] = 0x0; data_46[32] = 0x0; data_46[33] = 0x0; data_46[34] = 0x0; data_46[35] = 0x4c; data_46[36] = 0x39; data_46[37] = 0xd8; data_46[38] = 0xf; data_46[39] = 0x94; data_46[40] = 0xc0; data_46[41] = 0x48; data_46[42] = 0x83; data_46[43] = 0xe0; data_46[44] = 0x1; data_46[45] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_46)(&flag[46]);
    counter += res;
    memset((void*)data_46, 0, 46);

    // imm64;add;0x6a
    volatile uint8_t data_57[46] = {};
    // clang-format off
    data_57[0] = 0x8a; data_57[1] = 0x9; data_57[2] = 0x48; data_57[3] = 0x81; data_57[4] = 0xe1; data_57[5] = 0xff; data_57[6] = 0x0; data_57[7] = 0x0; data_57[8] = 0x0; data_57[9] = 0x48; data_57[10] = 0xba; data_57[11] = 0x49; data_57[12] = 0x0; data_57[13] = 0x0; data_57[14] = 0x0; data_57[15] = 0x0; data_57[16] = 0x0; data_57[17] = 0x0; data_57[18] = 0x0; data_57[19] = 0x48; data_57[20] = 0x89; data_57[21] = 0xc8; data_57[22] = 0x48; data_57[23] = 0x1; data_57[24] = 0xd0; data_57[25] = 0x49; data_57[26] = 0xbb; data_57[27] = 0x6a; data_57[28] = 0x0; data_57[29] = 0x0; data_57[30] = 0x0; data_57[31] = 0x0; data_57[32] = 0x0; data_57[33] = 0x0; data_57[34] = 0x0; data_57[35] = 0x4c; data_57[36] = 0x39; data_57[37] = 0xd8; data_57[38] = 0xf; data_57[39] = 0x94; data_57[40] = 0xc0; data_57[41] = 0x48; data_57[42] = 0x83; data_57[43] = 0xe0; data_57[44] = 0x1; data_57[45] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_57)(&flag[57]);
    counter += res;
    memset((void*)data_57, 0, 46);

    // peb_check;add;0x47
    volatile uint8_t data_94[59] = {};
    // clang-format off
    data_94[0] = 0x8a; data_94[1] = 0x9; data_94[2] = 0x48; data_94[3] = 0x81; data_94[4] = 0xe1; data_94[5] = 0xff; data_94[6] = 0x0; data_94[7] = 0x0; data_94[8] = 0x0; data_94[9] = 0x65; data_94[10] = 0x4c; data_94[11] = 0x8b; data_94[12] = 0x14; data_94[13] = 0x25; data_94[14] = 0x60; data_94[15] = 0x0; data_94[16] = 0x0; data_94[17] = 0x0; data_94[18] = 0x48; data_94[19] = 0x31; data_94[20] = 0xd2; data_94[21] = 0x41; data_94[22] = 0x8a; data_94[23] = 0x52; data_94[24] = 0x2; data_94[25] = 0x83; data_94[26] = 0xc2; data_94[27] = 0x1; data_94[28] = 0x48; data_94[29] = 0xc1; data_94[30] = 0xe2; data_94[31] = 0x4; data_94[32] = 0x48; data_94[33] = 0x89; data_94[34] = 0xc8; data_94[35] = 0x48; data_94[36] = 0x1; data_94[37] = 0xd0; data_94[38] = 0x49; data_94[39] = 0xbb; data_94[40] = 0x47; data_94[41] = 0x0; data_94[42] = 0x0; data_94[43] = 0x0; data_94[44] = 0x0; data_94[45] = 0x0; data_94[46] = 0x0; data_94[47] = 0x0; data_94[48] = 0x4c; data_94[49] = 0x39; data_94[50] = 0xd8; data_94[51] = 0xf; data_94[52] = 0x94; data_94[53] = 0xc0; data_94[54] = 0x48; data_94[55] = 0x83; data_94[56] = 0xe0; data_94[57] = 0x1; data_94[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_94)(&flag[94]);
    counter += res;
    memset((void*)data_94, 0, 59);

    // peb_check;xor;0x79
    volatile uint8_t data_90[59] = {};
    // clang-format off
    data_90[0] = 0x8a; data_90[1] = 0x9; data_90[2] = 0x48; data_90[3] = 0x81; data_90[4] = 0xe1; data_90[5] = 0xff; data_90[6] = 0x0; data_90[7] = 0x0; data_90[8] = 0x0; data_90[9] = 0x65; data_90[10] = 0x4c; data_90[11] = 0x8b; data_90[12] = 0x14; data_90[13] = 0x25; data_90[14] = 0x60; data_90[15] = 0x0; data_90[16] = 0x0; data_90[17] = 0x0; data_90[18] = 0x48; data_90[19] = 0x31; data_90[20] = 0xd2; data_90[21] = 0x41; data_90[22] = 0x8a; data_90[23] = 0x52; data_90[24] = 0x2; data_90[25] = 0x83; data_90[26] = 0xc2; data_90[27] = 0x1; data_90[28] = 0x48; data_90[29] = 0xc1; data_90[30] = 0xe2; data_90[31] = 0x4; data_90[32] = 0x48; data_90[33] = 0x89; data_90[34] = 0xc8; data_90[35] = 0x48; data_90[36] = 0x31; data_90[37] = 0xd0; data_90[38] = 0x49; data_90[39] = 0xbb; data_90[40] = 0x79; data_90[41] = 0x0; data_90[42] = 0x0; data_90[43] = 0x0; data_90[44] = 0x0; data_90[45] = 0x0; data_90[46] = 0x0; data_90[47] = 0x0; data_90[48] = 0x4c; data_90[49] = 0x39; data_90[50] = 0xd8; data_90[51] = 0xf; data_90[52] = 0x94; data_90[53] = 0xc0; data_90[54] = 0x48; data_90[55] = 0x83; data_90[56] = 0xe0; data_90[57] = 0x1; data_90[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_90)(&flag[90]);
    counter += res;
    memset((void*)data_90, 0, 59);

    // imm64;sub;0xffffffffffffff8b
    volatile uint8_t data_76[46] = {};
    // clang-format off
    data_76[0] = 0x8a; data_76[1] = 0x9; data_76[2] = 0x48; data_76[3] = 0x81; data_76[4] = 0xe1; data_76[5] = 0xff; data_76[6] = 0x0; data_76[7] = 0x0; data_76[8] = 0x0; data_76[9] = 0x48; data_76[10] = 0xba; data_76[11] = 0xe9; data_76[12] = 0x0; data_76[13] = 0x0; data_76[14] = 0x0; data_76[15] = 0x0; data_76[16] = 0x0; data_76[17] = 0x0; data_76[18] = 0x0; data_76[19] = 0x48; data_76[20] = 0x89; data_76[21] = 0xc8; data_76[22] = 0x48; data_76[23] = 0x29; data_76[24] = 0xd0; data_76[25] = 0x49; data_76[26] = 0xbb; data_76[27] = 0x8b; data_76[28] = 0xff; data_76[29] = 0xff; data_76[30] = 0xff; data_76[31] = 0xff; data_76[32] = 0xff; data_76[33] = 0xff; data_76[34] = 0xff; data_76[35] = 0x4c; data_76[36] = 0x39; data_76[37] = 0xd8; data_76[38] = 0xf; data_76[39] = 0x94; data_76[40] = 0xc0; data_76[41] = 0x48; data_76[42] = 0x83; data_76[43] = 0xe0; data_76[44] = 0x1; data_76[45] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_76)(&flag[76]);
    counter += res;
    memset((void*)data_76, 0, 46);

    // kusd_check;xor;0x79
    volatile uint8_t data_79[53] = {};
    // clang-format off
    data_79[0] = 0x8a; data_79[1] = 0x9; data_79[2] = 0x48; data_79[3] = 0x81; data_79[4] = 0xe1; data_79[5] = 0xff; data_79[6] = 0x0; data_79[7] = 0x0; data_79[8] = 0x0; data_79[9] = 0x48; data_79[10] = 0x31; data_79[11] = 0xd2; data_79[12] = 0x8a; data_79[13] = 0x14; data_79[14] = 0x25; data_79[15] = 0xd4; data_79[16] = 0x2; data_79[17] = 0xfe; data_79[18] = 0x7f; data_79[19] = 0x83; data_79[20] = 0xc2; data_79[21] = 0x1; data_79[22] = 0x48; data_79[23] = 0xc1; data_79[24] = 0xe2; data_79[25] = 0x4; data_79[26] = 0x48; data_79[27] = 0x89; data_79[28] = 0xc8; data_79[29] = 0x48; data_79[30] = 0x31; data_79[31] = 0xd0; data_79[32] = 0x49; data_79[33] = 0xbb; data_79[34] = 0x79; data_79[35] = 0x0; data_79[36] = 0x0; data_79[37] = 0x0; data_79[38] = 0x0; data_79[39] = 0x0; data_79[40] = 0x0; data_79[41] = 0x0; data_79[42] = 0x4c; data_79[43] = 0x39; data_79[44] = 0xd8; data_79[45] = 0xf; data_79[46] = 0x94; data_79[47] = 0xc0; data_79[48] = 0x48; data_79[49] = 0x83; data_79[50] = 0xe0; data_79[51] = 0x1; data_79[52] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_79)(&flag[79]);
    counter += res;
    memset((void*)data_79, 0, 53);

    // kusd_check;sub;0x5a
    volatile uint8_t data_89[53] = {};
    // clang-format off
    data_89[0] = 0x8a; data_89[1] = 0x9; data_89[2] = 0x48; data_89[3] = 0x81; data_89[4] = 0xe1; data_89[5] = 0xff; data_89[6] = 0x0; data_89[7] = 0x0; data_89[8] = 0x0; data_89[9] = 0x48; data_89[10] = 0x31; data_89[11] = 0xd2; data_89[12] = 0x8a; data_89[13] = 0x14; data_89[14] = 0x25; data_89[15] = 0xd4; data_89[16] = 0x2; data_89[17] = 0xfe; data_89[18] = 0x7f; data_89[19] = 0x83; data_89[20] = 0xc2; data_89[21] = 0x1; data_89[22] = 0x48; data_89[23] = 0xc1; data_89[24] = 0xe2; data_89[25] = 0x4; data_89[26] = 0x48; data_89[27] = 0x89; data_89[28] = 0xc8; data_89[29] = 0x48; data_89[30] = 0x29; data_89[31] = 0xd0; data_89[32] = 0x49; data_89[33] = 0xbb; data_89[34] = 0x5a; data_89[35] = 0x0; data_89[36] = 0x0; data_89[37] = 0x0; data_89[38] = 0x0; data_89[39] = 0x0; data_89[40] = 0x0; data_89[41] = 0x0; data_89[42] = 0x4c; data_89[43] = 0x39; data_89[44] = 0xd8; data_89[45] = 0xf; data_89[46] = 0x94; data_89[47] = 0xc0; data_89[48] = 0x48; data_89[49] = 0x83; data_89[50] = 0xe0; data_89[51] = 0x1; data_89[52] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_89)(&flag[89]);
    counter += res;
    memset((void*)data_89, 0, 53);

    // peb_check;add;0x31
    volatile uint8_t data_33[59] = {};
    // clang-format off
    data_33[0] = 0x8a; data_33[1] = 0x9; data_33[2] = 0x48; data_33[3] = 0x81; data_33[4] = 0xe1; data_33[5] = 0xff; data_33[6] = 0x0; data_33[7] = 0x0; data_33[8] = 0x0; data_33[9] = 0x65; data_33[10] = 0x4c; data_33[11] = 0x8b; data_33[12] = 0x14; data_33[13] = 0x25; data_33[14] = 0x60; data_33[15] = 0x0; data_33[16] = 0x0; data_33[17] = 0x0; data_33[18] = 0x48; data_33[19] = 0x31; data_33[20] = 0xd2; data_33[21] = 0x41; data_33[22] = 0x8a; data_33[23] = 0x52; data_33[24] = 0x2; data_33[25] = 0x83; data_33[26] = 0xc2; data_33[27] = 0x1; data_33[28] = 0x48; data_33[29] = 0xc1; data_33[30] = 0xe2; data_33[31] = 0x4; data_33[32] = 0x48; data_33[33] = 0x89; data_33[34] = 0xc8; data_33[35] = 0x48; data_33[36] = 0x1; data_33[37] = 0xd0; data_33[38] = 0x49; data_33[39] = 0xbb; data_33[40] = 0x31; data_33[41] = 0x0; data_33[42] = 0x0; data_33[43] = 0x0; data_33[44] = 0x0; data_33[45] = 0x0; data_33[46] = 0x0; data_33[47] = 0x0; data_33[48] = 0x4c; data_33[49] = 0x39; data_33[50] = 0xd8; data_33[51] = 0xf; data_33[52] = 0x94; data_33[53] = 0xc0; data_33[54] = 0x48; data_33[55] = 0x83; data_33[56] = 0xe0; data_33[57] = 0x1; data_33[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_33)(&flag[33]);
    counter += res;
    memset((void*)data_33, 0, 59);

    // kusd_check;sub;0x54
    volatile uint8_t data_56[53] = {};
    // clang-format off
    data_56[0] = 0x8a; data_56[1] = 0x9; data_56[2] = 0x48; data_56[3] = 0x81; data_56[4] = 0xe1; data_56[5] = 0xff; data_56[6] = 0x0; data_56[7] = 0x0; data_56[8] = 0x0; data_56[9] = 0x48; data_56[10] = 0x31; data_56[11] = 0xd2; data_56[12] = 0x8a; data_56[13] = 0x14; data_56[14] = 0x25; data_56[15] = 0xd4; data_56[16] = 0x2; data_56[17] = 0xfe; data_56[18] = 0x7f; data_56[19] = 0x83; data_56[20] = 0xc2; data_56[21] = 0x1; data_56[22] = 0x48; data_56[23] = 0xc1; data_56[24] = 0xe2; data_56[25] = 0x4; data_56[26] = 0x48; data_56[27] = 0x89; data_56[28] = 0xc8; data_56[29] = 0x48; data_56[30] = 0x29; data_56[31] = 0xd0; data_56[32] = 0x49; data_56[33] = 0xbb; data_56[34] = 0x54; data_56[35] = 0x0; data_56[36] = 0x0; data_56[37] = 0x0; data_56[38] = 0x0; data_56[39] = 0x0; data_56[40] = 0x0; data_56[41] = 0x0; data_56[42] = 0x4c; data_56[43] = 0x39; data_56[44] = 0xd8; data_56[45] = 0xf; data_56[46] = 0x94; data_56[47] = 0xc0; data_56[48] = 0x48; data_56[49] = 0x83; data_56[50] = 0xe0; data_56[51] = 0x1; data_56[52] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_56)(&flag[56]);
    counter += res;
    memset((void*)data_56, 0, 53);

    // kusd_check;xor;0x6a
    volatile uint8_t data_81[53] = {};
    // clang-format off
    data_81[0] = 0x8a; data_81[1] = 0x9; data_81[2] = 0x48; data_81[3] = 0x81; data_81[4] = 0xe1; data_81[5] = 0xff; data_81[6] = 0x0; data_81[7] = 0x0; data_81[8] = 0x0; data_81[9] = 0x48; data_81[10] = 0x31; data_81[11] = 0xd2; data_81[12] = 0x8a; data_81[13] = 0x14; data_81[14] = 0x25; data_81[15] = 0xd4; data_81[16] = 0x2; data_81[17] = 0xfe; data_81[18] = 0x7f; data_81[19] = 0x83; data_81[20] = 0xc2; data_81[21] = 0x1; data_81[22] = 0x48; data_81[23] = 0xc1; data_81[24] = 0xe2; data_81[25] = 0x4; data_81[26] = 0x48; data_81[27] = 0x89; data_81[28] = 0xc8; data_81[29] = 0x48; data_81[30] = 0x31; data_81[31] = 0xd0; data_81[32] = 0x49; data_81[33] = 0xbb; data_81[34] = 0x6a; data_81[35] = 0x0; data_81[36] = 0x0; data_81[37] = 0x0; data_81[38] = 0x0; data_81[39] = 0x0; data_81[40] = 0x0; data_81[41] = 0x0; data_81[42] = 0x4c; data_81[43] = 0x39; data_81[44] = 0xd8; data_81[45] = 0xf; data_81[46] = 0x94; data_81[47] = 0xc0; data_81[48] = 0x48; data_81[49] = 0x83; data_81[50] = 0xe0; data_81[51] = 0x1; data_81[52] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_81)(&flag[81]);
    counter += res;
    memset((void*)data_81, 0, 53);

    // peb_check;add;0x40
    volatile uint8_t data_36[59] = {};
    // clang-format off
    data_36[0] = 0x8a; data_36[1] = 0x9; data_36[2] = 0x48; data_36[3] = 0x81; data_36[4] = 0xe1; data_36[5] = 0xff; data_36[6] = 0x0; data_36[7] = 0x0; data_36[8] = 0x0; data_36[9] = 0x65; data_36[10] = 0x4c; data_36[11] = 0x8b; data_36[12] = 0x14; data_36[13] = 0x25; data_36[14] = 0x60; data_36[15] = 0x0; data_36[16] = 0x0; data_36[17] = 0x0; data_36[18] = 0x48; data_36[19] = 0x31; data_36[20] = 0xd2; data_36[21] = 0x41; data_36[22] = 0x8a; data_36[23] = 0x52; data_36[24] = 0x2; data_36[25] = 0x83; data_36[26] = 0xc2; data_36[27] = 0x1; data_36[28] = 0x48; data_36[29] = 0xc1; data_36[30] = 0xe2; data_36[31] = 0x4; data_36[32] = 0x48; data_36[33] = 0x89; data_36[34] = 0xc8; data_36[35] = 0x48; data_36[36] = 0x1; data_36[37] = 0xd0; data_36[38] = 0x49; data_36[39] = 0xbb; data_36[40] = 0x40; data_36[41] = 0x0; data_36[42] = 0x0; data_36[43] = 0x0; data_36[44] = 0x0; data_36[45] = 0x0; data_36[46] = 0x0; data_36[47] = 0x0; data_36[48] = 0x4c; data_36[49] = 0x39; data_36[50] = 0xd8; data_36[51] = 0xf; data_36[52] = 0x94; data_36[53] = 0xc0; data_36[54] = 0x48; data_36[55] = 0x83; data_36[56] = 0xe0; data_36[57] = 0x1; data_36[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_36)(&flag[36]);
    counter += res;
    memset((void*)data_36, 0, 59);

    // imm64;sub;0xfffffffffffffff9
    volatile uint8_t data_74[46] = {};
    // clang-format off
    data_74[0] = 0x8a; data_74[1] = 0x9; data_74[2] = 0x48; data_74[3] = 0x81; data_74[4] = 0xe1; data_74[5] = 0xff; data_74[6] = 0x0; data_74[7] = 0x0; data_74[8] = 0x0; data_74[9] = 0x48; data_74[10] = 0xba; data_74[11] = 0x6c; data_74[12] = 0x0; data_74[13] = 0x0; data_74[14] = 0x0; data_74[15] = 0x0; data_74[16] = 0x0; data_74[17] = 0x0; data_74[18] = 0x0; data_74[19] = 0x48; data_74[20] = 0x89; data_74[21] = 0xc8; data_74[22] = 0x48; data_74[23] = 0x29; data_74[24] = 0xd0; data_74[25] = 0x49; data_74[26] = 0xbb; data_74[27] = 0xf9; data_74[28] = 0xff; data_74[29] = 0xff; data_74[30] = 0xff; data_74[31] = 0xff; data_74[32] = 0xff; data_74[33] = 0xff; data_74[34] = 0xff; data_74[35] = 0x4c; data_74[36] = 0x39; data_74[37] = 0xd8; data_74[38] = 0xf; data_74[39] = 0x94; data_74[40] = 0xc0; data_74[41] = 0x48; data_74[42] = 0x83; data_74[43] = 0xe0; data_74[44] = 0x1; data_74[45] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_74)(&flag[74]);
    counter += res;
    memset((void*)data_74, 0, 46);

    // kusd_check;xor;0x74
    volatile uint8_t data_29[53] = {};
    // clang-format off
    data_29[0] = 0x8a; data_29[1] = 0x9; data_29[2] = 0x48; data_29[3] = 0x81; data_29[4] = 0xe1; data_29[5] = 0xff; data_29[6] = 0x0; data_29[7] = 0x0; data_29[8] = 0x0; data_29[9] = 0x48; data_29[10] = 0x31; data_29[11] = 0xd2; data_29[12] = 0x8a; data_29[13] = 0x14; data_29[14] = 0x25; data_29[15] = 0xd4; data_29[16] = 0x2; data_29[17] = 0xfe; data_29[18] = 0x7f; data_29[19] = 0x83; data_29[20] = 0xc2; data_29[21] = 0x1; data_29[22] = 0x48; data_29[23] = 0xc1; data_29[24] = 0xe2; data_29[25] = 0x4; data_29[26] = 0x48; data_29[27] = 0x89; data_29[28] = 0xc8; data_29[29] = 0x48; data_29[30] = 0x31; data_29[31] = 0xd0; data_29[32] = 0x49; data_29[33] = 0xbb; data_29[34] = 0x74; data_29[35] = 0x0; data_29[36] = 0x0; data_29[37] = 0x0; data_29[38] = 0x0; data_29[39] = 0x0; data_29[40] = 0x0; data_29[41] = 0x0; data_29[42] = 0x4c; data_29[43] = 0x39; data_29[44] = 0xd8; data_29[45] = 0xf; data_29[46] = 0x94; data_29[47] = 0xc0; data_29[48] = 0x48; data_29[49] = 0x83; data_29[50] = 0xe0; data_29[51] = 0x1; data_29[52] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_29)(&flag[29]);
    counter += res;
    memset((void*)data_29, 0, 53);

    // imm64;add;0x7a
    volatile uint8_t data_61[46] = {};
    // clang-format off
    data_61[0] = 0x8a; data_61[1] = 0x9; data_61[2] = 0x48; data_61[3] = 0x81; data_61[4] = 0xe1; data_61[5] = 0xff; data_61[6] = 0x0; data_61[7] = 0x0; data_61[8] = 0x0; data_61[9] = 0x48; data_61[10] = 0xba; data_61[11] = 0x59; data_61[12] = 0x0; data_61[13] = 0x0; data_61[14] = 0x0; data_61[15] = 0x0; data_61[16] = 0x0; data_61[17] = 0x0; data_61[18] = 0x0; data_61[19] = 0x48; data_61[20] = 0x89; data_61[21] = 0xc8; data_61[22] = 0x48; data_61[23] = 0x1; data_61[24] = 0xd0; data_61[25] = 0x49; data_61[26] = 0xbb; data_61[27] = 0x7a; data_61[28] = 0x0; data_61[29] = 0x0; data_61[30] = 0x0; data_61[31] = 0x0; data_61[32] = 0x0; data_61[33] = 0x0; data_61[34] = 0x0; data_61[35] = 0x4c; data_61[36] = 0x39; data_61[37] = 0xd8; data_61[38] = 0xf; data_61[39] = 0x94; data_61[40] = 0xc0; data_61[41] = 0x48; data_61[42] = 0x83; data_61[43] = 0xe0; data_61[44] = 0x1; data_61[45] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_61)(&flag[61]);
    counter += res;
    memset((void*)data_61, 0, 46);

    // peb_check;add;0x7c
    volatile uint8_t data_27[59] = {};
    // clang-format off
    data_27[0] = 0x8a; data_27[1] = 0x9; data_27[2] = 0x48; data_27[3] = 0x81; data_27[4] = 0xe1; data_27[5] = 0xff; data_27[6] = 0x0; data_27[7] = 0x0; data_27[8] = 0x0; data_27[9] = 0x65; data_27[10] = 0x4c; data_27[11] = 0x8b; data_27[12] = 0x14; data_27[13] = 0x25; data_27[14] = 0x60; data_27[15] = 0x0; data_27[16] = 0x0; data_27[17] = 0x0; data_27[18] = 0x48; data_27[19] = 0x31; data_27[20] = 0xd2; data_27[21] = 0x41; data_27[22] = 0x8a; data_27[23] = 0x52; data_27[24] = 0x2; data_27[25] = 0x83; data_27[26] = 0xc2; data_27[27] = 0x1; data_27[28] = 0x48; data_27[29] = 0xc1; data_27[30] = 0xe2; data_27[31] = 0x4; data_27[32] = 0x48; data_27[33] = 0x89; data_27[34] = 0xc8; data_27[35] = 0x48; data_27[36] = 0x1; data_27[37] = 0xd0; data_27[38] = 0x49; data_27[39] = 0xbb; data_27[40] = 0x7c; data_27[41] = 0x0; data_27[42] = 0x0; data_27[43] = 0x0; data_27[44] = 0x0; data_27[45] = 0x0; data_27[46] = 0x0; data_27[47] = 0x0; data_27[48] = 0x4c; data_27[49] = 0x39; data_27[50] = 0xd8; data_27[51] = 0xf; data_27[52] = 0x94; data_27[53] = 0xc0; data_27[54] = 0x48; data_27[55] = 0x83; data_27[56] = 0xe0; data_27[57] = 0x1; data_27[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_27)(&flag[27]);
    counter += res;
    memset((void*)data_27, 0, 59);

    // imm64;add;0x83
    volatile uint8_t data_54[46] = {};
    // clang-format off
    data_54[0] = 0x8a; data_54[1] = 0x9; data_54[2] = 0x48; data_54[3] = 0x81; data_54[4] = 0xe1; data_54[5] = 0xff; data_54[6] = 0x0; data_54[7] = 0x0; data_54[8] = 0x0; data_54[9] = 0x48; data_54[10] = 0xba; data_54[11] = 0x11; data_54[12] = 0x0; data_54[13] = 0x0; data_54[14] = 0x0; data_54[15] = 0x0; data_54[16] = 0x0; data_54[17] = 0x0; data_54[18] = 0x0; data_54[19] = 0x48; data_54[20] = 0x89; data_54[21] = 0xc8; data_54[22] = 0x48; data_54[23] = 0x1; data_54[24] = 0xd0; data_54[25] = 0x49; data_54[26] = 0xbb; data_54[27] = 0x83; data_54[28] = 0x0; data_54[29] = 0x0; data_54[30] = 0x0; data_54[31] = 0x0; data_54[32] = 0x0; data_54[33] = 0x0; data_54[34] = 0x0; data_54[35] = 0x4c; data_54[36] = 0x39; data_54[37] = 0xd8; data_54[38] = 0xf; data_54[39] = 0x94; data_54[40] = 0xc0; data_54[41] = 0x48; data_54[42] = 0x83; data_54[43] = 0xe0; data_54[44] = 0x1; data_54[45] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_54)(&flag[54]);
    counter += res;
    memset((void*)data_54, 0, 46);

    // peb_check;xor;0x75
    volatile uint8_t data_47[59] = {};
    // clang-format off
    data_47[0] = 0x8a; data_47[1] = 0x9; data_47[2] = 0x48; data_47[3] = 0x81; data_47[4] = 0xe1; data_47[5] = 0xff; data_47[6] = 0x0; data_47[7] = 0x0; data_47[8] = 0x0; data_47[9] = 0x65; data_47[10] = 0x4c; data_47[11] = 0x8b; data_47[12] = 0x14; data_47[13] = 0x25; data_47[14] = 0x60; data_47[15] = 0x0; data_47[16] = 0x0; data_47[17] = 0x0; data_47[18] = 0x48; data_47[19] = 0x31; data_47[20] = 0xd2; data_47[21] = 0x41; data_47[22] = 0x8a; data_47[23] = 0x52; data_47[24] = 0x2; data_47[25] = 0x83; data_47[26] = 0xc2; data_47[27] = 0x1; data_47[28] = 0x48; data_47[29] = 0xc1; data_47[30] = 0xe2; data_47[31] = 0x4; data_47[32] = 0x48; data_47[33] = 0x89; data_47[34] = 0xc8; data_47[35] = 0x48; data_47[36] = 0x31; data_47[37] = 0xd0; data_47[38] = 0x49; data_47[39] = 0xbb; data_47[40] = 0x75; data_47[41] = 0x0; data_47[42] = 0x0; data_47[43] = 0x0; data_47[44] = 0x0; data_47[45] = 0x0; data_47[46] = 0x0; data_47[47] = 0x0; data_47[48] = 0x4c; data_47[49] = 0x39; data_47[50] = 0xd8; data_47[51] = 0xf; data_47[52] = 0x94; data_47[53] = 0xc0; data_47[54] = 0x48; data_47[55] = 0x83; data_47[56] = 0xe0; data_47[57] = 0x1; data_47[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_47)(&flag[47]);
    counter += res;
    memset((void*)data_47, 0, 59);

    // imm64;xor;0xff
    volatile uint8_t data_37[46] = {};
    // clang-format off
    data_37[0] = 0x8a; data_37[1] = 0x9; data_37[2] = 0x48; data_37[3] = 0x81; data_37[4] = 0xe1; data_37[5] = 0xff; data_37[6] = 0x0; data_37[7] = 0x0; data_37[8] = 0x0; data_37[9] = 0x48; data_37[10] = 0xba; data_37[11] = 0x8f; data_37[12] = 0x0; data_37[13] = 0x0; data_37[14] = 0x0; data_37[15] = 0x0; data_37[16] = 0x0; data_37[17] = 0x0; data_37[18] = 0x0; data_37[19] = 0x48; data_37[20] = 0x89; data_37[21] = 0xc8; data_37[22] = 0x48; data_37[23] = 0x31; data_37[24] = 0xd0; data_37[25] = 0x49; data_37[26] = 0xbb; data_37[27] = 0xff; data_37[28] = 0x0; data_37[29] = 0x0; data_37[30] = 0x0; data_37[31] = 0x0; data_37[32] = 0x0; data_37[33] = 0x0; data_37[34] = 0x0; data_37[35] = 0x4c; data_37[36] = 0x39; data_37[37] = 0xd8; data_37[38] = 0xf; data_37[39] = 0x94; data_37[40] = 0xc0; data_37[41] = 0x48; data_37[42] = 0x83; data_37[43] = 0xe0; data_37[44] = 0x1; data_37[45] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_37)(&flag[37]);
    counter += res;
    memset((void*)data_37, 0, 46);

    // peb_check;xor;0x62
    volatile uint8_t data_1[59] = {};
    // clang-format off
    data_1[0] = 0x8a; data_1[1] = 0x9; data_1[2] = 0x48; data_1[3] = 0x81; data_1[4] = 0xe1; data_1[5] = 0xff; data_1[6] = 0x0; data_1[7] = 0x0; data_1[8] = 0x0; data_1[9] = 0x65; data_1[10] = 0x4c; data_1[11] = 0x8b; data_1[12] = 0x14; data_1[13] = 0x25; data_1[14] = 0x60; data_1[15] = 0x0; data_1[16] = 0x0; data_1[17] = 0x0; data_1[18] = 0x48; data_1[19] = 0x31; data_1[20] = 0xd2; data_1[21] = 0x41; data_1[22] = 0x8a; data_1[23] = 0x52; data_1[24] = 0x2; data_1[25] = 0x83; data_1[26] = 0xc2; data_1[27] = 0x1; data_1[28] = 0x48; data_1[29] = 0xc1; data_1[30] = 0xe2; data_1[31] = 0x4; data_1[32] = 0x48; data_1[33] = 0x89; data_1[34] = 0xc8; data_1[35] = 0x48; data_1[36] = 0x31; data_1[37] = 0xd0; data_1[38] = 0x49; data_1[39] = 0xbb; data_1[40] = 0x62; data_1[41] = 0x0; data_1[42] = 0x0; data_1[43] = 0x0; data_1[44] = 0x0; data_1[45] = 0x0; data_1[46] = 0x0; data_1[47] = 0x0; data_1[48] = 0x4c; data_1[49] = 0x39; data_1[50] = 0xd8; data_1[51] = 0xf; data_1[52] = 0x94; data_1[53] = 0xc0; data_1[54] = 0x48; data_1[55] = 0x83; data_1[56] = 0xe0; data_1[57] = 0x1; data_1[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_1)(&flag[1]);
    counter += res;
    memset((void*)data_1, 0, 59);

    // kusd_check;sub;0x4f
    volatile uint8_t data_43[53] = {};
    // clang-format off
    data_43[0] = 0x8a; data_43[1] = 0x9; data_43[2] = 0x48; data_43[3] = 0x81; data_43[4] = 0xe1; data_43[5] = 0xff; data_43[6] = 0x0; data_43[7] = 0x0; data_43[8] = 0x0; data_43[9] = 0x48; data_43[10] = 0x31; data_43[11] = 0xd2; data_43[12] = 0x8a; data_43[13] = 0x14; data_43[14] = 0x25; data_43[15] = 0xd4; data_43[16] = 0x2; data_43[17] = 0xfe; data_43[18] = 0x7f; data_43[19] = 0x83; data_43[20] = 0xc2; data_43[21] = 0x1; data_43[22] = 0x48; data_43[23] = 0xc1; data_43[24] = 0xe2; data_43[25] = 0x4; data_43[26] = 0x48; data_43[27] = 0x89; data_43[28] = 0xc8; data_43[29] = 0x48; data_43[30] = 0x29; data_43[31] = 0xd0; data_43[32] = 0x49; data_43[33] = 0xbb; data_43[34] = 0x4f; data_43[35] = 0x0; data_43[36] = 0x0; data_43[37] = 0x0; data_43[38] = 0x0; data_43[39] = 0x0; data_43[40] = 0x0; data_43[41] = 0x0; data_43[42] = 0x4c; data_43[43] = 0x39; data_43[44] = 0xd8; data_43[45] = 0xf; data_43[46] = 0x94; data_43[47] = 0xc0; data_43[48] = 0x48; data_43[49] = 0x83; data_43[50] = 0xe0; data_43[51] = 0x1; data_43[52] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_43)(&flag[43]);
    counter += res;
    memset((void*)data_43, 0, 53);

    // kusd_check;xor;0x23
    volatile uint8_t data_2[53] = {};
    // clang-format off
    data_2[0] = 0x8a; data_2[1] = 0x9; data_2[2] = 0x48; data_2[3] = 0x81; data_2[4] = 0xe1; data_2[5] = 0xff; data_2[6] = 0x0; data_2[7] = 0x0; data_2[8] = 0x0; data_2[9] = 0x48; data_2[10] = 0x31; data_2[11] = 0xd2; data_2[12] = 0x8a; data_2[13] = 0x14; data_2[14] = 0x25; data_2[15] = 0xd4; data_2[16] = 0x2; data_2[17] = 0xfe; data_2[18] = 0x7f; data_2[19] = 0x83; data_2[20] = 0xc2; data_2[21] = 0x1; data_2[22] = 0x48; data_2[23] = 0xc1; data_2[24] = 0xe2; data_2[25] = 0x4; data_2[26] = 0x48; data_2[27] = 0x89; data_2[28] = 0xc8; data_2[29] = 0x48; data_2[30] = 0x31; data_2[31] = 0xd0; data_2[32] = 0x49; data_2[33] = 0xbb; data_2[34] = 0x23; data_2[35] = 0x0; data_2[36] = 0x0; data_2[37] = 0x0; data_2[38] = 0x0; data_2[39] = 0x0; data_2[40] = 0x0; data_2[41] = 0x0; data_2[42] = 0x4c; data_2[43] = 0x39; data_2[44] = 0xd8; data_2[45] = 0xf; data_2[46] = 0x94; data_2[47] = 0xc0; data_2[48] = 0x48; data_2[49] = 0x83; data_2[50] = 0xe0; data_2[51] = 0x1; data_2[52] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_2)(&flag[2]);
    counter += res;
    memset((void*)data_2, 0, 53);

    // peb_check;add;0x83
    volatile uint8_t data_51[59] = {};
    // clang-format off
    data_51[0] = 0x8a; data_51[1] = 0x9; data_51[2] = 0x48; data_51[3] = 0x81; data_51[4] = 0xe1; data_51[5] = 0xff; data_51[6] = 0x0; data_51[7] = 0x0; data_51[8] = 0x0; data_51[9] = 0x65; data_51[10] = 0x4c; data_51[11] = 0x8b; data_51[12] = 0x14; data_51[13] = 0x25; data_51[14] = 0x60; data_51[15] = 0x0; data_51[16] = 0x0; data_51[17] = 0x0; data_51[18] = 0x48; data_51[19] = 0x31; data_51[20] = 0xd2; data_51[21] = 0x41; data_51[22] = 0x8a; data_51[23] = 0x52; data_51[24] = 0x2; data_51[25] = 0x83; data_51[26] = 0xc2; data_51[27] = 0x1; data_51[28] = 0x48; data_51[29] = 0xc1; data_51[30] = 0xe2; data_51[31] = 0x4; data_51[32] = 0x48; data_51[33] = 0x89; data_51[34] = 0xc8; data_51[35] = 0x48; data_51[36] = 0x1; data_51[37] = 0xd0; data_51[38] = 0x49; data_51[39] = 0xbb; data_51[40] = 0x83; data_51[41] = 0x0; data_51[42] = 0x0; data_51[43] = 0x0; data_51[44] = 0x0; data_51[45] = 0x0; data_51[46] = 0x0; data_51[47] = 0x0; data_51[48] = 0x4c; data_51[49] = 0x39; data_51[50] = 0xd8; data_51[51] = 0xf; data_51[52] = 0x94; data_51[53] = 0xc0; data_51[54] = 0x48; data_51[55] = 0x83; data_51[56] = 0xe0; data_51[57] = 0x1; data_51[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_51)(&flag[51]);
    counter += res;
    memset((void*)data_51, 0, 59);

    // kusd_check;sub;0x3c
    volatile uint8_t data_32[53] = {};
    // clang-format off
    data_32[0] = 0x8a; data_32[1] = 0x9; data_32[2] = 0x48; data_32[3] = 0x81; data_32[4] = 0xe1; data_32[5] = 0xff; data_32[6] = 0x0; data_32[7] = 0x0; data_32[8] = 0x0; data_32[9] = 0x48; data_32[10] = 0x31; data_32[11] = 0xd2; data_32[12] = 0x8a; data_32[13] = 0x14; data_32[14] = 0x25; data_32[15] = 0xd4; data_32[16] = 0x2; data_32[17] = 0xfe; data_32[18] = 0x7f; data_32[19] = 0x83; data_32[20] = 0xc2; data_32[21] = 0x1; data_32[22] = 0x48; data_32[23] = 0xc1; data_32[24] = 0xe2; data_32[25] = 0x4; data_32[26] = 0x48; data_32[27] = 0x89; data_32[28] = 0xc8; data_32[29] = 0x48; data_32[30] = 0x29; data_32[31] = 0xd0; data_32[32] = 0x49; data_32[33] = 0xbb; data_32[34] = 0x3c; data_32[35] = 0x0; data_32[36] = 0x0; data_32[37] = 0x0; data_32[38] = 0x0; data_32[39] = 0x0; data_32[40] = 0x0; data_32[41] = 0x0; data_32[42] = 0x4c; data_32[43] = 0x39; data_32[44] = 0xd8; data_32[45] = 0xf; data_32[46] = 0x94; data_32[47] = 0xc0; data_32[48] = 0x48; data_32[49] = 0x83; data_32[50] = 0xe0; data_32[51] = 0x1; data_32[52] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_32)(&flag[32]);
    counter += res;
    memset((void*)data_32, 0, 53);

    // peb_check;sub;0x5e
    volatile uint8_t data_48[59] = {};
    // clang-format off
    data_48[0] = 0x8a; data_48[1] = 0x9; data_48[2] = 0x48; data_48[3] = 0x81; data_48[4] = 0xe1; data_48[5] = 0xff; data_48[6] = 0x0; data_48[7] = 0x0; data_48[8] = 0x0; data_48[9] = 0x65; data_48[10] = 0x4c; data_48[11] = 0x8b; data_48[12] = 0x14; data_48[13] = 0x25; data_48[14] = 0x60; data_48[15] = 0x0; data_48[16] = 0x0; data_48[17] = 0x0; data_48[18] = 0x48; data_48[19] = 0x31; data_48[20] = 0xd2; data_48[21] = 0x41; data_48[22] = 0x8a; data_48[23] = 0x52; data_48[24] = 0x2; data_48[25] = 0x83; data_48[26] = 0xc2; data_48[27] = 0x1; data_48[28] = 0x48; data_48[29] = 0xc1; data_48[30] = 0xe2; data_48[31] = 0x4; data_48[32] = 0x48; data_48[33] = 0x89; data_48[34] = 0xc8; data_48[35] = 0x48; data_48[36] = 0x29; data_48[37] = 0xd0; data_48[38] = 0x49; data_48[39] = 0xbb; data_48[40] = 0x5e; data_48[41] = 0x0; data_48[42] = 0x0; data_48[43] = 0x0; data_48[44] = 0x0; data_48[45] = 0x0; data_48[46] = 0x0; data_48[47] = 0x0; data_48[48] = 0x4c; data_48[49] = 0x39; data_48[50] = 0xd8; data_48[51] = 0xf; data_48[52] = 0x94; data_48[53] = 0xc0; data_48[54] = 0x48; data_48[55] = 0x83; data_48[56] = 0xe0; data_48[57] = 0x1; data_48[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_48)(&flag[48]);
    counter += res;
    memset((void*)data_48, 0, 59);

    // imm64;add;0x119
    volatile uint8_t data_59[46] = {};
    // clang-format off
    data_59[0] = 0x8a; data_59[1] = 0x9; data_59[2] = 0x48; data_59[3] = 0x81; data_59[4] = 0xe1; data_59[5] = 0xff; data_59[6] = 0x0; data_59[7] = 0x0; data_59[8] = 0x0; data_59[9] = 0x48; data_59[10] = 0xba; data_59[11] = 0xf8; data_59[12] = 0x0; data_59[13] = 0x0; data_59[14] = 0x0; data_59[15] = 0x0; data_59[16] = 0x0; data_59[17] = 0x0; data_59[18] = 0x0; data_59[19] = 0x48; data_59[20] = 0x89; data_59[21] = 0xc8; data_59[22] = 0x48; data_59[23] = 0x1; data_59[24] = 0xd0; data_59[25] = 0x49; data_59[26] = 0xbb; data_59[27] = 0x19; data_59[28] = 0x1; data_59[29] = 0x0; data_59[30] = 0x0; data_59[31] = 0x0; data_59[32] = 0x0; data_59[33] = 0x0; data_59[34] = 0x0; data_59[35] = 0x4c; data_59[36] = 0x39; data_59[37] = 0xd8; data_59[38] = 0xf; data_59[39] = 0x94; data_59[40] = 0xc0; data_59[41] = 0x48; data_59[42] = 0x83; data_59[43] = 0xe0; data_59[44] = 0x1; data_59[45] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_59)(&flag[59]);
    counter += res;
    memset((void*)data_59, 0, 46);

    // kusd_check;add;0x87
    volatile uint8_t data_9[53] = {};
    // clang-format off
    data_9[0] = 0x8a; data_9[1] = 0x9; data_9[2] = 0x48; data_9[3] = 0x81; data_9[4] = 0xe1; data_9[5] = 0xff; data_9[6] = 0x0; data_9[7] = 0x0; data_9[8] = 0x0; data_9[9] = 0x48; data_9[10] = 0x31; data_9[11] = 0xd2; data_9[12] = 0x8a; data_9[13] = 0x14; data_9[14] = 0x25; data_9[15] = 0xd4; data_9[16] = 0x2; data_9[17] = 0xfe; data_9[18] = 0x7f; data_9[19] = 0x83; data_9[20] = 0xc2; data_9[21] = 0x1; data_9[22] = 0x48; data_9[23] = 0xc1; data_9[24] = 0xe2; data_9[25] = 0x4; data_9[26] = 0x48; data_9[27] = 0x89; data_9[28] = 0xc8; data_9[29] = 0x48; data_9[30] = 0x1; data_9[31] = 0xd0; data_9[32] = 0x49; data_9[33] = 0xbb; data_9[34] = 0x87; data_9[35] = 0x0; data_9[36] = 0x0; data_9[37] = 0x0; data_9[38] = 0x0; data_9[39] = 0x0; data_9[40] = 0x0; data_9[41] = 0x0; data_9[42] = 0x4c; data_9[43] = 0x39; data_9[44] = 0xd8; data_9[45] = 0xf; data_9[46] = 0x94; data_9[47] = 0xc0; data_9[48] = 0x48; data_9[49] = 0x83; data_9[50] = 0xe0; data_9[51] = 0x1; data_9[52] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_9)(&flag[9]);
    counter += res;
    memset((void*)data_9, 0, 53);

    // peb_check;xor;0x4f
    volatile uint8_t data_28[59] = {};
    // clang-format off
    data_28[0] = 0x8a; data_28[1] = 0x9; data_28[2] = 0x48; data_28[3] = 0x81; data_28[4] = 0xe1; data_28[5] = 0xff; data_28[6] = 0x0; data_28[7] = 0x0; data_28[8] = 0x0; data_28[9] = 0x65; data_28[10] = 0x4c; data_28[11] = 0x8b; data_28[12] = 0x14; data_28[13] = 0x25; data_28[14] = 0x60; data_28[15] = 0x0; data_28[16] = 0x0; data_28[17] = 0x0; data_28[18] = 0x48; data_28[19] = 0x31; data_28[20] = 0xd2; data_28[21] = 0x41; data_28[22] = 0x8a; data_28[23] = 0x52; data_28[24] = 0x2; data_28[25] = 0x83; data_28[26] = 0xc2; data_28[27] = 0x1; data_28[28] = 0x48; data_28[29] = 0xc1; data_28[30] = 0xe2; data_28[31] = 0x4; data_28[32] = 0x48; data_28[33] = 0x89; data_28[34] = 0xc8; data_28[35] = 0x48; data_28[36] = 0x31; data_28[37] = 0xd0; data_28[38] = 0x49; data_28[39] = 0xbb; data_28[40] = 0x4f; data_28[41] = 0x0; data_28[42] = 0x0; data_28[43] = 0x0; data_28[44] = 0x0; data_28[45] = 0x0; data_28[46] = 0x0; data_28[47] = 0x0; data_28[48] = 0x4c; data_28[49] = 0x39; data_28[50] = 0xd8; data_28[51] = 0xf; data_28[52] = 0x94; data_28[53] = 0xc0; data_28[54] = 0x48; data_28[55] = 0x83; data_28[56] = 0xe0; data_28[57] = 0x1; data_28[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_28)(&flag[28]);
    counter += res;
    memset((void*)data_28, 0, 59);

    // peb_check;sub;0x67
    volatile uint8_t data_85[59] = {};
    // clang-format off
    data_85[0] = 0x8a; data_85[1] = 0x9; data_85[2] = 0x48; data_85[3] = 0x81; data_85[4] = 0xe1; data_85[5] = 0xff; data_85[6] = 0x0; data_85[7] = 0x0; data_85[8] = 0x0; data_85[9] = 0x65; data_85[10] = 0x4c; data_85[11] = 0x8b; data_85[12] = 0x14; data_85[13] = 0x25; data_85[14] = 0x60; data_85[15] = 0x0; data_85[16] = 0x0; data_85[17] = 0x0; data_85[18] = 0x48; data_85[19] = 0x31; data_85[20] = 0xd2; data_85[21] = 0x41; data_85[22] = 0x8a; data_85[23] = 0x52; data_85[24] = 0x2; data_85[25] = 0x83; data_85[26] = 0xc2; data_85[27] = 0x1; data_85[28] = 0x48; data_85[29] = 0xc1; data_85[30] = 0xe2; data_85[31] = 0x4; data_85[32] = 0x48; data_85[33] = 0x89; data_85[34] = 0xc8; data_85[35] = 0x48; data_85[36] = 0x29; data_85[37] = 0xd0; data_85[38] = 0x49; data_85[39] = 0xbb; data_85[40] = 0x67; data_85[41] = 0x0; data_85[42] = 0x0; data_85[43] = 0x0; data_85[44] = 0x0; data_85[45] = 0x0; data_85[46] = 0x0; data_85[47] = 0x0; data_85[48] = 0x4c; data_85[49] = 0x39; data_85[50] = 0xd8; data_85[51] = 0xf; data_85[52] = 0x94; data_85[53] = 0xc0; data_85[54] = 0x48; data_85[55] = 0x83; data_85[56] = 0xe0; data_85[57] = 0x1; data_85[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_85)(&flag[85]);
    counter += res;
    memset((void*)data_85, 0, 59);

    // kusd_check;sub;0x5e
    volatile uint8_t data_78[53] = {};
    // clang-format off
    data_78[0] = 0x8a; data_78[1] = 0x9; data_78[2] = 0x48; data_78[3] = 0x81; data_78[4] = 0xe1; data_78[5] = 0xff; data_78[6] = 0x0; data_78[7] = 0x0; data_78[8] = 0x0; data_78[9] = 0x48; data_78[10] = 0x31; data_78[11] = 0xd2; data_78[12] = 0x8a; data_78[13] = 0x14; data_78[14] = 0x25; data_78[15] = 0xd4; data_78[16] = 0x2; data_78[17] = 0xfe; data_78[18] = 0x7f; data_78[19] = 0x83; data_78[20] = 0xc2; data_78[21] = 0x1; data_78[22] = 0x48; data_78[23] = 0xc1; data_78[24] = 0xe2; data_78[25] = 0x4; data_78[26] = 0x48; data_78[27] = 0x89; data_78[28] = 0xc8; data_78[29] = 0x48; data_78[30] = 0x29; data_78[31] = 0xd0; data_78[32] = 0x49; data_78[33] = 0xbb; data_78[34] = 0x5e; data_78[35] = 0x0; data_78[36] = 0x0; data_78[37] = 0x0; data_78[38] = 0x0; data_78[39] = 0x0; data_78[40] = 0x0; data_78[41] = 0x0; data_78[42] = 0x4c; data_78[43] = 0x39; data_78[44] = 0xd8; data_78[45] = 0xf; data_78[46] = 0x94; data_78[47] = 0xc0; data_78[48] = 0x48; data_78[49] = 0x83; data_78[50] = 0xe0; data_78[51] = 0x1; data_78[52] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_78)(&flag[78]);
    counter += res;
    memset((void*)data_78, 0, 53);

    // imm64;xor;0x7f
    volatile uint8_t data_66[46] = {};
    // clang-format off
    data_66[0] = 0x8a; data_66[1] = 0x9; data_66[2] = 0x48; data_66[3] = 0x81; data_66[4] = 0xe1; data_66[5] = 0xff; data_66[6] = 0x0; data_66[7] = 0x0; data_66[8] = 0x0; data_66[9] = 0x48; data_66[10] = 0xba; data_66[11] = 0x12; data_66[12] = 0x0; data_66[13] = 0x0; data_66[14] = 0x0; data_66[15] = 0x0; data_66[16] = 0x0; data_66[17] = 0x0; data_66[18] = 0x0; data_66[19] = 0x48; data_66[20] = 0x89; data_66[21] = 0xc8; data_66[22] = 0x48; data_66[23] = 0x31; data_66[24] = 0xd0; data_66[25] = 0x49; data_66[26] = 0xbb; data_66[27] = 0x7f; data_66[28] = 0x0; data_66[29] = 0x0; data_66[30] = 0x0; data_66[31] = 0x0; data_66[32] = 0x0; data_66[33] = 0x0; data_66[34] = 0x0; data_66[35] = 0x4c; data_66[36] = 0x39; data_66[37] = 0xd8; data_66[38] = 0xf; data_66[39] = 0x94; data_66[40] = 0xc0; data_66[41] = 0x48; data_66[42] = 0x83; data_66[43] = 0xe0; data_66[44] = 0x1; data_66[45] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_66)(&flag[66]);
    counter += res;
    memset((void*)data_66, 0, 46);

    // imm64;add;0x11f
    volatile uint8_t data_30[46] = {};
    // clang-format off
    data_30[0] = 0x8a; data_30[1] = 0x9; data_30[2] = 0x48; data_30[3] = 0x81; data_30[4] = 0xe1; data_30[5] = 0xff; data_30[6] = 0x0; data_30[7] = 0x0; data_30[8] = 0x0; data_30[9] = 0x48; data_30[10] = 0xba; data_30[11] = 0xec; data_30[12] = 0x0; data_30[13] = 0x0; data_30[14] = 0x0; data_30[15] = 0x0; data_30[16] = 0x0; data_30[17] = 0x0; data_30[18] = 0x0; data_30[19] = 0x48; data_30[20] = 0x89; data_30[21] = 0xc8; data_30[22] = 0x48; data_30[23] = 0x1; data_30[24] = 0xd0; data_30[25] = 0x49; data_30[26] = 0xbb; data_30[27] = 0x1f; data_30[28] = 0x1; data_30[29] = 0x0; data_30[30] = 0x0; data_30[31] = 0x0; data_30[32] = 0x0; data_30[33] = 0x0; data_30[34] = 0x0; data_30[35] = 0x4c; data_30[36] = 0x39; data_30[37] = 0xd8; data_30[38] = 0xf; data_30[39] = 0x94; data_30[40] = 0xc0; data_30[41] = 0x48; data_30[42] = 0x83; data_30[43] = 0xe0; data_30[44] = 0x1; data_30[45] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_30)(&flag[30]);
    counter += res;
    memset((void*)data_30, 0, 46);

    // imm64;xor;0x24
    volatile uint8_t data_34[46] = {};
    // clang-format off
    data_34[0] = 0x8a; data_34[1] = 0x9; data_34[2] = 0x48; data_34[3] = 0x81; data_34[4] = 0xe1; data_34[5] = 0xff; data_34[6] = 0x0; data_34[7] = 0x0; data_34[8] = 0x0; data_34[9] = 0x48; data_34[10] = 0xba; data_34[11] = 0x6d; data_34[12] = 0x0; data_34[13] = 0x0; data_34[14] = 0x0; data_34[15] = 0x0; data_34[16] = 0x0; data_34[17] = 0x0; data_34[18] = 0x0; data_34[19] = 0x48; data_34[20] = 0x89; data_34[21] = 0xc8; data_34[22] = 0x48; data_34[23] = 0x31; data_34[24] = 0xd0; data_34[25] = 0x49; data_34[26] = 0xbb; data_34[27] = 0x24; data_34[28] = 0x0; data_34[29] = 0x0; data_34[30] = 0x0; data_34[31] = 0x0; data_34[32] = 0x0; data_34[33] = 0x0; data_34[34] = 0x0; data_34[35] = 0x4c; data_34[36] = 0x39; data_34[37] = 0xd8; data_34[38] = 0xf; data_34[39] = 0x94; data_34[40] = 0xc0; data_34[41] = 0x48; data_34[42] = 0x83; data_34[43] = 0xe0; data_34[44] = 0x1; data_34[45] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_34)(&flag[34]);
    counter += res;
    memset((void*)data_34, 0, 46);

    // imm64;sub;0xffffffffffffff7b
    volatile uint8_t data_53[46] = {};
    // clang-format off
    data_53[0] = 0x8a; data_53[1] = 0x9; data_53[2] = 0x48; data_53[3] = 0x81; data_53[4] = 0xe1; data_53[5] = 0xff; data_53[6] = 0x0; data_53[7] = 0x0; data_53[8] = 0x0; data_53[9] = 0x48; data_53[10] = 0xba; data_53[11] = 0xb9; data_53[12] = 0x0; data_53[13] = 0x0; data_53[14] = 0x0; data_53[15] = 0x0; data_53[16] = 0x0; data_53[17] = 0x0; data_53[18] = 0x0; data_53[19] = 0x48; data_53[20] = 0x89; data_53[21] = 0xc8; data_53[22] = 0x48; data_53[23] = 0x29; data_53[24] = 0xd0; data_53[25] = 0x49; data_53[26] = 0xbb; data_53[27] = 0x7b; data_53[28] = 0xff; data_53[29] = 0xff; data_53[30] = 0xff; data_53[31] = 0xff; data_53[32] = 0xff; data_53[33] = 0xff; data_53[34] = 0xff; data_53[35] = 0x4c; data_53[36] = 0x39; data_53[37] = 0xd8; data_53[38] = 0xf; data_53[39] = 0x94; data_53[40] = 0xc0; data_53[41] = 0x48; data_53[42] = 0x83; data_53[43] = 0xe0; data_53[44] = 0x1; data_53[45] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_53)(&flag[53]);
    counter += res;
    memset((void*)data_53, 0, 46);

    // imm64;add;0x164
    volatile uint8_t data_6[46] = {};
    // clang-format off
    data_6[0] = 0x8a; data_6[1] = 0x9; data_6[2] = 0x48; data_6[3] = 0x81; data_6[4] = 0xe1; data_6[5] = 0xff; data_6[6] = 0x0; data_6[7] = 0x0; data_6[8] = 0x0; data_6[9] = 0x48; data_6[10] = 0xba; data_6[11] = 0xed; data_6[12] = 0x0; data_6[13] = 0x0; data_6[14] = 0x0; data_6[15] = 0x0; data_6[16] = 0x0; data_6[17] = 0x0; data_6[18] = 0x0; data_6[19] = 0x48; data_6[20] = 0x89; data_6[21] = 0xc8; data_6[22] = 0x48; data_6[23] = 0x1; data_6[24] = 0xd0; data_6[25] = 0x49; data_6[26] = 0xbb; data_6[27] = 0x64; data_6[28] = 0x1; data_6[29] = 0x0; data_6[30] = 0x0; data_6[31] = 0x0; data_6[32] = 0x0; data_6[33] = 0x0; data_6[34] = 0x0; data_6[35] = 0x4c; data_6[36] = 0x39; data_6[37] = 0xd8; data_6[38] = 0xf; data_6[39] = 0x94; data_6[40] = 0xc0; data_6[41] = 0x48; data_6[42] = 0x83; data_6[43] = 0xe0; data_6[44] = 0x1; data_6[45] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_6)(&flag[6]);
    counter += res;
    memset((void*)data_6, 0, 46);

    // peb_check;xor;0x20
    volatile uint8_t data_7[59] = {};
    // clang-format off
    data_7[0] = 0x8a; data_7[1] = 0x9; data_7[2] = 0x48; data_7[3] = 0x81; data_7[4] = 0xe1; data_7[5] = 0xff; data_7[6] = 0x0; data_7[7] = 0x0; data_7[8] = 0x0; data_7[9] = 0x65; data_7[10] = 0x4c; data_7[11] = 0x8b; data_7[12] = 0x14; data_7[13] = 0x25; data_7[14] = 0x60; data_7[15] = 0x0; data_7[16] = 0x0; data_7[17] = 0x0; data_7[18] = 0x48; data_7[19] = 0x31; data_7[20] = 0xd2; data_7[21] = 0x41; data_7[22] = 0x8a; data_7[23] = 0x52; data_7[24] = 0x2; data_7[25] = 0x83; data_7[26] = 0xc2; data_7[27] = 0x1; data_7[28] = 0x48; data_7[29] = 0xc1; data_7[30] = 0xe2; data_7[31] = 0x4; data_7[32] = 0x48; data_7[33] = 0x89; data_7[34] = 0xc8; data_7[35] = 0x48; data_7[36] = 0x31; data_7[37] = 0xd0; data_7[38] = 0x49; data_7[39] = 0xbb; data_7[40] = 0x20; data_7[41] = 0x0; data_7[42] = 0x0; data_7[43] = 0x0; data_7[44] = 0x0; data_7[45] = 0x0; data_7[46] = 0x0; data_7[47] = 0x0; data_7[48] = 0x4c; data_7[49] = 0x39; data_7[50] = 0xd8; data_7[51] = 0xf; data_7[52] = 0x94; data_7[53] = 0xc0; data_7[54] = 0x48; data_7[55] = 0x83; data_7[56] = 0xe0; data_7[57] = 0x1; data_7[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_7)(&flag[7]);
    counter += res;
    memset((void*)data_7, 0, 59);

    // kusd_check;add;0x31
    volatile uint8_t data_62[53] = {};
    // clang-format off
    data_62[0] = 0x8a; data_62[1] = 0x9; data_62[2] = 0x48; data_62[3] = 0x81; data_62[4] = 0xe1; data_62[5] = 0xff; data_62[6] = 0x0; data_62[7] = 0x0; data_62[8] = 0x0; data_62[9] = 0x48; data_62[10] = 0x31; data_62[11] = 0xd2; data_62[12] = 0x8a; data_62[13] = 0x14; data_62[14] = 0x25; data_62[15] = 0xd4; data_62[16] = 0x2; data_62[17] = 0xfe; data_62[18] = 0x7f; data_62[19] = 0x83; data_62[20] = 0xc2; data_62[21] = 0x1; data_62[22] = 0x48; data_62[23] = 0xc1; data_62[24] = 0xe2; data_62[25] = 0x4; data_62[26] = 0x48; data_62[27] = 0x89; data_62[28] = 0xc8; data_62[29] = 0x48; data_62[30] = 0x1; data_62[31] = 0xd0; data_62[32] = 0x49; data_62[33] = 0xbb; data_62[34] = 0x31; data_62[35] = 0x0; data_62[36] = 0x0; data_62[37] = 0x0; data_62[38] = 0x0; data_62[39] = 0x0; data_62[40] = 0x0; data_62[41] = 0x0; data_62[42] = 0x4c; data_62[43] = 0x39; data_62[44] = 0xd8; data_62[45] = 0xf; data_62[46] = 0x94; data_62[47] = 0xc0; data_62[48] = 0x48; data_62[49] = 0x83; data_62[50] = 0xe0; data_62[51] = 0x1; data_62[52] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_62)(&flag[62]);
    counter += res;
    memset((void*)data_62, 0, 53);

    // peb_check;add;0x75
    volatile uint8_t data_25[59] = {};
    // clang-format off
    data_25[0] = 0x8a; data_25[1] = 0x9; data_25[2] = 0x48; data_25[3] = 0x81; data_25[4] = 0xe1; data_25[5] = 0xff; data_25[6] = 0x0; data_25[7] = 0x0; data_25[8] = 0x0; data_25[9] = 0x65; data_25[10] = 0x4c; data_25[11] = 0x8b; data_25[12] = 0x14; data_25[13] = 0x25; data_25[14] = 0x60; data_25[15] = 0x0; data_25[16] = 0x0; data_25[17] = 0x0; data_25[18] = 0x48; data_25[19] = 0x31; data_25[20] = 0xd2; data_25[21] = 0x41; data_25[22] = 0x8a; data_25[23] = 0x52; data_25[24] = 0x2; data_25[25] = 0x83; data_25[26] = 0xc2; data_25[27] = 0x1; data_25[28] = 0x48; data_25[29] = 0xc1; data_25[30] = 0xe2; data_25[31] = 0x4; data_25[32] = 0x48; data_25[33] = 0x89; data_25[34] = 0xc8; data_25[35] = 0x48; data_25[36] = 0x1; data_25[37] = 0xd0; data_25[38] = 0x49; data_25[39] = 0xbb; data_25[40] = 0x75; data_25[41] = 0x0; data_25[42] = 0x0; data_25[43] = 0x0; data_25[44] = 0x0; data_25[45] = 0x0; data_25[46] = 0x0; data_25[47] = 0x0; data_25[48] = 0x4c; data_25[49] = 0x39; data_25[50] = 0xd8; data_25[51] = 0xf; data_25[52] = 0x94; data_25[53] = 0xc0; data_25[54] = 0x48; data_25[55] = 0x83; data_25[56] = 0xe0; data_25[57] = 0x1; data_25[58] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_25)(&flag[25]);
    counter += res;
    memset((void*)data_25, 0, 59);

    // imm64;sub;0xffffffffffffff45
    volatile uint8_t data_22[46] = {};
    // clang-format off
    data_22[0] = 0x8a; data_22[1] = 0x9; data_22[2] = 0x48; data_22[3] = 0x81; data_22[4] = 0xe1; data_22[5] = 0xff; data_22[6] = 0x0; data_22[7] = 0x0; data_22[8] = 0x0; data_22[9] = 0x48; data_22[10] = 0xba; data_22[11] = 0xee; data_22[12] = 0x0; data_22[13] = 0x0; data_22[14] = 0x0; data_22[15] = 0x0; data_22[16] = 0x0; data_22[17] = 0x0; data_22[18] = 0x0; data_22[19] = 0x48; data_22[20] = 0x89; data_22[21] = 0xc8; data_22[22] = 0x48; data_22[23] = 0x29; data_22[24] = 0xd0; data_22[25] = 0x49; data_22[26] = 0xbb; data_22[27] = 0x45; data_22[28] = 0xff; data_22[29] = 0xff; data_22[30] = 0xff; data_22[31] = 0xff; data_22[32] = 0xff; data_22[33] = 0xff; data_22[34] = 0xff; data_22[35] = 0x4c; data_22[36] = 0x39; data_22[37] = 0xd8; data_22[38] = 0xf; data_22[39] = 0x94; data_22[40] = 0xc0; data_22[41] = 0x48; data_22[42] = 0x83; data_22[43] = 0xe0; data_22[44] = 0x1; data_22[45] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_22)(&flag[22]);
    counter += res;
    memset((void*)data_22, 0, 46);

    // kusd_check;sub;0x4f
    volatile uint8_t data_11[53] = {};
    // clang-format off
    data_11[0] = 0x8a; data_11[1] = 0x9; data_11[2] = 0x48; data_11[3] = 0x81; data_11[4] = 0xe1; data_11[5] = 0xff; data_11[6] = 0x0; data_11[7] = 0x0; data_11[8] = 0x0; data_11[9] = 0x48; data_11[10] = 0x31; data_11[11] = 0xd2; data_11[12] = 0x8a; data_11[13] = 0x14; data_11[14] = 0x25; data_11[15] = 0xd4; data_11[16] = 0x2; data_11[17] = 0xfe; data_11[18] = 0x7f; data_11[19] = 0x83; data_11[20] = 0xc2; data_11[21] = 0x1; data_11[22] = 0x48; data_11[23] = 0xc1; data_11[24] = 0xe2; data_11[25] = 0x4; data_11[26] = 0x48; data_11[27] = 0x89; data_11[28] = 0xc8; data_11[29] = 0x48; data_11[30] = 0x29; data_11[31] = 0xd0; data_11[32] = 0x49; data_11[33] = 0xbb; data_11[34] = 0x4f; data_11[35] = 0x0; data_11[36] = 0x0; data_11[37] = 0x0; data_11[38] = 0x0; data_11[39] = 0x0; data_11[40] = 0x0; data_11[41] = 0x0; data_11[42] = 0x4c; data_11[43] = 0x39; data_11[44] = 0xd8; data_11[45] = 0xf; data_11[46] = 0x94; data_11[47] = 0xc0; data_11[48] = 0x48; data_11[49] = 0x83; data_11[50] = 0xe0; data_11[51] = 0x1; data_11[52] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_11)(&flag[11]);
    counter += res;
    memset((void*)data_11, 0, 53);

    // imm64;xor;0xc3
    volatile uint8_t data_49[46] = {};
    // clang-format off
    data_49[0] = 0x8a; data_49[1] = 0x9; data_49[2] = 0x48; data_49[3] = 0x81; data_49[4] = 0xe1; data_49[5] = 0xff; data_49[6] = 0x0; data_49[7] = 0x0; data_49[8] = 0x0; data_49[9] = 0x48; data_49[10] = 0xba; data_49[11] = 0xb7; data_49[12] = 0x0; data_49[13] = 0x0; data_49[14] = 0x0; data_49[15] = 0x0; data_49[16] = 0x0; data_49[17] = 0x0; data_49[18] = 0x0; data_49[19] = 0x48; data_49[20] = 0x89; data_49[21] = 0xc8; data_49[22] = 0x48; data_49[23] = 0x31; data_49[24] = 0xd0; data_49[25] = 0x49; data_49[26] = 0xbb; data_49[27] = 0xc3; data_49[28] = 0x0; data_49[29] = 0x0; data_49[30] = 0x0; data_49[31] = 0x0; data_49[32] = 0x0; data_49[33] = 0x0; data_49[34] = 0x0; data_49[35] = 0x4c; data_49[36] = 0x39; data_49[37] = 0xd8; data_49[38] = 0xf; data_49[39] = 0x94; data_49[40] = 0xc0; data_49[41] = 0x48; data_49[42] = 0x83; data_49[43] = 0xe0; data_49[44] = 0x1; data_49[45] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_49)(&flag[49]);
    counter += res;
    memset((void*)data_49, 0, 46);

    // imm64;add;0x6e
    volatile uint8_t data_82[46] = {};
    // clang-format off
    data_82[0] = 0x8a; data_82[1] = 0x9; data_82[2] = 0x48; data_82[3] = 0x81; data_82[4] = 0xe1; data_82[5] = 0xff; data_82[6] = 0x0; data_82[7] = 0x0; data_82[8] = 0x0; data_82[9] = 0x48; data_82[10] = 0xba; data_82[11] = 0x4; data_82[12] = 0x0; data_82[13] = 0x0; data_82[14] = 0x0; data_82[15] = 0x0; data_82[16] = 0x0; data_82[17] = 0x0; data_82[18] = 0x0; data_82[19] = 0x48; data_82[20] = 0x89; data_82[21] = 0xc8; data_82[22] = 0x48; data_82[23] = 0x1; data_82[24] = 0xd0; data_82[25] = 0x49; data_82[26] = 0xbb; data_82[27] = 0x6e; data_82[28] = 0x0; data_82[29] = 0x0; data_82[30] = 0x0; data_82[31] = 0x0; data_82[32] = 0x0; data_82[33] = 0x0; data_82[34] = 0x0; data_82[35] = 0x4c; data_82[36] = 0x39; data_82[37] = 0xd8; data_82[38] = 0xf; data_82[39] = 0x94; data_82[40] = 0xc0; data_82[41] = 0x48; data_82[42] = 0x83; data_82[43] = 0xe0; data_82[44] = 0x1; data_82[45] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_82)(&flag[82]);
    counter += res;
    memset((void*)data_82, 0, 46);

    // imm64;sub;0xffffffffffffff37
    volatile uint8_t data_58[46] = {};
    // clang-format off
    data_58[0] = 0x8a; data_58[1] = 0x9; data_58[2] = 0x48; data_58[3] = 0x81; data_58[4] = 0xe1; data_58[5] = 0xff; data_58[6] = 0x0; data_58[7] = 0x0; data_58[8] = 0x0; data_58[9] = 0x48; data_58[10] = 0xba; data_58[11] = 0xea; data_58[12] = 0x0; data_58[13] = 0x0; data_58[14] = 0x0; data_58[15] = 0x0; data_58[16] = 0x0; data_58[17] = 0x0; data_58[18] = 0x0; data_58[19] = 0x48; data_58[20] = 0x89; data_58[21] = 0xc8; data_58[22] = 0x48; data_58[23] = 0x29; data_58[24] = 0xd0; data_58[25] = 0x49; data_58[26] = 0xbb; data_58[27] = 0x37; data_58[28] = 0xff; data_58[29] = 0xff; data_58[30] = 0xff; data_58[31] = 0xff; data_58[32] = 0xff; data_58[33] = 0xff; data_58[34] = 0xff; data_58[35] = 0x4c; data_58[36] = 0x39; data_58[37] = 0xd8; data_58[38] = 0xf; data_58[39] = 0x94; data_58[40] = 0xc0; data_58[41] = 0x48; data_58[42] = 0x83; data_58[43] = 0xe0; data_58[44] = 0x1; data_58[45] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_58)(&flag[58]);
    counter += res;
    memset((void*)data_58, 0, 46);

    // kusd_check;xor;0x67
    volatile uint8_t data_44[53] = {};
    // clang-format off
    data_44[0] = 0x8a; data_44[1] = 0x9; data_44[2] = 0x48; data_44[3] = 0x81; data_44[4] = 0xe1; data_44[5] = 0xff; data_44[6] = 0x0; data_44[7] = 0x0; data_44[8] = 0x0; data_44[9] = 0x48; data_44[10] = 0x31; data_44[11] = 0xd2; data_44[12] = 0x8a; data_44[13] = 0x14; data_44[14] = 0x25; data_44[15] = 0xd4; data_44[16] = 0x2; data_44[17] = 0xfe; data_44[18] = 0x7f; data_44[19] = 0x83; data_44[20] = 0xc2; data_44[21] = 0x1; data_44[22] = 0x48; data_44[23] = 0xc1; data_44[24] = 0xe2; data_44[25] = 0x4; data_44[26] = 0x48; data_44[27] = 0x89; data_44[28] = 0xc8; data_44[29] = 0x48; data_44[30] = 0x31; data_44[31] = 0xd0; data_44[32] = 0x49; data_44[33] = 0xbb; data_44[34] = 0x67; data_44[35] = 0x0; data_44[36] = 0x0; data_44[37] = 0x0; data_44[38] = 0x0; data_44[39] = 0x0; data_44[40] = 0x0; data_44[41] = 0x0; data_44[42] = 0x4c; data_44[43] = 0x39; data_44[44] = 0xd8; data_44[45] = 0xf; data_44[46] = 0x94; data_44[47] = 0xc0; data_44[48] = 0x48; data_44[49] = 0x83; data_44[50] = 0xe0; data_44[51] = 0x1; data_44[52] = 0xc3;
    // clang-format on
    res = ((uint64_t(*)(char*))data_44)(&flag[44]);
    counter += res;
    memset((void*)data_44, 0, 53);

    if (counter >= 100) {
        printf("[i] sounds about right\n");
    } else {
        printf("[i] hell nah\n");
    }

    VM_TIGER_WHITE_END;
    return 0;
}
