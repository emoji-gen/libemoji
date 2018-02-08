cmake_minimum_required(VERSION 3.10.2)

get_filename_component(gn_path externals/depot_tools/gn ABSOLUTE)
get_filename_component(ninja_path externals/depot_tools/ninja ABSOLUTE)

# Skia
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
