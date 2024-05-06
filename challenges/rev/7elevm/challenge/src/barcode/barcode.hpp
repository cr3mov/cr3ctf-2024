#pragma once
#include <bitset>
#include <expected>
#include <filesystem>
#include <memory>

#include "zxing/ReadBarcode.h"
#include "stb/stb_image.h"

#include "util/logger.hpp"
#include "vm/structs.hpp"

namespace barcode {
    namespace detail {
        class STBImage : public ZXing::ImageView {
            std::unique_ptr<stbi_uc[], void (*)(void*)> _memory;

        public:
            STBImage(): ImageView(nullptr, 0, 0, ZXing::ImageFormat::None), _memory(nullptr, stbi_image_free) { }

            void load(const std::filesystem::path& imgPath) {
                int width, height, colors;
                _memory.reset(stbi_load(imgPath.string().c_str(), &width, &height, &colors, 3));
                if (_memory == nullptr)
                    throw std::runtime_error("Failed to read image");

                auto* img = _memory.get();
                for (int i = 0; i < width * height; ++i)
                    img[i] = ZXing::RGBToLum(img[3 * i + 0], img[3 * i + 1], img[3 * i + 2]);

                ImageView::operator=({_memory.get(), width, height, ZXing::ImageFormat::Lum});
            }

            explicit operator bool() const {
                return _data;
            }
        };
    } // namespace detail

    enum class e_error_code : std::uint8_t {
        UNKNOWN = 0,
        UNABLE_TO_DECODE,
        INVALID_PAYLOAD,
    };

    inline std::expected<vm::cmd_t, e_error_code> read_command(const std::filesystem::path& path) {
        detail::STBImage img;
        img.load(path);

        const auto options = ZXing::ReaderOptions().setFormats(ZXing::BarcodeFormat::Code39);
        const auto results = ZXing::ReadBarcodes(img, options);

        if (results.size() != 1) {
            return std::unexpected(e_error_code::UNABLE_TO_DECODE);
        }

        vm::cmd_t result = {};

        for (const auto& r : results) {
            const auto pp = r.bytes().asString();
            if (pp.size() != (sizeof(vm::cmd_t) * CHAR_BIT)) {
                return std::unexpected(e_error_code::INVALID_PAYLOAD);
            }

            auto* p_result = reinterpret_cast<std::uint8_t*>(&result);
            for (std::size_t i = 0; i < sizeof(vm::cmd_t); ++i) {
                const auto payload = pp.substr(i * CHAR_BIT, CHAR_BIT);
                std::bitset<CHAR_BIT> vals(payload.data(), payload.size());

                p_result[i] = static_cast<std::uint8_t>(vals.to_ulong());
            }
        }

        return result;
    }
} // namespace barcode
