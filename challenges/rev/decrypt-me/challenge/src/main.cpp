#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <random>
#include <vector>

#define FORCEINL

#if defined(__clang__) && defined(__linux__)
unsigned char _rotl8(unsigned char x, unsigned char y) {
    return __builtin_rotateleft8(x, y);
}
char _rotr8(char x, char y) {
    return __builtin_rotateright8(x, y);
}
uint64_t _byteswap_uint64(uint64_t value) {
    return __builtin_bswap64(value);
}
#endif

FORCEINL std::vector<uint8_t> read_file(const std::filesystem::path& path) noexcept {
    std::fstream f(path, std::ios::in | std::ios::binary);
    if (!f)
        return {};

    f.seekg(0, f.end);
    const auto f_size = f.tellg();
    f.seekg(0, f.beg);

    std::vector<uint8_t> buffer(f_size);
    f.read(reinterpret_cast<char*>(buffer.data()), buffer.size());

    return buffer;
}

FORCEINL void write_file(const std::filesystem::path& path, const std::uint8_t* raw_buffer, const size_t buffer_size) {
    std::ofstream file(path, std::ios::binary | std::ios::out);
    file.write(reinterpret_cast<const char*>(raw_buffer), buffer_size);
    file.close();
}

#pragma pack(push, 1)
struct hdr_t {
    uint64_t m_seed = 0;
    uint32_t m_seed2 = 0;
    uint32_t m_seed3 = 0;
};
#pragma pack(pop)

template <class Ty, class... Types>
inline constexpr bool is_any_of_v = std::disjunction_v<std::is_same<Ty, Types>...>;

std::mt19937 prng = {};
template <typename Ty = std::uint32_t, typename TyVal = std::remove_reference_t<Ty>, typename Limits = std::numeric_limits<TyVal>>
FORCEINL TyVal random_number() {
    if constexpr (std::is_same_v<TyVal, uint64_t>) {
        return (static_cast<uint64_t>(prng()) << 32) | prng();
    } else if constexpr (std::is_same_v<TyVal, uint32_t>) {
        return prng();
    } else if constexpr (std::is_same_v<TyVal, uint8_t>) {
        return prng() & 0xFF;
    } else {
        std::terminate();
    }
}

FORCEINL void xtea_encipher(uint32_t v[2], uint32_t const key[4], uint32_t const delta_v, unsigned int const num_rounds = 64) {
    uint32_t v0 = v[0], v1 = v[1], sum = 0, delta = delta_v;
    for (unsigned int i = 0; i < num_rounds; i++) {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
        sum += delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum >> 11) & 3]);
    }
    v[0] = v0;
    v[1] = v1;
}

int main() {
    auto content = read_file(L"flag.txt");
    if (content.empty() || content.size() % 8 != 0) {
        printf("no flag :(\n");
        return EXIT_FAILURE;
    }

    auto* ptr = new hdr_t();
    ptr->m_seed = __rdtsc();

    std::vector<std::uint8_t> content_dummy_copy = {};
    for (unsigned char i : content) {
        content_dummy_copy.push_back(i);
    }
    content = content_dummy_copy;

    std::random_device device;

    auto rnd_seed = device();
    prng.seed(rnd_seed);

    auto orig_seed = ptr->m_seed;

    volatile auto kek = 0x6362639724379833;
    kek ^= 0x5a67639724379833;
    kek = _byteswap_uint64(kek);

    auto iters = static_cast<std::size_t>(orig_seed) % kek;

    ptr->m_seed2 = random_number<std::uint32_t>() + (static_cast<uint32_t>(content.size()) / 7);
    ptr->m_seed3 = random_number<std::uint32_t>() ^ random_number<std::uint32_t>();
    ptr->m_seed = static_cast<uint64_t>(ptr->m_seed) ^ random_number<std::uint64_t>();

    uint32_t key_[] = {
        random_number<std::uint32_t>(),
        random_number<std::uint32_t>(),
        random_number<std::uint32_t>(),
        random_number<std::uint32_t>(),
    };
    uint32_t delta_ = random_number<std::uint32_t>();
    uint32_t num_rounds_ = std::min(std::max(delta_ & 0xFF, 32U), 64U);

    for (std::size_t i = 0; i < (content.size() / (sizeof(uint32_t) * 2)); ++i) {
        auto* ptr = &content[i * 8];
        xtea_encipher((uint32_t*)ptr, key_, delta_, num_rounds_);
    }

    for (std::size_t i = 0; i < iters; ++i) {
        auto pp = (random_number<uint8_t>() & 0xF) * (random_number<uint8_t>() & 0b111) % ((random_number<uint8_t>() & 0xF) + 5);
        pp ^= orig_seed & 0b111;
        for (std::size_t k = 0; k < pp; ++k) {
            for (unsigned char& j : content) {
                switch (random_number<uint8_t>() % 6) {
                case 0:
                    j ^= random_number<std::uint8_t>();
                    break;

                case 1:
                    j += random_number<std::uint8_t>();
                    break;

                case 2:
                    j -= random_number<std::uint8_t>();
                    break;

                case 3:
                    j = _rotl8(j, 1);
                    break;

                case 4:
                    j = _rotr8(j, 1);
                    break;

                case 5:
                    j = ~j;
                    break;

                default:
                    std::terminate();
                    break;
                }
            }
        }
    }

    std::vector<std::uint8_t> out_buffer;
    out_buffer.resize(sizeof(hdr_t) + content.size());

    memcpy(out_buffer.data(), ptr, sizeof(hdr_t));
    memcpy(out_buffer.data() + sizeof(hdr_t), content.data(), content.size());

    write_file(L"flag.enc.bin", out_buffer.data(), out_buffer.size());
    return EXIT_SUCCESS;
}
