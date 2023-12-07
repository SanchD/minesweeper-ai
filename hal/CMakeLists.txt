cmake_minimum_required (VERSION 3.12)

add_subdirectory(gpio)
add_subdirectory(uart)

target_include_directories(${TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
