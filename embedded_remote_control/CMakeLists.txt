cmake_minimum_required(VERSION 3.16.0)
set(EXTRA_COMPONENT_DIRS ${EXTRA_COMPONENT_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/../common)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(embedded_remote_control LANGUAGES C CXX)
