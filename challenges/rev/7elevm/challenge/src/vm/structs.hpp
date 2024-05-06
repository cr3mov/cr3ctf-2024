#pragma once
#include <array>
#include <cstdint>
#include <stack>

#include "../util/platform.hpp"
#include "../cube/cube.hpp"

#pragma pack(push, 1)
namespace vm {
    constexpr std::uint32_t kRomMagic = '1MOR';

    struct header_t {
        std::uint32_t magic = kRomMagic;
    };
    static_assert(sizeof(header_t) == 4, " ");

    template <typename ItemTy, typename HolderTy, size_t SizeBits = sizeof(ItemTy) * 8 / 2>
    struct encrypted_val_t {
        union {
            struct {
                ItemTy value_ : SizeBits;
                ItemTy key_ : SizeBits;
            };
            HolderTy encrypted;
        };

        [[nodiscard]] FORCEINL ItemTy get_value() const {
            return value_;
        }

        [[nodiscard]] FORCEINL ItemTy get_key() const {
            return key_;
        }

        template <typename Ty = ItemTy>
        FORCEINL Ty get() const noexcept {
            return static_cast<Ty>(get_value() ^ get_key());
        }
    };

    struct cmd_t {
        encrypted_val_t<std::uint8_t, std::uint8_t> opcode;
        encrypted_val_t<std::uint8_t, std::uint8_t> op1;
        encrypted_val_t<std::uint8_t, std::uint8_t> op2;
        encrypted_val_t<std::uint32_t, std::uint64_t, 32> imm;
    };
    static_assert(sizeof(cmd_t) == 11, " ");

    struct ctx_t {
        std::array<uint64_t, 6> registers = {};
        std::stack<uint32_t> stack = {};
        Cube cube = {};

        struct flags_t {
            union {
                std::uint8_t zf:1;
                std::uint8_t pad:7;
            };
        } flags = {};

        FORCEINL std::uint32_t pop_stack_top() {
            const auto r = stack.top();
            stack.pop();
            return r;
        }

        FORCEINL std::uint64_t pop_stack_top_64() {
            union {
                struct {
                    std::uint32_t u32_0, u32_1;
                };
                std::uint64_t u64_0;
            } converter = {};

            converter.u32_0 = pop_stack_top();
            converter.u32_1 = pop_stack_top();
            return converter.u64_0;
        }
    } ctx;

    enum e_cmd_t : std::uint8_t {
        MOV_IMM = 0,
        ADD,
        CMP,
        JZ,
        XOR,
        SUB,
        PUSH,
        INPUT_OUTPUT,
        EXIT,
        CUBE_UP,
        CUBE_DOWN,
        CUBE_LEFT,
        CUBE_RIGHT,
        CUBE_FRONT,
        CUBE_BACK,
        CUBE_CMP_SIDES,
    };
} // namespace vm
#pragma pack(pop)
