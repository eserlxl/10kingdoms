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

# Collect compile definitions (replaces config.h for CMake builds)
set(PROJECT_COMPILE_DEFINITIONS)

# Debug defines
if(ENABLE_DEBUG)
  list(APPEND PROJECT_COMPILE_DEFINITIONS DEBUG=1 DYNARRAY_DEBUG_ELEMENT_ACCESS=1)
endif()

# Multiplayer
if(NOT ENABLE_MULTIPLAYER)
  list(APPEND PROJECT_COMPILE_DEFINITIONS DISABLE_MULTI_PLAYER=1)
endif()

# FPU support
if(FPU_387_SUPPORTED)
  list(APPEND PROJECT_COMPILE_DEFINITIONS HAVE_KNOWN_BUILD=1)
endif()

# Feature flags
if(ENABLE_CURL)
  list(APPEND PROJECT_COMPILE_DEFINITIONS HAVE_LIBCURL=1)
endif()

if(ENABLE_ASM)
  list(APPEND PROJECT_COMPILE_DEFINITIONS USE_ASM=1)
endif()

if(ENABLE_ENET)
  list(APPEND PROJECT_COMPILE_DEFINITIONS USE_ENET=1)
endif()

# Platform defines
if(USE_POSIX)
  list(APPEND PROJECT_COMPILE_DEFINITIONS USE_POSIX=1)
endif()

if(USE_WINDOWS)
  list(APPEND PROJECT_COMPILE_DEFINITIONS USE_WINDOWS=1)
endif()

# Required dependencies
list(APPEND PROJECT_COMPILE_DEFINITIONS USE_OPENAL=1 USE_SDL=1)

# Memory management
list(APPEND PROJECT_COMPILE_DEFINITIONS NO_MEM_CLASS=1)

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

# Define OpenAL header paths (required for openal_audio.h)
# These are string macros used in #include directives - need special handling
# Store as separate variables for use with target_compile_options
if(EXISTS "${OpenAL_INCLUDE_DIRS}/AL/al.h")
  set(OPENAL_AL_H_VALUE "<AL/al.h>")
  set(OPENAL_ALC_H_VALUE "<AL/alc.h>")
elseif(EXISTS "${OpenAL_INCLUDE_DIRS}/OpenAL/al.h")
  set(OPENAL_AL_H_VALUE "<OpenAL/al.h>")
  set(OPENAL_ALC_H_VALUE "<OpenAL/alc.h>")
else()
  # Default to AL/al.h format (most common)
  set(OPENAL_AL_H_VALUE "<AL/al.h>")
  set(OPENAL_ALC_H_VALUE "<AL/alc.h>")
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
  list(APPEND PROJECT_COMPILE_DEFINITIONS ENABLE_NLS=1)
  find_library(INTL_LIBRARY NAMES intl)
  find_library(ICONV_LIBRARY NAMES iconv)
endif()

# Set strcasecmp defines
if(HAVE_STRING_STRCASECMP)
  list(APPEND PROJECT_COMPILE_DEFINITIONS HAVE_STRING_STRCASECMP=1)
elseif(HAVE_STRINGS_STRCASECMP)
  list(APPEND PROJECT_COMPILE_DEFINITIONS HAVE_STRINGS_STRCASECMP=1)
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

# Add subdirectories (no longer need include/CMakeLists.txt for config.h)
add_subdirectory(src)

# Data directory
install(DIRECTORY data DESTINATION share/10kingdoms)

# Documentation
install(FILES COPYING README.md DESTINATION share/doc/10kingdoms)
