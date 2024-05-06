#ifndef UTIL_H
#define UTIL_H
#include <fstream>
#include <ios>
#include <vector>

[[nodiscard]] inline std::vector<uint8_t> read_file(const std::string&path) {
    std::fstream file(path, std::ios::in | std::ios::binary);
    if (!file) {
        return {};
    }

    file.seekg(0, std::fstream::end);
    const auto file_size = file.tellg();
    file.seekg(0, std::fstream::beg);

    std::vector<uint8_t> buffer(file_size);
    file.read(reinterpret_cast<char *>(buffer.data()), static_cast<std::streamsize>(buffer.size()));

    return buffer;
}

inline void write_file(const std::string&path, const std::uint8_t* raw_buffer, const size_t buffer_size) {
    std::ofstream file(path, std::ios::binary | std::ios::out);
    file.write(reinterpret_cast<const char *>(raw_buffer), buffer_size);
    file.close();
}


[[nodiscard]] inline std::pair<std::string, std::string> split_path_data(const char beacon[33]) {
    std::string payload = std::string(beacon);
    auto del_idx = payload.find('|');
    std::filesystem::path path(payload.substr(0, del_idx));
    std::string data = payload.substr(del_idx + 1, 33 - del_idx);
    return {path, data};
}

#endif //UTIL_H
