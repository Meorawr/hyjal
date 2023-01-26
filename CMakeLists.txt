# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.24)

project(
  hyjal
  VERSION 0.0.1
  DESCRIPTION "World of Warcraft Frame API Library"
  HOMEPAGE_URL "https://github.com/Meorawr/hyjal"
  LANGUAGES CXX
)

list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(CMakeDependentOption)
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

option(BUILD_SHARED_LIBS "Build components as shared libraries?" OFF)
option(BUILD_TESTING "Build test executables?" ${PROJECT_IS_TOP_LEVEL})
option(BUILD_INSTALL "Enable the generation of installation targets?" ${PROJECT_IS_TOP_LEVEL})
cmake_dependent_option(BUILD_PACKAGE "Enable the generation of packaging targets?" ${PROJECT_IS_TOP_LEVEL} "BUILD_INSTALL" OFF)

option(CMAKE_BUILD_RPATH_USE_ORIGIN "Use relative paths for build rpath?" ON)
option(CMAKE_CXX_EXTENSIONS "Allow the use of non-standard CXX compiler extensions?" OFF)
option(CMAKE_CXX_STANDARD_REQUIRED "Require support for requested C++ compiler standards?" ON)
option(CMAKE_VISIBILITY_INLINES_HIDDEN "Hide symbols for inline function definitions?" ON)

set(CMAKE_CXX_VISIBILITY_PRESET hidden CACHE STRING "Default visibility level for symbols")
set(CMAKE_INSTALL_RPATH "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}" CACHE STRING "The rpath to use for installed targets")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin" CACHE PATH "Output directory for runtime outputs (executables and dlls)")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib" CACHE PATH "Output directory for static library outputs")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib" CACHE PATH "Output directory for library outputs")
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} CACHE PATH "Output directory for debug symbol outputs")

set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<CONFIG:Debug>,EditAndContinue,ProgramDatabase>" CACHE STRING "Debug information format for MSVC compilations")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL" CACHE STRING "Runtime library to link against for MSVC compilations")

find_package(Boost REQUIRED)
find_package(elune CONFIG REQUIRED COMPONENTS liblua)
find_package(fmt CONFIG REQUIRED)

##
## Configuration Target
##

add_library(hyjal_config INTERFACE)
add_library(hyjal::config ALIAS hyjal_config)

target_compile_definitions(
  hyjal_config
  INTERFACE
    $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
    $<$<PLATFORM_ID:Windows>:UNICODE>
    $<$<PLATFORM_ID:Windows>:_UNICODE>
)

target_compile_features(
  hyjal_config
  INTERFACE
    cxx_std_20
)

target_compile_options(
  hyjal_config
  INTERFACE
    $<$<CXX_COMPILER_ID:MSVC>:/EHsc>
    $<$<CXX_COMPILER_ID:MSVC>:/fp:fast>
    $<$<CXX_COMPILER_ID:MSVC>:/utf-8>
    $<$<CXX_COMPILER_ID:MSVC>:/Zc:__cplusplus>
    $<$<CXX_COMPILER_ID:MSVC>:/Zc:inline>
    $<$<CXX_COMPILER_ID:MSVC>:/Zc:throwingNew>
    $<$<CXX_COMPILER_ID:GNU,Clang>:-ffast-math>
)

target_link_options(
  hyjal_config
  INTERFACE
    $<$<CXX_COMPILER_ID:MSVC>:/MANIFEST:NO>
    $<$<CXX_COMPILER_ID:MSVC>:/PDBALTPATH:%_PDB%>
)

##
## Library Target
##

add_library(hyjal)
add_library(hyjal::hyjal ALIAS hyjal)

set_target_properties(
  hyjal
  PROPERTIES
    DEFINE_SYMBOL HYJAL_BUILD_SHARED
)

target_compile_definitions(
  hyjal
  INTERFACE
    $<$<STREQUAL:$<TARGET_PROPERTY:hyjal,TYPE>,SHARED_LIBRARY>:HYJAL_USE_SHARED>
)

target_include_directories(
  hyjal
  INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  PRIVATE
    "${PROJECT_SOURCE_DIR}/include/meorawr/hyjal"
)

target_link_libraries(
  hyjal
  PRIVATE
    hyjal::config
    Boost::boost
    elune::liblua
    fmt::fmt
)

target_sources(
  hyjal
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      "include"
    FILES
      "include/meorawr/hyjal/hyjal.hpp"
  PRIVATE
    "src/hyjal.cpp"
    "src/uiframe.cpp"
    "src/uiframe.hpp"
    "src/uiobject.cpp"
    "src/uiobject.hpp"
    "src/uistate.cpp"
    "src/uistate.hpp"
)

##
## Installation and Packaging
##

if(BUILD_INSTALL)
  install(
    TARGETS hyjal
    EXPORT hyjal-targets
    RUNTIME
      DESTINATION ${CMAKE_INSTALL_BINDIR}
      COMPONENT Runtime
    LIBRARY
      DESTINATION ${CMAKE_INSTALL_LIBDIR}
      COMPONENT Runtime
    ARCHIVE
      DESTINATION ${CMAKE_INSTALL_LIBDIR}
      COMPONENT Development
    FILE_SET HEADERS
      DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
      COMPONENT Development
  )

  install(
    EXPORT hyjal-targets
    FILE hyjal-targets.cmake
    NAMESPACE meorawr::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/hyjal"
    COMPONENT Development
  )

  write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/hyjal-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY ExactVersion
  )

  configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/hyjal-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/hyjal-config.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/hyjal"
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
  )

  install(
    FILES
      "${CMAKE_CURRENT_BINARY_DIR}/hyjal-config-version.cmake"
      "${CMAKE_CURRENT_BINARY_DIR}/hyjal-config.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/hyjal"
    COMPONENT Development
  )
endif()

if(BUILD_PACKAGE)
  set(CPACK_PACKAGE_VENDOR "Meorawr")
  string(TOLOWER "${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}" CPACK_SYSTEM_NAME)
  set(CPACK_GENERATOR "TXZ;ZIP")
  set(CPACK_STRIP_FILES ON)

  include(CPack)
endif()
