cmake_minimum_required(VERSION 3.20)

project(cnerium_middleware
    VERSION 0.2.0
    DESCRIPTION "Cnerium Middleware - Composable middleware system for the Cnerium web framework"
    LANGUAGES CXX
)

include(GNUInstallDirs)

# ========================
# Options
# ========================
option(CNERIUM_MIDDLEWARE_BUILD_TESTS "Build middleware tests" OFF)
option(CNERIUM_MIDDLEWARE_BUILD_EXAMPLES "Build middleware examples" OFF)

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

if(NOT TARGET cnerium::http
   AND NOT TARGET cnerium_http
   AND NOT TARGET http::http
   AND NOT TARGET http)
    find_package(cnerium_http CONFIG QUIET)
    find_package(http CONFIG QUIET)
endif()

if(NOT TARGET cnerium::json
   AND NOT TARGET cnerium_json
   AND NOT TARGET json::json
   AND NOT TARGET json)
    find_package(cnerium_json CONFIG QUIET)
    find_package(json CONFIG QUIET)
endif()

set(CNERIUM_MIDDLEWARE_HTTP_TARGET "")
set(CNERIUM_MIDDLEWARE_JSON_TARGET "")

if(TARGET cnerium::http)
    set(CNERIUM_MIDDLEWARE_HTTP_TARGET cnerium::http)
elseif(TARGET cnerium_http)
    set(CNERIUM_MIDDLEWARE_HTTP_TARGET cnerium_http)
elseif(TARGET http::http)
    set(CNERIUM_MIDDLEWARE_HTTP_TARGET http::http)
elseif(TARGET http)
    set(CNERIUM_MIDDLEWARE_HTTP_TARGET http)
endif()

if(TARGET cnerium::json)
    set(CNERIUM_MIDDLEWARE_JSON_TARGET cnerium::json)
elseif(TARGET cnerium_json)
    set(CNERIUM_MIDDLEWARE_JSON_TARGET cnerium_json)
elseif(TARGET json::json)
    set(CNERIUM_MIDDLEWARE_JSON_TARGET json::json)
elseif(TARGET json)
    set(CNERIUM_MIDDLEWARE_JSON_TARGET json)
endif()

if(CNERIUM_MIDDLEWARE_HTTP_TARGET STREQUAL "")
    message(FATAL_ERROR
        "cnerium http target not found.\n"
        "Install dependencies with Vix:\n"
        "  vix add @cnerium/http\n"
        "  vix install"
    )
endif()

if(CNERIUM_MIDDLEWARE_JSON_TARGET STREQUAL "")
    message(FATAL_ERROR
        "cnerium json target not found.\n"
        "Install dependencies with Vix:\n"
        "  vix add @cnerium/json\n"
        "  vix install"
    )
endif()

# ========================
# Library (header-only)
# ========================
add_library(cnerium_middleware INTERFACE)
add_library(cnerium::middleware ALIAS cnerium_middleware)

target_compile_features(cnerium_middleware INTERFACE cxx_std_20)

target_link_libraries(cnerium_middleware
    INTERFACE
        ${CNERIUM_MIDDLEWARE_HTTP_TARGET}
        ${CNERIUM_MIDDLEWARE_JSON_TARGET}
)

target_include_directories(cnerium_middleware
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# ========================
# Compiler options
# ========================
if(MSVC)
    target_compile_options(cnerium_middleware INTERFACE /W4 /permissive-)
else()
    target_compile_options(cnerium_middleware INTERFACE -Wall -Wextra -Wpedantic)
endif()

# ========================
# Target properties
# ========================
set_target_properties(cnerium_middleware PROPERTIES
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
        add_executable(cnerium_middleware_test_basic tests/test_basic.cpp)
        target_link_libraries(cnerium_middleware_test_basic PRIVATE cnerium::middleware)
        target_compile_features(cnerium_middleware_test_basic PRIVATE cxx_std_20)

        if(MSVC)
            target_compile_options(cnerium_middleware_test_basic PRIVATE /W4 /permissive-)
        else()
            target_compile_options(cnerium_middleware_test_basic PRIVATE -Wall -Wextra -Wpedantic)
        endif()

        add_test(NAME cnerium_middleware.basic COMMAND cnerium_middleware_test_basic)
    endif()
endif()

# ========================
# Summary
# ========================
message(STATUS "------------------------------------------------------")
message(STATUS "cnerium::middleware configured (${PROJECT_VERSION})")
message(STATUS "HTTP target:     ${CNERIUM_MIDDLEWARE_HTTP_TARGET}")
message(STATUS "JSON target:     ${CNERIUM_MIDDLEWARE_JSON_TARGET}")
message(STATUS "Include dir:     ${CMAKE_CURRENT_SOURCE_DIR}/include")
message(STATUS "Build tests:     ${CNERIUM_MIDDLEWARE_BUILD_TESTS}")
message(STATUS "Build examples:  ${CNERIUM_MIDDLEWARE_BUILD_EXAMPLES}")
message(STATUS "Build type:      ${CMAKE_BUILD_TYPE}")
message(STATUS "------------------------------------------------------")
