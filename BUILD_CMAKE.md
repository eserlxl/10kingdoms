# Building 10kingdoms with CMake

This document describes how to build 10kingdoms using the CMake build system.

## Prerequisites

- CMake 3.20 or later
- C++ compiler with C++11 support (GCC, Clang, or MSVC)
- SDL2 development libraries
- OpenAL development libraries
- enet library (optional, for networking)
- curl library (optional, for web services)
- gettext (optional, for internationalization)
- jwasm (optional, for x86 assembly optimizations)

### Installing Dependencies

#### Linux (Ubuntu/Debian)
```bash
sudo apt-get install cmake build-essential libsdl2-dev libopenal-dev libenet-dev libcurl4-openssl-dev gettext
```

#### Linux (Arch Linux)
```bash
sudo pacman -S cmake base-devel sdl2 openal enet curl gettext
```

#### macOS
```bash
brew install cmake sdl2 openal-soft enet curl gettext
```

#### Windows
- Install CMake from https://cmake.org/download/
- Install SDL2, OpenAL, enet, and curl libraries
- Use vcpkg or MSYS2 for package management

## Build Options

CMake provides several build options:

- `ENABLE_ASM` (default: OFF) - Enable x86 assembly optimizations (requires jwasm)
- `ENABLE_CURL` (default: ON) - Enable curl support for web services
- `ENABLE_ENET` (default: ON) - Enable enet support for networking
- `ENABLE_MULTIPLAYER` (default: ON) - Enable multiplayer support in menu
- `ENABLE_DEBUG` (default: OFF) - Enable debugging features
- `CMAKE_BUILD_TYPE` - Build type: Release, Debug, RelWithDebInfo, MinSizeRel

## Building

### Basic Build

```bash
mkdir build
cd build
cmake ..
make -j20
```

Or use CMake's build command (automatically uses -j20):
```bash
mkdir build
cd build
cmake ..
cmake --build . -j20
```

### Build with Options

```bash
mkdir build
cd build
cmake -DENABLE_ASM=ON -DCMAKE_BUILD_TYPE=Release ..
make -j20
```

### Build without Multiplayer

```bash
mkdir build
cd build
cmake -DENABLE_MULTIPLAYER=OFF ..
make -j20
```

### Build with Assembly Optimizations

```bash
mkdir build
cd build
cmake -DENABLE_ASM=ON ..
make -j20
```

## Installation

After building, install the game:

```bash
make install
```

Or specify a custom installation prefix:

```bash
cmake -DCMAKE_INSTALL_PREFIX=/usr/local ..
make install
```

## Running

The executable will be built in the `build` directory:

```bash
./build/10kingdoms
```

## Troubleshooting

### Missing Dependencies

If CMake reports missing dependencies:

1. Install the required development packages for your system
2. Use `pkg-config` to verify libraries are found:
   ```bash
   pkg-config --modversion sdl2
   pkg-config --modversion openal
   ```

### Assembly Build Issues

If building with `ENABLE_ASM=ON` fails:

1. Ensure `jwasm` is installed and in your PATH
2. Verify jwasm works: `jwasm --version`
3. If jwasm is not available, build without assembly: `-DENABLE_ASM=OFF`

### FPU 387 Support

The game requires 387 FPU support for stable gameplay. If your compiler doesn't support `-mfpmath=387`, the build will continue but multiplayer may be disabled.

## Comparison with Autotools

The CMake build system is designed to produce functionally identical binaries to the autotools build. Key differences:

- CMake uses modern dependency detection
- CMake supports out-of-source builds by default
- CMake provides better IDE integration
- Both build systems can coexist - you can use either one

## Notes

- The CMake build system is parallel to the existing autotools build
- Both build systems should produce equivalent results
- The autotools build remains the primary build system for releases
- CMake build is provided for modern development workflows
