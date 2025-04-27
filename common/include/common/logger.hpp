#ifndef COMMON_LOGGER_HPP_
#define COMMON_LOGGER_HPP_

#include <utility>
#include <array>
#include <string_view>
#include <ranges>
#include <algorithm>

#include <esp_log.h>

#include <common/utility/string_literal.hpp>
#include <common/utility/format.hpp>

namespace common {


template<utility::string_literal Tag, std::size_t BufferSize = 64u>
class logger {

public:

  logger() = delete;

  template<typename... Args>
  static auto debug(utility::format_string format, Args&&... args) -> void {
    log(ESP_LOG_DEBUG, format, std::forward<Args>(args)...);
  }

  template<typename... Args>
  static auto info(utility::format_string format, Args&&... args) -> void {
    log(ESP_LOG_INFO, format, std::forward<Args>(args)...);
  }

  template<typename... Args>
  static auto warn(utility::format_string format, Args&&... args) -> void {
    log(ESP_LOG_WARN, format, std::forward<Args>(args)...);
  }

  template<typename... Args>
  static auto error(utility::format_string format, Args&&... args) -> void {
    log(ESP_LOG_ERROR, format, std::forward<Args>(args)...);
  }

private:

  template<typename... Args>
  static auto log(const esp_log_level_t level, utility::format_string format, Args&&... args) -> void {
    auto [end, size] = utility::format_to(std::span{buffer.data(), buffer.size() - 1u}, format, std::forward<Args>(args)...);

    *end = '\0';

    ESP_LOG_LEVEL(level, Tag.data(), "%s", buffer.data());
  }

  inline static std::array<char, BufferSize> buffer;

}; // class logger

} // namespace common

#endif // COMMON_LOGGER_HPP_
