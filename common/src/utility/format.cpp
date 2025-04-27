#include <common/utility/format.hpp>

namespace common::utility {

namespace detail {

// auto format_to(format_context& context, format_string format_string) -> bool {
//   const auto remaining = std::strlen(format_string);

//   if (context.written + remaining >= context.buffer_size) {
//     return false;  // Buffer overflow
//   }

//   std::strncpy(context.data(), format_string, remaining);

//   context.written += remaining;

//   return true;
// }

} // namespace detail

} // namespace common::utility
