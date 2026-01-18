# Building 10kingdoms with CMake

This document describes how to build 10kingdoms using the CMake build system.

## Prerequisites

- CMake 3.20 or later
- C++ compiler with C++23 support:
  - GCC 13.0+ (full C++23 support)
  - Clang 16.0+ (full C++23 support)
  - MSVC 2022 17.5+ (Visual Studio 2022 17.5)
- SDL2 2.24.0+ development libraries (required)
- OpenAL development libraries (required)
- enet 1.3.x library (required for networking/multiplayer)
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
cmake --install .
```

Or specify a custom installation prefix:

```bash
cmake -DCMAKE_INSTALL_PREFIX=/usr/local ..
cmake --build . -j20
cmake --install .
```

The installation will place:
- Executable in `PREFIX/bin/10kingdoms`
- Data files in `PREFIX/share/10kingdoms`
- Documentation in `PREFIX/share/doc/10kingdoms`

## Running

The executable will be built in the `build/src` directory:

```bash
# From the build directory
./src/10kingdoms

# Or from the project root with TKDATA set
TKDATA=data build/src/10kingdoms
```

The `TKDATA` environment variable must point to the directory containing the game data files (typically the `data/` directory in the source tree).

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

## Notes

- **CMake is now the only build system** - The autotools build system has been removed
- CMake uses modern dependency detection with fallback to pkg-config
- CMake supports out-of-source builds by default
- CMake provides better IDE integration and cross-platform support
- The project uses C++23 standard with compiler version checks
- Parallel builds are enabled by default (20 jobs)
