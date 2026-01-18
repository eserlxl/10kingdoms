# Ten Kingdoms: Ancient Adversaries

Ten Kingdoms is a public fork of the Seven Kingdoms project for 7kfans.

**Forked from:** Seven Kingdoms 2.15.7  
**Seven Kingdoms website:** [www.7kfans.com](http://www.7kfans.com)  
**Seven Kingdoms source repo:** [sourceforge.net/projects/skfans](https://sourceforge.net/projects/skfans)

Ten Kingdoms is a community continuation of the Seven Kingdoms project, maintained for the 7kfans community. This fork is based on Seven Kingdoms version 2.15.7 and continues development under the GPL.

## Acknowledgments & Trademarks

- **Seven Kingdoms** is a trademark of Enlight Software Ltd., used here with permission for non-commercial, community development projects.
- The source code for Seven Kingdoms was released under the GPL‑2.0‑or‑later by Enlight Software in 2009. See COPYING for full details.
- **Ten Kingdoms** is a community continuation of the Seven Kingdoms project (via 7kfans), now licensed under GPL‑3.0.

This project does not imply any transfer of trademark ownership or official endorsement by Enlight. For trademark questions, please contact Enlight Software Ltd.

## System Requirements

### Supported Operating Systems
- Windows (mingw-w64/MSYS2 or Visual Studio)
- Linux
- macOS

### Supported Architectures
- x86/x86_64 compatible processor
- **Note:** Multiplayer requires x86 architecture with 387 FPU support for deterministic gameplay

## Quick Start

### Prerequisites

Install the required dependencies for your platform:

**Linux (Ubuntu/Debian):**
```bash
sudo apt-get install cmake build-essential libsdl2-dev libopenal-dev libenet-dev libcurl4-openssl-dev gettext
```

**Linux (Arch Linux):**
```bash
sudo pacman -S cmake base-devel sdl2 openal enet curl gettext
```

**macOS:**
```bash
brew install cmake sdl2 openal-soft enet curl gettext
```

**Windows (MSYS2):**
```bash
pacman -S mingw-w64-x86_64-cmake mingw-w64-x86_64-gcc mingw-w64-x86_64-SDL2 mingw-w64-x86_64-openal mingw-w64-x86_64-enet mingw-w64-x86_64-curl gettext
```

### Building

```bash
# Clone the repository
git clone https://github.com/eserlxl/10kingdoms.git
cd 10kingdoms

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build (uses parallel compilation with 20 jobs)
cmake --build . -j20

# Or if using Makefile generator
make -j20
```

The executable will be created at `build/src/10kingdoms`.

### Running

To run the game from the build directory, set the `TKDATA` environment variable to point to the game data folder:

```bash
TKDATA=../data build/src/10kingdoms
```

Or from the project root:
```bash
TKDATA=data build/src/10kingdoms
```

## Build System

This project uses **CMake** exclusively for building. The autotools build system has been removed.

### Build Requirements

- **CMake** 3.20 or later
- **C++ compiler** with C++23 support:
  - GCC 13.0+ (full C++23 support)
  - Clang 16.0+ (full C++23 support)
  - MSVC 2022 17.5+ (Visual Studio 2022 17.5)
- **SDL2** 2.24.0+ (required for video/input)
- **OpenAL** (required for audio)
- **enet** 1.3.x (required for networking/multiplayer)

### Optional Dependencies

- **libcurl** - For full 7kfans multiplayer integration (website requires TLS1.2)
- **gettext** 0.19+ - For internationalization support
- **jwasm** - For x86 assembly optimizations (enable with `-DENABLE_ASM=ON`)
- **Game music** - 10kingdoms-music-2.15 package (copy to data folder with uppercase filenames)
- **NSIS** - For building Windows installer
- **TeX Live** - For generating game manual PDF (requires latexmk, fontaxes, accanthis, universalis, yfonts)

### Build Options

CMake provides several configuration options:

- `ENABLE_ASM` (default: OFF) - Enable x86 assembly optimizations (requires jwasm)
- `ENABLE_CURL` (default: ON) - Enable curl support for web services
- `ENABLE_ENET` (default: ON) - Enable enet support for networking
- `ENABLE_MULTIPLAYER` (default: ON) - Enable multiplayer support in menu
- `ENABLE_DEBUG` (default: OFF) - Enable debugging features
- `CMAKE_BUILD_TYPE` - Build type: Release, Debug, RelWithDebInfo, MinSizeRel

Example with options:
```bash
cmake -DENABLE_ASM=ON -DCMAKE_BUILD_TYPE=Release ..
```

### Installation

After building, you can install the game:
```bash
cmake --install . --prefix /usr/local
```

Or specify a custom prefix:
```bash
cmake --install . --prefix /opt/10kingdoms
```

## Project Structure

- `src/` - Source code (C++ files)
- `include/` - Header files
- `data/` - Game data files (graphics, sounds, scenarios)
- `tools/` - Build tools and utilities
- `doc/` - Documentation (LaTeX source)
- `docs/` - Additional documentation (Markdown)
- `packaging/` - Packaging scripts for distributions

## Development

### Codebase Status

The project is currently being modernized:
- ✅ **Build System:** Migrated from autotools to CMake
- ✅ **C++ Standard:** Upgraded to C++23
- 🔄 **Architecture:** Ongoing modernization (see MODERNIZATION_ROADMAP.md)

### Building for Development

For development builds with debugging symbols:
```bash
cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_DEBUG=ON ..
cmake --build . -j20
```

### Contributing

Contributions are welcome! Please ensure:
- Code follows the existing style
- Builds successfully with CMake
- Maintains compatibility with existing save games and multiplayer protocol
- Includes appropriate documentation

## Documentation

- `BUILD_CMAKE.md` - Detailed CMake build instructions
- `MODERNIZATION_ROADMAP.md` - Project modernization plan
- `docs/` - Architecture and subsystem documentation

## License

This project is licensed under the GNU General Public License version 3 (GPL-3.0). See the COPYING file for details.

