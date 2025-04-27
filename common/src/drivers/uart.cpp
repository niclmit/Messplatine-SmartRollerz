#include <common/drivers/uart.hpp>

#include <vector>

#include <esp_log.h>

namespace common::drivers {

uart::uart(const config& config)
: _config{config},
  _is_synced{false} {
  auto uart_config = uart_config_t{
    .baud_rate = static_cast<std::int32_t>(_config.baut_rate),
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    .rx_flow_ctrl_thresh = 0,
    .source_clk = UART_SCLK_DEFAULT
  };

  uart_driver_install(_config.port, _config.rx_buffer_size, _config.tx_buffer_size, 0, nullptr, 0);
  uart_param_config(_config.port, &uart_config);
  uart_set_pin(_config.port, _config.tx, _config.rx, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

uart::~uart() {
  
}

/*
  To be able to properly sync the data stream that we read/write over uart we use frames with delimiters.

  The sender encodes his data stream and adds a 0x7E at the end:


      0x01 0x02 0x03 0x04 0x7E 0x06 0x07D 0x08 0x09 0x0A
                           |         |                             +--- gets added
                           +----+    +----+----+                   |
                           |    |         |    |                   |
                           v    v         v    v                   v
      0x01 0x02 0x03 0x04 0x7D 0x5E 0x06 0x7D 0x5D 0x08 0x09 0x0A 0x7E
               

  Using this technique we can clearly recognize the end of a frame by the 0x7E byte while not loosing any available character.

  When receiving data we can decode the 0x7D 0x5x pattern by discarding the 0x7D and doing a xor 0x20 with the following byte (e.g. 0x5E ^ 0x20 = 0x7E).
  This way we get the original 0x7E byte while not confusing it with a delimiter.

      0x01 0x02 0x03 0x04 0x7D 0x5E 0x06 0x7D 0x5D 0x08 0x09 0x0A 0x7E
                           |    |         |    |                   |
                           +----+    +----+----+                   |
                           |         |                             +---> gets dropped
                           v         v                          
      0x01 0x02 0x03 0x04 0x7E 0x06 0x07D 0x08 0x09 0x0A

  After decoding the data stream this way, we get our original bytes back.

*/

auto uart::write(utility::buffer_view buffer) -> void {
  uart_write_bytes(_config.port, buffer.data(), buffer.size());
}

auto uart::read(utility::mutable_buffer_view buffer, const units::milli_seconds& wait) -> std::size_t {
  return uart_read_bytes(_config.port, buffer.data(), buffer.size(), pdMS_TO_TICKS(wait.value()));
}

// auto uart::write(std::span<const std::uint8_t> buffer) -> void {
//   // We send one byte at a time to make encoding easier.

//   for (const auto& byte : buffer) {
//     if (byte == 0x7E) {
//       auto bytes = std::array<std::uint8_t, 2>{0x7D, 0x5E};
//       uart_write_bytes(_config.port, bytes.data(), bytes.size());
//     } else if (byte == 0x7D) {
//       auto bytes = std::array<std::uint8_t, 2>{0x7D, 0x5E};
//       uart_write_bytes(_config.port, bytes.data(), bytes.size());
//     } else {
//       uart_write_bytes(_config.port, &byte, 1u);
//     }
//   }

//   auto delimiter = 0x7E;
//   uart_write_bytes(_config.port, &delimiter, 1u);
// }

// auto uart::read(std::span<std::uint8_t> buffer) -> read_result {
//   while (!_is_synced) {
//     // If we are not synced we discard every byte until the beginning of a new frame
//     auto byte = std::uint8_t{0x00};
//     const auto length = uart_read_bytes(_config.port, &byte, 1u, pdMS_TO_TICKS(0));

//     if (length > 0 && byte == 0x7E) {
//       _is_synced = true;
//     }
//   }

//   // Now we are synced. We still need to decode the 0x7D 0x5E and 0x7D 0x5D bytes.

//   auto i = std::size_t{0u};
//   bool needs_decoding = false;

//   while (true) {
//     if (i > buffer.size()) {
//       // The frame size has exceeded the buffer size. We are considered out of sync again.
//       _is_synced = false;
//       return read_result::frame_overflow;
//     }

//     auto byte = std::uint8_t{0x00};
//     const auto length = uart_read_bytes(_config.port, &byte, 1u, pdMS_TO_TICKS(0));

//     if (length > 0) {
//       if (byte == 0x7E) {
//         break;
//       }

//       if (byte == 0x7D) {
//         needs_decoding = true;
//         continue;
//       }

//       if (needs_decoding) {
//         buffer[i] = byte ^ 0x20;
//         needs_decoding = false;
//       } else {
//         buffer[i] = byte;
//       }

//       ++i;
//     }
//   }

//   if (i < buffer.size()) {
//     // We have encountered a stop byte too early. We are considered out of sync again.
//     _is_synced = false;
//     return read_result::frame_underflow;
//   }

//   return read_result::success;
// }

} // namespace common::drivers
