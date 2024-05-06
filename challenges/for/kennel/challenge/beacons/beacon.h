#ifndef BEACON_H
#define BEACON_H
#include <cstring>
#include <string>

class __attribute__((packed, aligned(1))) beacon_t {
 public:
  explicit beacon_t(const std::string& path);

  explicit beacon_t(std::int8_t typet, char text_datat[32]) {
    type = typet;
    std::memcpy(text_data, text_datat, 32);
  }

  bool pack_beacon(const std::string& path);

  std::uint8_t type;
  char text_data[32];
};

#endif  // BEACON_H
