cmake_minimum_required(VERSION 3.28)
project(paktool)

if(NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    message(FATAL_ERROR "MSVC is required to build this project.")
endif()

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_subdirectory(vendor/libpeggle)

set(vendor_sources
        vendor/CLI11.hpp  # https://github.com/CLIUtils/CLI11/releases/tag/v2.4.2
)

set(target_sources
        main.cpp
        logma.cpp
)
set(target_headers
        logma.h
)

add_executable(${PROJECT_NAME}
        ${target_sources}
        ${target_headers}
        ${vendor_sources}
)
add_dependencies(${PROJECT_NAME} libpeggle)
target_link_libraries(${PROJECT_NAME} libpeggle)

