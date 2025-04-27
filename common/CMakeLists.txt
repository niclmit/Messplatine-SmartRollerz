set(
  SOURCES
    # Headers
    ${CMAKE_CURRENT_SOURCE_DIR}/include/common/drivers/servo.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/common/drivers/drivers.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/common/drivers/gpio.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/common/drivers/i2c.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/common/drivers/uart.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/common/drivers/pwm.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/common/utility/string_literal.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/common/utility/enum.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/common/utility/layout.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/common/utility/type.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/common/utility/mutex.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/common/utility/buffer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/common/utility/utility.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/common/units/quantity.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/common/units/frequency.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/common/units/angle.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/common/units/data.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/common/units/units.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/common/logger.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/common/task.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/common/common.hpp
    # Sources
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/servo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/gpio.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/i2c.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/uart.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/pwm.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utility/mutex.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utility/format.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/task.cpp
)

idf_component_register(
  SRCS ${SOURCES}
  INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
  REQUIRES driver
)
