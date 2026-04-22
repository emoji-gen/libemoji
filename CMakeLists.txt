cmake_minimum_required(VERSION 3.10.0)
project(emoji)


# Common
#~~~~~~~~~
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0 -pg")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -s -DNDEBUG -march=native")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(EXECUTABLE_OUTPUT_PATH "bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  "tmp")
include_directories(
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/externals/skia
  ${CMAKE_SOURCE_DIR}/externals/skia/include
)
link_directories(
  ${CMAKE_SOURCE_DIR}/tmp
)

# emoji
#~~~~~~~~
file(GLOB emoji_files "src/*.cpp")
add_library(emoji STATIC
  ${emoji_files}
)
add_dependencies(emoji skia)
target_link_libraries(emoji skia z)
set_property(TARGET emoji PROPERTY POSITION_INDEPENDENT_CODE ON)

if(APPLE)
  target_compile_definitions(emoji PRIVATE EG_OS_MAC)
elseif(UNIX)
  target_compile_definitions(emoji PRIVATE EG_OS_LINUX)
endif()

#~~~~~~~

add_custom_target(emoji_combined
  DEPENDS emoji
  SOURCES lib/libemoji.a
)
add_custom_command(
  OUTPUT lib/libemoji.a
  COMMAND ${CMAKE_COMMAND} -E remove -f *.o
  COMMAND ar -x libemoji.a
  COMMAND ar -x libskia.a
  COMMAND ar -qcs libemoji_combined.a *.o
  COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/tmp/libemoji_combined.a
    ${CMAKE_SOURCE_DIR}/lib/libemoji.a
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tmp
)


# example
#~~~~~~~~~~
file(GLOB example_files "example/*.c")
add_executable(example
  ${example_files}
)
add_dependencies(example emoji_combined)

if(APPLE)
  target_link_libraries(example
    emoji
    "-framework CoreFoundation"
    "-framework CoreGraphics"
    "-framework CoreText"
    "-framework CoreServices"
  )
else()
  set(THREADS_PREFER_PTHREAD_FLAG TRUE)
  find_package(Threads REQUIRED)
  find_package(OpenGL REQUIRED)

  target_link_libraries(example
    emoji
    ${OPENGL_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
  )
endif()


# Skia
#~~~~~~~~
get_filename_component(ninja_bin externals/skia/third_party/ninja/ninja ABSOLUTE)

set(gn_args
  target_cpu="${CMAKE_SYSTEM_PROCESSOR}"
  is_debug=false
  is_official_build=true
  skia_use_fontconfig=false
  skia_use_system_libpng=false
  skia_use_system_libwebp=false
  skia_use_system_icu=false
  skia_use_system_harfbuzz=false
  skia_use_system_libjpeg_turbo=false
)

if(NOT APPLE)
  list(APPEND gn_args
    skia_use_system_freetype2=false
  )
endif()

add_custom_target(skia
  SOURCES tmp/libskia.a
)
add_custom_command(
  OUTPUT tmp/libskia.a
  COMMAND ${CMAKE_COMMAND} -E rm -f ${ninja_bin}
  COMMAND python3 tools/git-sync-deps
  COMMAND python3 bin/fetch-ninja
  COMMAND bin/gn gen out/Static --args='${gn_args}'
  COMMAND ${ninja_bin} -C out/Static
  COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/externals/skia/out/Static/libskia.a
    ${CMAKE_SOURCE_DIR}/tmp/libskia.a
  WORKING_DIRECTORY externals/skia
)

# vim: se sw=2 ts=2 sts=2 et ft=cmake :
