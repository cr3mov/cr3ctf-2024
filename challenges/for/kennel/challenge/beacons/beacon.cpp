#include "beacon.h"

#include <Magick++.h>

#include <cstring>

#include "consts.h"

unsigned char reverse(unsigned char b) {
  b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
  b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
  b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
  return b;
}

beacon_t::beacon_t(const std::string& path) {
  Magick::Image beacon(path);
  Magick::Image base_image(kBaseImagePath);
  unsigned char packed_data[33] = {0};

  if (beacon.columns() != base_image.columns() ||
      beacon.rows() != base_image.rows())
    throw std::length_error("Wrong filesize");
  for (ssize_t x = 0; x < 132; x += 4) {
    for (ssize_t i = 0; i < 4; i++) {
      auto mcolor = beacon.pixelColor(x + i, 0);
      auto bcolor = base_image.pixelColor(x + i, 0);  // later will be y prolly

      auto mr = static_cast<std::uint16_t>(mcolor.redQuantum());
      auto br = static_cast<std::uint16_t>(bcolor.redQuantum());

      auto mg = static_cast<std::uint16_t>(mcolor.greenQuantum());
      auto bg = static_cast<std::uint16_t>(bcolor.greenQuantum());

      std::int32_t data_red = (mr ^ br) & 1;
      std::int32_t data_green = (mg ^ bg) & 1;

      // don't care, actually it's a bool
      packed_data[x / 4] =
          ((packed_data[x / 4] << 2) | (data_red << 1) | data_green);
    }
  }

  type = packed_data[0];
  std::memcpy(text_data, packed_data + 1, 32);
}

bool beacon_t::pack_beacon(const std::string& path) {
  Magick::Image base_image(kBaseImagePath);
  Magick::Image beacon(base_image);
  beacon.modifyImage();
  unsigned char packed_data[33] = {0};
  packed_data[0] = type;
  std::memcpy(packed_data + 1, text_data, 32);

  for (unsigned char& c : packed_data) {
    c = reverse(c);
  }

  for (ssize_t x = 0; x < 132; x += 4) {
    for (ssize_t i = 0; i < 4; i++) {
      auto bcolor = base_image.pixelColor(x + i, 0);
      auto br = static_cast<uint16_t>(bcolor.redQuantum());
      auto bg = static_cast<uint16_t>(bcolor.greenQuantum());
      auto r = (packed_data[x / 4] & 1) ^ br;
      auto g = ((packed_data[x / 4] >> 1) & 1) ^ bg;
      beacon.pixelColor(
          x + i, 0,
          Magick::Color(static_cast<float>(r), static_cast<float>(g),
                        bcolor.blueQuantum()));

      packed_data[x / 4] >>= 2;
    }
  }

  beacon.write(path);

  return true;
}
