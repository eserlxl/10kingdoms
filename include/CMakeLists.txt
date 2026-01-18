# Create config.h from template
configure_file(
  ${CMAKE_SOURCE_DIR}/include/config.h.in
  ${CMAKE_BINARY_DIR}/include/config.h
  @ONLY
)

# Install headers (optional, for development)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/
  DESTINATION include/10kingdoms
  FILES_MATCHING PATTERN "*.h"
  PATTERN "config.h" EXCLUDE
  PATTERN "config.h.in" EXCLUDE
)
