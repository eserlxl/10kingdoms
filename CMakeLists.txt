cmake_minimum_required(VERSION 3.20)
project(10kingdoms VERSION 2.15.7 LANGUAGES CXX)

# Set C++ standard
# C++23 requires:
# - GCC 13+ (full support in GCC 13.0)
# - Clang 16+ (full support in Clang 16.0)
# - MSVC 2022 17.5+ (Visual Studio 2022 17.5)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Verify compiler version supports C++23
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "13.0")
        message(FATAL_ERROR "GCC 13.0 or later is required for C++23 support. Found: ${CMAKE_CXX_COMPILER_VERSION}")
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "16.0")
        message(FATAL_ERROR "Clang 16.0 or later is required for C++23 support. Found: ${CMAKE_CXX_COMPILER_VERSION}")
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "19.35")
        message(FATAL_ERROR "MSVC 2022 17.5 (19.35) or later is required for C++23 support. Found: ${CMAKE_CXX_COMPILER_VERSION}")
    endif()
endif()

# Enable parallel builds (equivalent to make -j20)
# This sets the default parallel build level for cmake --build
if(NOT DEFINED CMAKE_BUILD_PARALLEL_LEVEL)
  set(CMAKE_BUILD_PARALLEL_LEVEL 20)
endif()

# Global compiler flags (as list for proper handling)
# Initialize as a list to ensure proper expansion
set(GLOBAL_CFLAGS)

# Check if char is signed (required)
include(CheckCXXSourceCompiles)
set(CMAKE_REQUIRED_FLAGS "${CMAKE_CXX_FLAGS} -fsigned-char")
check_cxx_source_compiles(
  "#include <limits.h>
   #if CHAR_MIN == 0
   #error
   #endif
   int main() { return 0; }"
  CHAR_IS_SIGNED
)
if(NOT CHAR_IS_SIGNED)
  message(FATAL_ERROR "The compiler must interpret char as signed")
endif()
unset(CMAKE_REQUIRED_FLAGS)

# Always add -fsigned-char
list(APPEND GLOBAL_CFLAGS "-fsigned-char")

# Check for 387 FPU support
set(CMAKE_REQUIRED_FLAGS "${CMAKE_CXX_FLAGS} -mfpmath=387")
check_cxx_source_compiles("int main() { return 0; }" FPU_387_SUPPORTED)
unset(CMAKE_REQUIRED_FLAGS)

if(FPU_387_SUPPORTED)
  list(APPEND GLOBAL_CFLAGS "-mfpmath=387" "-ffloat-store")
  set(FPU_TYPE "387")
else()
  set(FPU_TYPE "")
endif()

# Check for register keyword support
check_cxx_source_compiles(
  "int main() { register int x; return 0; }"
  HAVE_REGISTER_KEYWORD
)

# Check for strcasecmp in string.h
include(CheckSymbolExists)
check_symbol_exists(strcasecmp "string.h" HAVE_STRING_STRCASECMP)
if(NOT HAVE_STRING_STRCASECMP)
  check_symbol_exists(strcasecmp "strings.h" HAVE_STRINGS_STRCASECMP)
endif()

# Build options
option(ENABLE_ASM "Enable x86 assembly optimizations" OFF)
option(ENABLE_CURL "Enable curl support" ON)
option(ENABLE_ENET "Enable enet support" ON)
option(ENABLE_MULTIPLAYER "Enable multiplayer support in menu" ON)
option(ENABLE_DEBUG "Enable debugging features" OFF)

# Multiplayer requires enet and 387 FPU
if(ENABLE_MULTIPLAYER)
  if(NOT FPU_387_SUPPORTED)
    message(STATUS "Disabled: Multiplayer requires 387 FPU support")
    set(ENABLE_MULTIPLAYER OFF)
  endif()
  if(NOT ENABLE_ENET)
    message(STATUS "Disabled: Multiplayer requires enet support")
    set(ENABLE_MULTIPLAYER OFF)
  endif()
endif()

# If multiplayer is disabled, disable curl and enet
if(NOT ENABLE_MULTIPLAYER)
  set(ENABLE_CURL OFF)
  set(ENABLE_ENET OFF)
endif()

# Platform detection
if(WIN32)
  set(PLATFORM "windows")
  set(USE_WINDOWS TRUE)
  set(USE_POSIX FALSE)
else()
  set(PLATFORM "posix")
  set(USE_WINDOWS FALSE)
  set(USE_POSIX TRUE)
endif()

# Set register keyword
if(HAVE_REGISTER_KEYWORD)
  set(REGISTER_KEYWORD "register")
else()
  set(REGISTER_KEYWORD "")
endif()

# Configure defines for config.h
if(ENABLE_DEBUG)
  set(DEBUG_DEFINE "#define DEBUG 1")
  set(DYNARRAY_DEBUG_ELEMENT_ACCESS_DEFINE "#define DYNARRAY_DEBUG_ELEMENT_ACCESS 1")
else()
  set(DEBUG_DEFINE "/* #undef DEBUG */")
  set(DYNARRAY_DEBUG_ELEMENT_ACCESS_DEFINE "/* #undef DYNARRAY_DEBUG_ELEMENT_ACCESS */")
endif()

if(ENABLE_MULTIPLAYER)
  set(DISABLE_MULTI_PLAYER_DEFINE "/* #undef DISABLE_MULTI_PLAYER */")
else()
  set(DISABLE_MULTI_PLAYER_DEFINE "#define DISABLE_MULTI_PLAYER 1")
