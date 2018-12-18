cmake_minimum_required(VERSION 3.2)

project(dotenv VERSION 0.9.0 LANGUAGES CXX)

if(EXISTS "${CMAKE_SOURCE_DIR}/.git") 
    set(default_build_type "Debug")
else()
    set(default_build_type "Release")
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to ${default_build_type}.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE FORCE)
endif()

configure_file(
    "${PROJECT_SOURCE_DIR}/src/dotenv-config.h.in"
    "${PROJECT_BINARY_DIR}/dotenv-config.h"
    @ONLY)

configure_file(
    "${PROJECT_SOURCE_DIR}/src/dotenv-config-version.cmake.in"
    "${PROJECT_BINARY_DIR}/dotenv-config-version.cmake"
    @ONLY)

add_library(dotenv INTERFACE)

target_include_directories(dotenv INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/laserpants/dotenv>
    $<INSTALL_INTERFACE:include/laserpants/dotenv-${dotenv_VERSION}>)

install(
    FILES "${PROJECT_BINARY_DIR}/dotenv-config.h" 
    DESTINATION include/laserpants/dotenv-${dotenv_VERSION})

install(
    FILES include/laserpants/dotenv/dotenv.h 
    DESTINATION include/laserpants/dotenv-${dotenv_VERSION})

install(
    FILES cmake/dotenv-config.cmake ${CMAKE_CURRENT_BINARY_DIR}/dotenv-config-version.cmake 
    DESTINATION lib/laserpants/dotenv-${dotenv_VERSION})
