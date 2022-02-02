cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
PROJECT (buildggimg)

set( NAME_SRC
    src/main.c
    src/argparse.c
    src/image.c
    src/rgb.c
    src/gamegear_image_converter.c
)

set( NAME_HEADERS
  src/argparse.h
  src/image.h
  src/rgb.h
  src/gamegear_image_converter.h
)

add_compile_options(
  "-Wall" "-Wextra" "-Wno-missing-field-initializers" "-ggdb3" "-O0"
  "$<$<CONFIG:DEBUG>:-O0;-ggdb3>"
)

add_executable(buildggimg ${NAME_SRC} ${NAME_HEADERS} )

target_link_libraries(buildggimg)