endif()

if(FPU_387_SUPPORTED)
  set(HAVE_KNOWN_BUILD_DEFINE "#define HAVE_KNOWN_BUILD 1")
else()
  set(HAVE_KNOWN_BUILD_DEFINE "/* #undef HAVE_KNOWN_BUILD */")
endif()

if(ENABLE_CURL)
  set(HAVE_LIBCURL_DEFINE "#define HAVE_LIBCURL 1")
else()
  set(HAVE_LIBCURL_DEFINE "/* #undef HAVE_LIBCURL */")
endif()

if(ENABLE_ASM)
  set(USE_ASM_DEFINE "#define USE_ASM 1")
else()
  set(USE_ASM_DEFINE "/* #undef USE_ASM */")
endif()

if(ENABLE_ENET)
  set(USE_ENET_DEFINE "#define USE_ENET 1")
else()
  set(USE_ENET_DEFINE "/* #undef USE_ENET */")
endif()

if(USE_POSIX)
  set(USE_POSIX_DEFINE "#define USE_POSIX 1")
else()
  set(USE_POSIX_DEFINE "/* #undef USE_POSIX */")
endif()

if(USE_WINDOWS)
  set(USE_WINDOWS_DEFINE "#define USE_WINDOWS 1")
else()
  set(USE_WINDOWS_DEFINE "/* #undef USE_WINDOWS */")
endif()

# Dependency detection
# SDL2 (required)
find_package(SDL2 REQUIRED)
if(NOT SDL2_FOUND)
  # Fallback to pkg-config
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(SDL2 REQUIRED sdl2)
endif()

# OpenAL (required for audio)
find_package(OpenAL REQUIRED)
if(NOT OpenAL_FOUND)
  # Fallback to pkg-config
  pkg_check_modules(OPENAL REQUIRED openal)
  set(OpenAL_LIBRARIES ${OPENAL_LIBRARIES})
  set(OpenAL_INCLUDE_DIRS ${OPENAL_INCLUDE_DIRS})
endif()

# enet (optional, for networking)
if(ENABLE_ENET)
  find_package(PkgConfig)
  if(PkgConfig_FOUND)
    pkg_check_modules(ENET libenet)
  endif()
  if(NOT ENET_FOUND)
    # Try to find enet manually
    find_library(ENET_LIBRARIES NAMES enet)
    if(ENET_LIBRARIES)
      set(ENET_FOUND TRUE)
      set(ENET_LIBRARIES ${ENET_LIBRARIES})
      if(WIN32)
        set(ENET_LIBRARIES ${ENET_LIBRARIES} ws2_32 winmm)
      endif()
    else()
      message(FATAL_ERROR "enet library not found. Install libenet or disable with -DENABLE_ENET=OFF")
    endif()
  endif()
endif()

# curl (optional, for web services)
if(ENABLE_CURL)
  find_package(CURL REQUIRED)
  if(NOT CURL_FOUND)
    # Fallback to pkg-config
    pkg_check_modules(CURL REQUIRED libcurl)
    set(CURL_LIBRARIES ${CURL_LIBRARIES})
    set(CURL_INCLUDE_DIRS ${CURL_INCLUDE_DIRS})
  endif()
endif()

# Gettext/NLS - check if available
find_package(Gettext)
if(Gettext_FOUND)
  set(ENABLE_NLS_DEFINE "#define ENABLE_NLS 1")
  find_library(INTL_LIBRARY NAMES intl)
  find_library(ICONV_LIBRARY NAMES iconv)
else()
  set(ENABLE_NLS_DEFINE "/* #undef ENABLE_NLS */")
endif()

# Set strcasecmp defines
if(HAVE_STRING_STRCASECMP)
  set(HAVE_STRING_STRCASECMP_DEFINE "#define HAVE_STRING_STRCASECMP 1")
  set(HAVE_STRINGS_STRCASECMP_DEFINE "/* #undef HAVE_STRINGS_STRCASECMP */")
elseif(HAVE_STRINGS_STRCASECMP)
  set(HAVE_STRING_STRCASECMP_DEFINE "/* #undef HAVE_STRING_STRCASECMP */")
  set(HAVE_STRINGS_STRCASECMP_DEFINE "#define HAVE_STRINGS_STRCASECMP 1")
else()
  set(HAVE_STRING_STRCASECMP_DEFINE "/* #undef HAVE_STRING_STRCASECMP */")
  set(HAVE_STRINGS_STRCASECMP_DEFINE "/* #undef HAVE_STRINGS_STRCASECMP */")
endif()

# Windows-specific libraries
if(WIN32)
  set(PLATFORM_LIBS ole32 msvcrt winmm)
  if(NOT ENABLE_DEBUG)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
  endif()
endif()

# Assembly support
if(ENABLE_ASM)
  find_program(JWASM_CMD jwasm)
  if(NOT JWASM_CMD)
    message(FATAL_ERROR "jwasm assembler not found. Install jwasm or disable assembly with -DENABLE_ASM=OFF")
  endif()
  if(WIN32)
    set(JWASM_FMT "-coff")
  else()
    set(JWASM_FMT "-elf -zt0")
  endif()
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_BINARY_DIR}/include)

# Add subdirectories
add_subdirectory(include)
add_subdirectory(src)

# Data directory
install(DIRECTORY data DESTINATION share/10kingdoms)

# Documentation
install(FILES COPYING README DESTINATION share/doc/10kingdoms)
