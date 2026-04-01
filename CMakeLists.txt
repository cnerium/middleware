cmake_minimum_required(VERSION 3.20)

project(cnerium_middleware
    VERSION 0.1.0
    DESCRIPTION "Cnerium Middleware - Composable middleware system for the Cnerium web framework"
    LANGUAGES CXX
)

include(GNUInstallDirs)

# ========================
# Options
# ========================
option(CNERIUM_MIDDLEWARE_BUILD_TESTS "Build middleware tests" ON)
option(CNERIUM_MIDDLEWARE_BUILD_EXAMPLES "Build middleware examples" ON)

# ========================
# Global settings
# ========================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ========================
# Dependencies
# ========================
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.vix/vix_deps.cmake")
    include("${CMAKE_CURRENT_SOURCE_DIR}/.vix/vix_deps.cmake")
endif()

find_package(cnerium_http CONFIG QUIET)
find_package(http CONFIG QUIET)

set(CNERIUM_MIDDLEWARE_HTTP_TARGET "")

if(TARGET http::http)
    set(CNERIUM_MIDDLEWARE_HTTP_TARGET http::http)
elseif(TARGET http)
    set(CNERIUM_MIDDLEWARE_HTTP_TARGET http)
elseif(TARGET cnerium::http)
    set(CNERIUM_MIDDLEWARE_HTTP_TARGET cnerium::http)
elseif(TARGET cnerium_http)
    set(CNERIUM_MIDDLEWARE_HTTP_TARGET cnerium_http)
endif()

if(CNERIUM_MIDDLEWARE_HTTP_TARGET STREQUAL "")
    message(FATAL_ERROR
        "cnerium http target not found.\n"
        "Tried: http::http, http, cnerium::http, cnerium_http\n"
        "Install dependencies with Vix:\n"
        "  vix add @cnerium/http\n"
        "  vix install"
    )
endif()

# ========================
# Library (header-only)
# ========================
add_library(middleware INTERFACE)
add_library(middleware::middleware ALIAS middleware)

target_compile_features(middleware INTERFACE cxx_std_20)

target_link_libraries(middleware
    INTERFACE
        ${CNERIUM_MIDDLEWARE_HTTP_TARGET}
)

target_include_directories(middleware INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# ========================
# Compiler options
# ========================
if(MSVC)
    target_compile_options(middleware INTERFACE /W4 /permissive-)
else()
    target_compile_options(middleware INTERFACE -Wall -Wextra -Wpedantic)
endif()

# ========================
# Target properties
# ========================
set_target_properties(middleware PROPERTIES
    EXPORT_NAME middleware
)

# ========================
# Examples
# ========================
if(CNERIUM_MIDDLEWARE_BUILD_EXAMPLES AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples/CMakeLists.txt")
    add_subdirectory(examples)
endif()

# ========================
# Tests
# ========================
if(CNERIUM_MIDDLEWARE_BUILD_TESTS)
    include(CTest)
    enable_testing()

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_basic.cpp")
        add_executable(middleware_basic_test tests/test_basic.cpp)
        target_link_libraries(middleware_basic_test PRIVATE middleware::middleware)
        add_test(NAME middleware.basic COMMAND middleware_basic_test)
    endif()
endif()

# ========================
# Summary
# ========================
message(STATUS "------------------------------------------------------")
message(STATUS "cnerium middleware configured (${PROJECT_VERSION})")
message(STATUS "HTTP target:     ${CNERIUM_MIDDLEWARE_HTTP_TARGET}")
message(STATUS "Include dir:     ${CMAKE_CURRENT_SOURCE_DIR}/include")
message(STATUS "Build tests:     ${CNERIUM_MIDDLEWARE_BUILD_TESTS}")
message(STATUS "Build examples:  ${CNERIUM_MIDDLEWARE_BUILD_EXAMPLES}")
message(STATUS "Build type:      ${CMAKE_BUILD_TYPE}")
message(STATUS "------------------------------------------------------")
