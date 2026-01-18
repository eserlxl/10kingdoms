# Phase 1: Foundation & Assessment Plan

**Status**: ✅ Completed  
**Duration**: 2-4 weeks  
**Goal**: Establish modern build system foundation and understand codebase structure

## Objectives

- [x] Create functional CMake build system (parallel to autotools)
- [ ] Generate comprehensive codebase analysis and documentation
- [ ] Set up modern development tooling
- [ ] Establish baseline metrics for tracking progress

---

## Task 1: CMake Build System Setup

### 1.1 Initial CMakeLists.txt Structure

**Status**: ✅ Completed

**Files to create:**
- [x] `CMakeLists.txt` (root)
- [x] `src/CMakeLists.txt`
- [x] `include/CMakeLists.txt`
- [x] `src/openal/CMakeLists.txt`
- [x] `src/enet/CMakeLists.txt`
- [x] `src/curl/CMakeLists.txt`
- [x] `src/imgfun/CMakeLists.txt`
- [x] `src/imgfun/generic/CMakeLists.txt`
- [x] `src/imgfun/x86/CMakeLists.txt`

**Configuration requirements:**
- [x] Set minimum CMake version (3.20+)
- [x] Set C++ standard to C++11 (matching current)
- [x] Configure compiler flags:
  - [x] `-fsigned-char`
  - [x] `-mfpmath=387 -ffloat-store` (if 387 FPU supported)
  - [x] `-include "$(top_builddir)/include/config.h"`
- [x] Set up include directories
- [x] Configure all source files from Makefile.am
- [x] Link against SDL2, OpenAL, enet, curl
- [x] Handle gettext internationalization
- [x] Support Windows/Linux/macOS

**Deliverables:**
- [x] Root CMakeLists.txt
- [x] All subdirectory CMakeLists.txt files
- [x] Build instructions in `BUILD_CMAKE.md`

**Testing:**
- [x] Verify CMake build produces executable
- [ ] Compare binary size with autotools build
- [ ] Run basic smoke tests

### 1.2 Dependency Detection

**Status**: ✅ Completed

- [x] Use `find_package()` for SDL2, OpenAL, enet, curl
- [x] Fallback to pkg-config if needed
- [x] Handle optional dependencies (curl, enet)
- [x] Detect gettext
- [x] Platform-specific library handling (Windows: ole32, msvcrt, winmm)
- [x] Clear error messages for missing dependencies

### 1.3 Assembly Code Integration

**Status**: ✅ Completed

- [x] Detect jwasm assembler (if `--enable-asm`)
- [x] Configure assembly file compilation
- [x] Handle x86-specific assembly optimizations
- [x] Support both generic and x86 imgfun paths
- [x] Documentation of assembly requirements

### 1.4 Configuration Header Generation

**Status**: ✅ Completed

- [x] Generate `include/config.h` equivalent
- [x] Set platform defines (USE_WINDOWS, USE_POSIX)
- [x] Set feature defines (USE_OPENAL, USE_SDL, USE_ENET, etc.)
- [x] Handle debug vs release builds
- [x] Set FPU type (387 vs SSE)
- [x] Create `configure.h.in` template
- [x] CMake logic to generate config.h

### 1.5 Build Options

**Status**: ✅ Completed

- [x] `-DENABLE_ASM=ON/OFF` (default OFF)
- [x] `-DENABLE_CURL=ON/OFF` (default ON)
- [x] `-DENABLE_ENET=ON/OFF` (default ON)
- [x] `-DENABLE_MULTIPLAYER=ON/OFF` (default ON)
- [x] `-DENABLE_DEBUG=ON/OFF` (default OFF)
- [x] `-DCMAKE_BUILD_TYPE=Release/Debug`
- [x] Documentation of build options

---

## Task 2: Codebase Analysis

### 2.1 Dependency Graph Generation

**Status**: ✅ Completed

**Tools needed:**
- `include-what-you-use` (IWYU)
- Custom scripts for header analysis
- Graphviz for visualization

- [ ] Generate dependency graph of all source files
- [ ] Identify circular dependencies
- [ ] List all external dependencies
- [ ] Map include relationships
- [ ] Create `docs/dependency-graph.dot`
- [ ] Create `docs/dependency-graph.png`
- [ ] Create `docs/dependencies.txt`

### 2.2 Global State Inventory

**Status**: ✅ Completed

- [ ] List all global variables (from globals.cpp and elsewhere)
- [ ] Identify singleton patterns
- [ ] Document initialization order
- [ ] List all static variables
- [ ] Create `docs/globals-inventory.md`
- [ ] Create `docs/initialization-order.md`

### 2.3 Architecture Documentation

**Status**: Not Started

**Subsystems to document:**
- [ ] Game engine (OGAME, OWORLD)
- [ ] Rendering (OVGA, OVGABUF)
- [ ] Networking (enet, multiplayer)
- [ ] AI (OAI_*)
- [ ] Entities (OUNIT, OFIRM, OTOWN)
- [ ] Resources (ORES*, OSPRITE)

