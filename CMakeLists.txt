cmake_minimum_required(VERSION 3.16)
project(Constitution LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB CONSTITUTION_C_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/c/*.c"
)

file(GLOB CONSTITUTION_CPP_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/*.cpp"
)

add_executable(Constitution
    ${CONSTITUTION_C_SOURCES}
    ${CONSTITUTION_CPP_SOURCES}
)

target_include_directories(Constitution PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/c"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/c"
)

if (MSVC)
    message(FATAL_ERROR "This project is tuned for GNU/Clang toolchains, not MSVC.")
endif()

target_compile_options(Constitution PRIVATE -Wall -Wextra -Wpedantic)
