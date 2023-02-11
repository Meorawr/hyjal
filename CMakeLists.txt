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
    $<$<NOT:$<BOOL:BUILD_TESTING>>:DOCTEST_CONFIG_DISABLE>
)

target_compile_features(
  hyjal_config
  INTERFACE
    cxx_std_23
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

add_library(hyjal SHARED)
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
    "src/animatable_object.cpp"
    "src/animatable_object.hpp"
    "src/animation.cpp"
    "src/animation.hpp"
    "src/animation_group.cpp"
    "src/animation_group.hpp"
    "src/child_list.hpp"
    "src/child_list_hook.hpp"
    "src/child_list_node.hpp"
    "src/child_list_traits.hpp"
    "src/child_list_value_traits.hpp"
    "src/child_list_view.hpp"
    "src/detail/concepts.hpp"
    "src/detail/type_list.hpp"
    "src/font.cpp"
    "src/font.hpp"
    "src/frame.cpp"
    "src/frame.hpp"
    "src/hyjal.cpp"
    "src/intrusive_list_node.hpp"
    "src/lua/index.hpp"
    "src/lua/pseudo_index.hpp"
    "src/lua/raw_index.hpp"
    "src/lua/stack_algorithms.hpp"
    "src/lua/stack_index.hpp"
    "src/lua/stack_iterator.hpp"
    "src/lua/stack_reference.hpp"
    "src/lua/state.hpp"
    "src/lua/state_deleter.cpp"
    "src/lua/state_deleter.hpp"
    "src/lua/type.hpp"
    "src/lua/upvalue_index.hpp"
    "src/lua/value.hpp"
    "src/object.cpp"
    "src/object.hpp"
    "src/object_base.hpp"
    "src/object_list.hpp"
    "src/object_list_hook.hpp"
    "src/object_list_node.hpp"
    "src/object_list_traits.hpp"
    "src/object_list_value_traits.hpp"
    "src/object_list_view.hpp"
    "src/region.cpp"
    "src/region.hpp"
    "src/script_util.cpp"
    "src/script_util.hpp"
    "src/ui_service.cpp"
    "src/ui_service.hpp"
    "src/ui_service_script.cpp"
    "src/ui_service_script.hpp"
    "src/ui_type_id.hpp"
    "src/ui_type_info.hpp"
    "src/ui_type_traits.hpp"
)

##
## Test Runner Target
##

if(BUILD_TESTING)
  find_package(doctest CONFIG REQUIRED)

  add_executable(hyjal_test)
  add_executable(hyjal::test ALIAS hyjal_test)

  target_link_libraries(
    hyjal_test
    PRIVATE
      doctest::doctest
      $<TARGET_OBJECTS:hyjal::hyjal>
      $<TARGET_PROPERTY:hyjal::hyjal,LINK_LIBRARIES>
  )

  #DOCTEST_CONFIG_IMPLEMENT

  target_include_directories(
    hyjal_test
    PRIVATE
      $<TARGET_PROPERTY:hyjal::hyjal,INCLUDE_DIRECTORIES>
  )

  target_sources(
    hyjal_test
    PRIVATE
      "test/debug_memory_listener.cpp"
      "test/debug_memory_listener.hpp"
      "test/debug_memory_resource.cpp"
      "test/debug_memory_resource.hpp"
      "test/main.cpp"
      "test/test_util.cpp"
      "test/test_util.hpp"
      "test/ui_service_test.cpp"
  )

  set_property(
    SOURCE "test/main.cpp"
    APPEND PROPERTY COMPILE_DEFINITIONS DOCTEST_CONFIG_IMPLEMENT
  )

  include(CTest)
  include("${doctest_DIR}/doctest.cmake")
  doctest_discover_tests(hyjal_test)
endif()

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
