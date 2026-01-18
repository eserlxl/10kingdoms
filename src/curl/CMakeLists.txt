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
  "-include" "${CMAKE_BINARY_DIR}/include/config.h"
)

# Link curl to the static library (needed for symbols)
target_link_libraries(WebService PUBLIC ${CURL_LIBRARIES})
