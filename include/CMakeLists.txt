# Install headers (optional, for development)
# Note: config.h is no longer generated for CMake builds - uses compile definitions instead
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/
  DESTINATION include/10kingdoms
  FILES_MATCHING PATTERN "*.h"
  PATTERN "config.h" EXCLUDE
  PATTERN "config.h.in" EXCLUDE
)
