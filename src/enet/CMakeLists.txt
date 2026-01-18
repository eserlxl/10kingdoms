# enet multiplayer library
add_library(multiplayer STATIC
  multiplayer.cpp
)

target_include_directories(multiplayer PRIVATE
  ${CMAKE_SOURCE_DIR}/include
  ${ENET_INCLUDE_DIRS}
)

target_compile_options(multiplayer PRIVATE
  ${GLOBAL_CFLAGS}
)

# Compile definitions (replaces config.h)
target_compile_definitions(multiplayer PRIVATE
  ${PROJECT_COMPILE_DEFINITIONS}
)