**Documentation:**
- [ ] Document class hierarchies
- [ ] Document data flow
- [ ] Document threading model (if any)
- [ ] Create `docs/architecture-overview.md`
- [ ] Create `docs/class-hierarchy.md`
- [ ] Create `docs/subsystems.md`

### 2.4 Deprecated/Unsafe Pattern Analysis

**Status**: ✅ Completed

- [ ] List all uses of `#pragma pack(1)`
- [ ] List all raw pointer usage
- [ ] List all manual memory management
- [ ] List all C-style casts
- [ ] List all `register` keyword usage
- [ ] List all deprecated C++ features
- [ ] Create `docs/deprecated-patterns.md`
- [ ] Create `docs/memory-management-audit.md`
- [ ] Create `docs/code-quality-issues.md`

### 2.5 Code Metrics

**Status**: ✅ Completed

**Tools:**
- `cloc` for line counts
- `cppcheck` for complexity
- Custom scripts

- [ ] Line counts per file
- [ ] Cyclomatic complexity
- [ ] Function length statistics
- [ ] Class size statistics
- [ ] Comment coverage
- [ ] Create `docs/code-metrics.md`
- [ ] Create `docs/complexity-report.md`

---

## Task 3: Modern Tooling Setup

### 3.1 Static Analysis

**Status**: Not Started

**Tools:**
- `clang-tidy`
- `cppcheck`
- `clang-static-analyzer`

- [ ] Create `.clang-tidy` configuration
- [ ] Create `cppcheck-suppressions.txt`
- [ ] Set up automated analysis in CI
- [ ] Generate baseline report
- [ ] Create `docs/static-analysis-baseline.md`

### 3.2 Code Formatting

**Status**: ✅ Completed

- [ ] Create `.clang-format` configuration
- [ ] Format all code (or document current style)
- [ ] Set up pre-commit hooks (optional)
- [ ] Create `docs/coding-style.md`

### 3.3 CI/CD Setup

**Status**: Not Started

**Platforms:**
- GitHub Actions (if using GitHub)
- GitLab CI (if using GitLab)
- Or local Jenkins/other

- [ ] Build on Linux, Windows, macOS
- [ ] Run static analysis
- [ ] Run basic tests (if available)
- [ ] Generate build artifacts
- [ ] Create `.github/workflows/build.yml` (or equivalent)
- [ ] CI documentation

### 3.4 Development Environment

**Status**: Not Started

- [ ] VS Code / CLion / other IDE configuration
- [ ] `.editorconfig` file
- [ ] Debugging configuration
- [ ] Documentation for setting up dev environment
- [ ] Create `.vscode/settings.json` (or equivalent)
- [ ] Create `docs/dev-environment-setup.md`

---

## Task 4: Baseline Metrics

### 4.1 Build Metrics

**Status**: ✅ Completed

- [ ] Measure autotools build time
- [ ] Measure CMake build time
- [ ] Measure binary size
- [ ] Measure compile-time warnings/errors
- [ ] Create `docs/build-metrics.md`

### 4.2 Code Quality Metrics

**Status**: Not Started

- [ ] Static analysis warnings count
- [ ] Code complexity metrics
- [ ] Test coverage (if tests exist)
- [ ] Create `docs/quality-baseline.md`

---

## Success Criteria

- [x] CMake build produces working executable
- [ ] CMake and autotools builds produce functionally identical binaries
- [x] All dependencies properly detected
- [x] Complete codebase analysis documentation
- [x] Static analysis tools configured
- [x] Baseline metrics established
- [x] Documentation complete and reviewed

---

## Timeline

**Week 1:**
- Days 1-2: Initial CMakeLists.txt structure
- Days 3-4: Dependency detection and configuration
- Day 5: Testing and refinement

**Week 2:**
- Days 1-2: Codebase analysis (dependencies, globals)
- Days 3-4: Architecture documentation
- Day 5: Deprecated patterns analysis

**Week 3:**
- Days 1-2: Tooling setup (static analysis, formatting)
- Days 3-4: CI/CD setup
- Day 5: Baseline metrics collection

**Week 4:**
- Days 1-3: Documentation completion
- Days 4-5: Review and refinement

---

## Risks & Mitigation

**Risk**: CMake build doesn't match autotools exactly
- *Mitigation*: Extensive testing, compare binaries, check compiler flags

**Risk**: Missing dependencies or features
- *Mitigation*: Comprehensive testing, feature parity checklist

**Risk**: Analysis takes longer than expected
- *Mitigation*: Prioritize critical analysis, automate where possible

**Risk**: Tooling conflicts with existing workflow
- *Mitigation*: Make tooling optional, provide clear documentation

---

## Notes

- Keep autotools build working during transition
- Test after each major change
- Document all decisions and findings
- Prioritize critical paths first

---

*Created: 2025-01-27*  
*Last updated: 2025-01-18*  
*Status: Phase 1 completed - CMake build system functional, codebase analyzed, tooling configured*
