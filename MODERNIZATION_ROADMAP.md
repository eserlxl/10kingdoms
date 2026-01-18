# Ten Kingdoms Modernization Roadmap

## Executive Summary

This document outlines the roadmap for modernizing the Ten Kingdoms codebase from C++11/autotools to C++23/26 with CMake and improved architecture. This is a large-scale refactoring project estimated to take 6-12 months.

## Current State Assessment

**Build System:**
- Autotools (configure.ac, Makefile.am)
- C++11 standard (configure.ac line 45: `AX_CXX_COMPILE_STDCXX_11`)
- Complex multi-directory structure with embedded libraries

**Codebase Size:**
- ~328 .cpp files
- ~193 header files
- Embedded libraries: enet, openal, curl, imgfun
- Assembly code (.asm files)
- Internationalization (gettext)

**Architecture:**
- Legacy design patterns:
  - Global variables (see globals.cpp)
  - Manual memory management
  - `#pragma pack(1)` for binary compatibility
  - Heavy inheritance-based design
  - Old-style C++ (pre-modern C++)
- Multiplayer requires 387 FPU for determinism
- Binary compatibility critical (save games, multiplayer protocol)

## Challenges

1. **Scale**: Large, interconnected codebase with deep dependencies
2. **Compatibility**: Must maintain save game and multiplayer protocol compatibility
3. **Legacy Requirements**: 387 FPU requirement for multiplayer determinism
4. **Complexity**: Game logic, rendering, networking, AI all tightly coupled

## Roadmap Overview

### Phase 1: Foundation & Assessment (2-4 weeks)
**Goal**: Establish modern build system foundation and understand codebase

1. **Create CMake Build System (Parallel to Autotools)**
   - Start with minimal CMakeLists.txt that builds current code
   - Keep autotools working during transition
   - Test that both produce identical binaries

2. **Codebase Analysis**
   - Generate dependency graph
   - Identify all global state
   - Document current architecture
   - List deprecated/unsafe patterns

3. **Set Up Modern Tooling**
   - Static analyzers (clang-tidy, cppcheck)
   - Code formatters (clang-format)
   - Basic CI/CD setup

### Phase 2: Incremental C++ Standard Upgrade (4-8 weeks)
**Goal**: Gradually upgrade C++ standard with minimal breaking changes

1. **C++14 (Minimal Changes)**
   - Enable C++14 in CMake
   - Fix deprecation warnings
   - Use `auto` where safe

2. **C++17 (Moderate Changes)**
   - Introduce `std::optional`, `std::variant`
   - Use filesystem API
   - Structured bindings
   - `if constexpr`

3. **C++20 (Significant Changes)**
   - Modules (experimental, optional)
   - Concepts
   - Ranges (where applicable)
   - Coroutines (if useful)

4. **C++23/26 (Target)**
   - Latest features as available
   - **Note**: C++26 is not finalized; target C++23 with C++26 preview

### Phase 3: Architectural Refactoring (12-24 weeks)
**Goal**: Modernize architecture while maintaining functionality

1. **Dependency Injection**
   - Replace global instances with DI
   - Start with new code, migrate incrementally

2. **Modern Memory Management**
   - Replace raw pointers with smart pointers
   - Use RAII consistently
   - Remove manual `new`/`delete`

3. **Remove `#pragma pack(1)`**
   - Introduce serialization layer
   - Maintain save game compatibility
   - Version save files

4. **Modernize Class Hierarchy**
   - Prefer composition over inheritance
   - Use interfaces/abstract classes
   - Consider ECS for game entities

5. **Error Handling**
   - Replace error codes with exceptions or `std::expected` (C++23)
   - Use `std::optional` for nullable values

### Phase 4: Build System Migration (2-4 weeks)
**Goal**: Complete migration to CMake

1. **Complete CMake Migration**
   - All libraries and subdirectories
   - Platform-specific code (Windows/Linux/macOS)
   - Assembly integration
   - Internationalization (gettext)

2. **Remove Autotools**
   - After CMake is fully functional
   - Update documentation

### Phase 5: Testing & Validation (Ongoing)
**Goal**: Ensure quality and compatibility

1. **Unit Tests**
   - Start with critical paths
   - Game logic, serialization, networking

2. **Integration Tests**
   - Save/load compatibility
   - Multiplayer determinism

3. **Performance Testing**
   - Ensure no regressions
   - Profile and optimize

## Critical Recommendations

### 1. C++ Standard Target
**Do NOT target C++26 yet** - it's not finalized. Target C++23 with C++26 preview flags.

### 2. Binary Compatibility
- Version save files
- Maintain multiplayer protocol compatibility
- Keep FPU determinism for multiplayer

### 3. Incremental Approach
- One module at a time
- Keep game playable
- Test after each major change

### 4. Hybrid Approach
- New code in modern C++
- Legacy code gradually modernized
- Clear boundaries between old and new

### 5. Assembly Code
- Keep x86 assembly if needed for performance
- Consider modern intrinsics as alternatives
- Document why assembly is used

## Estimated Timeline

- **Phase 1**: 2-4 weeks
- **Phase 2**: 4-8 weeks
- **Phase 3**: 12-24 weeks (can be done incrementally)
- **Phase 4**: 2-4 weeks
- **Phase 5**: Ongoing

**Total**: 6-12 months for complete modernization (depending on team size and priorities)

## Success Criteria

- [ ] CMake build system fully functional
- [ ] Code compiles with C++23 standard
- [ ] All tests pass
- [ ] Save game compatibility maintained
- [ ] Multiplayer compatibility maintained
- [ ] Performance equal or better than before
- [ ] Code quality metrics improved
- [ ] Documentation updated

## Risk Mitigation

1. **Save Game Compatibility**: Version save files, provide migration tools
2. **Multiplayer Compatibility**: Maintain protocol, extensive testing
3. **Performance Regressions**: Continuous benchmarking
4. **Scope Creep**: Strict phase boundaries, prioritize critical paths
5. **Team Knowledge**: Document decisions, pair programming on critical sections

## Next Steps

1. Review and approve this roadmap
2. Set up development branch for modernization work
3. Begin Phase 1 implementation
4. Establish regular review checkpoints

---

*Document created: 2025-01-27*
*Last updated: 2025-01-27*
