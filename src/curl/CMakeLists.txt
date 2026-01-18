# curl WebService library
add_library(WebService STATIC
  WebService.cpp
)

target_include_directories(WebService PRIVATE
  ${CMAKE_SOURCE_DIR}/include
  ${CURL_INCLUDE_DIRS}
)

target_compile_options(WebService PRIVATE
  ${GLOBAL_CFLAGS}
)

# Compile definitions (replaces config.h)
target_compile_definitions(WebService PRIVATE
  ${PROJECT_COMPILE_DEFINITIONS}
)

# Link curl to the static library (needed for symbols)
target_link_libraries(WebService PUBLIC ${CURL_LIBRARIES})
