cmake_minimum_required(VERSION 3.2)

project(laserpants_dotenv VERSION 0.9.3 LANGUAGES CXX)

set(DOTENV_CONFIG_INSTALL_DIR "lib/cmake/${PROJECT_NAME}")

option(BUILD_DOCS "Build documentation" ON)

if(BUILD_DOCS)
    find_package(Doxygen)

    if(DOXYGEN_FOUND)
        configure_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in"
            "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
            @ONLY)
        add_custom_target(doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating Doxygen documentation"
            VERBATIM)
    else(DOXYGEN_FOUND)
        message("Doxygen needs to be installed to generate documentation")
    endif(DOXYGEN_FOUND)
endif(BUILD_DOCS)

if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
    set(DEFAULT_BUILD_TYPE "Debug")
else()
    set(DEFAULT_BUILD_TYPE "Release")
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}")
    message(STATUS "Setting build type to ${DEFAULT_BUILD_TYPE}")
endif()

configure_file(
    "${PROJECT_SOURCE_DIR}/src/laserpants_dotenv-config.h.in"
    "${PROJECT_BINARY_DIR}/laserpants_dotenv-config.h" @ONLY)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    laserpants_dotenv-config-version.cmake
    VERSION ${laserpants_dotenv_VERSION}
    COMPATIBILITY AnyNewerVersion)

add_library(dotenv INTERFACE)

target_include_directories(dotenv INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/laserpants/dotenv>
    $<INSTALL_INTERFACE:include/laserpants/dotenv-${laserpants_dotenv_VERSION}>)

install(
    FILES "${PROJECT_BINARY_DIR}/laserpants_dotenv-config.h"
    DESTINATION include/laserpants/dotenv-${laserpants_dotenv_VERSION})

install(
    FILES include/laserpants/dotenv/dotenv.h
    DESTINATION include/laserpants/dotenv-${laserpants_dotenv_VERSION})

install(
    FILES cmake/laserpants_dotenv-config.cmake
    DESTINATION ${DOTENV_CONFIG_INSTALL_DIR})

install(TARGETS dotenv EXPORT dotenv)

install(EXPORT dotenv NAMESPACE laserpants::
    DESTINATION ${DOTENV_CONFIG_INSTALL_DIR})
