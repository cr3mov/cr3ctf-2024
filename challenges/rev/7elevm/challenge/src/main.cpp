#include "barcode/barcode.hpp"
#include "util/logger.hpp"

#include <functional>
#include <unordered_map>

int main(int argc, char* argv[]) try {
    if (argc < 2) {
        throw std::runtime_error("usage: chall <rom_dir>");
    }

    const auto path = std::filesystem::path(argv[1]);
    if (!exists(path)) {
        throw std::runtime_error("invalid rom");
    }

    printf("*** scanning barcodes, please wait. do you need a bag tho?\n");

    std::vector<vm::cmd_t> cmds = {};
    for (const auto& file : std::filesystem::directory_iterator{path}) {
        std::cout << "** " << file.path().filename().string() << std::endl;
        auto cmd = barcode::read_command(file.path());
        if (!cmd.has_value()) {
            logger::critical("bruh: {}", static_cast<int>(cmd.error()));
            return 1;
        }

        cmds.emplace_back(cmd.value());
    }

    if (cmds.empty()) {
        throw std::runtime_error("empty rom");
    }

    bool should_exit = false;
    std::size_t cur_cmd_index = 0;

    std::unordered_map<vm::e_cmd_t, std::function<void(vm::cmd_t&)>> handlers = {
        //
        {vm::e_cmd_t::MOV_IMM,
         [&](vm::cmd_t& cmd) -> void {
             vm::ctx.registers[cmd.op1.get()] = cmd.imm.get();
         }},
        //
        {vm::e_cmd_t::ADD,
         [&](vm::cmd_t& cmd) -> void {
             vm::ctx.registers[cmd.op1.get()] += cmd.imm.get();
         }},
        //
        {vm::e_cmd_t::PUSH,
         [&](vm::cmd_t& cmd) -> void {
             vm::ctx.stack.push(cmd.imm.get());
         }},
        //
        {vm::e_cmd_t::CMP,
         [&](vm::cmd_t& cmd) -> void {
             vm::ctx.flags.zf = vm::ctx.registers[cmd.op1.get()] == vm::ctx.registers[cmd.op2.get()];
         }},
        //
        {vm::e_cmd_t::JZ,
         [&](vm::cmd_t& cmd) -> void {
             if (!vm::ctx.flags.zf) {
                 cur_cmd_index += cmd.imm.get();
             }
         }},
        //
        {vm::e_cmd_t::XOR,
         [&](vm::cmd_t& cmd) -> void {
             vm::ctx.registers[cmd.op1.get()] ^= cmd.imm.get();
         }},
        //
        {vm::e_cmd_t::SUB,
         [&](vm::cmd_t& cmd) -> void {
             vm::ctx.registers[cmd.op1.get()] -= cmd.imm.get();
         }},
        //
        {vm::e_cmd_t::INPUT_OUTPUT,
         [&](vm::cmd_t& cmd) -> void {
             if (cmd.imm.get() == 1) {
                 std::vector<std::uint32_t> data = {};
                 while (true) {
                     const auto val = vm::ctx.pop_stack_top();
                     data.emplace_back(val);

                     if (val == 0) {
                         break;
                     }
                 }

                 printf("%s\n", reinterpret_cast<char*>(data.data()));
             } else {
                 std::string str;
                 std::cin >> str;

                 for (std::size_t i = 0; i < 6; ++i) {
                     for (std::size_t j = 0; j < 8; ++j) {
                         auto ch = '?';
                         if (auto idx = i * 8 + j; str.size() > idx) {
                             ch = str.at(idx);
                         }

                         uint32_t value = 6;
                         if (ch == '|') {
                             value = 0;
                         } else if (ch == 'l') {
                             value = 1;
                         } else if (ch == '_') {
                             value = 2;
                         } else if (ch == 'I') {
                             value = 3;
                         } else if (ch == '1') {
                             value = 4;
                         } else if (ch == 'i') {
                             value = 5;
                         }

                         vm::ctx.cube.sides[i] |= (value << j * 4);
                     }
                 }
             }
         }},
        //
        {vm::e_cmd_t::EXIT,
         [&](vm::cmd_t& cmd) -> void {
             should_exit = true;
         }},
        //
        {vm::e_cmd_t::CUBE_UP,
         [&](vm::cmd_t& cmd) -> void {
             vm::ctx.cube.up(cmd.imm.get() > 0);
         }},
        //
        {vm::e_cmd_t::CUBE_DOWN,
         [&](vm::cmd_t& cmd) -> void {
             vm::ctx.cube.down(cmd.imm.get() > 0);
         }},
        //
        {vm::e_cmd_t::CUBE_LEFT,
         [&](vm::cmd_t& cmd) -> void {
             vm::ctx.cube.left(cmd.imm.get() > 0);
         }},
        //
        {vm::e_cmd_t::CUBE_RIGHT,
         [&](vm::cmd_t& cmd) -> void {
             vm::ctx.cube.right(cmd.imm.get() > 0);
         }},
        //
        {vm::e_cmd_t::CUBE_FRONT,
         [&](vm::cmd_t& cmd) -> void {
             vm::ctx.cube.front(cmd.imm.get() > 0);
         }},
        //
        {vm::e_cmd_t::CUBE_BACK,
         [&](vm::cmd_t& cmd) -> void {
             vm::ctx.cube.back(cmd.imm.get() > 0);
         }},
        //
        {vm::e_cmd_t::CUBE_CMP_SIDES,
         [&](vm::cmd_t& cmd) -> void {
             vm::ctx.flags.zf = true;

             for (std::size_t i = 0; i < 6; ++i) {
                 vm::ctx.flags.zf &= static_cast<uint32_t>(vm::ctx.registers[i]) == vm::ctx.cube.sides[i];
             }
         }},
    };

    for (; cur_cmd_index < cmds.size(); ++cur_cmd_index) {
        auto& cmd = cmds.at(cur_cmd_index);
        handlers[cmd.opcode.get<vm::e_cmd_t>()](cmd);

        if (should_exit) {
            break;
        }
    }

    printf("*** end of execution\n");
    return 0;
} catch (const std::runtime_error& err) {
    logger::critical("RUNTIME EXCEPTION: {}", err.what());
}
