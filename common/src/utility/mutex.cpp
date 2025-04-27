#include <common/utility/mutex.hpp>

namespace common::utility {

// mutex::mutex()
// : _handle{xSemaphoreCreateMutexStatic(&_buffer)} {

// }

// mutex::~mutex() {
//   // vSemaphoreDelete(_handle);
// }

// auto mutex::give() -> void {
//   xSemaphoreGive(_handle);
// }

// auto mutex::take() -> void {
//   xSemaphoreTake(_handle, portMAX_DELAY);
// }

} // namespace common::utility
