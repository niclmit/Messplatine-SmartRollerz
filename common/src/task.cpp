#include <common/task.hpp>

#include <common/logger.hpp>

namespace common {

auto task::delay(const common::units::milli_seconds& delay) -> void {
  vTaskDelay(pdMS_TO_TICKS(delay.value()));
}

auto task::handle() const noexcept -> handle_type {
  return _handle;
}

} // namespace common
