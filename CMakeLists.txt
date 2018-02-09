cmake_minimum_required(VERSION 3.5.1)
project(emoji)

set(CMAKE_VERBOSE_MAKEFILE 1)


# C++
#~~~~~~~
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0 -pg")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -s -DNDEBUG -march=native")

set(EXECUTABLE_OUTPUT_PATH "bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  "obj")
include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/externals/skia/out/Static)


# emoji
#~~~~~~~~
add_library(emoji STATIC
  src/emoji.cpp
)
add_dependencies(emoji skia)
target_link_libraries(emoji skia)


# example
#~~~~~~~~~~
add_executable(example
  example/main.c
)
add_dependencies(example emoji)
target_link_libraries(example emoji)


# Skia
#~~~~~~~~
get_filename_component(gn_path externals/depot_tools/gn ABSOLUTE)
get_filename_component(ninja_path externals/depot_tools/ninja ABSOLUTE)

add_custom_target(skia
  SOURCES externals/skia/out/Static/libskia.a
)
add_custom_command(
  OUTPUT externals/skia/out/Static/libskia.a
  COMMAND python tools/git-sync-deps
  COMMAND ${gn_path} gen out/Static --args='is_debug=false　target_cpu="x64" is_official_build=true skia_use_system_libjpeg_turbo=false skia_use_system_libpng=false skia_use_system_libwebp=false skia_use_system_icu=false'
  COMMAND ${ninja_path} -C out/Static
  WORKING_DIRECTORY externals/skia
)

# vim: se sw=2 ts=2 sts=2 et ft=cmake :
