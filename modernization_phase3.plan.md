# Phase 3: Architectural Refactoring Plan

**Status**: 🔄 Not Started  
**Duration**: 12-24 weeks  
**Goal**: Modernize architecture while maintaining functionality and binary compatibility

## Objectives

- [ ] Replace global instances with dependency injection
- [ ] Modernize memory management (smart pointers, RAII)
- [ ] Remove `#pragma pack(1)` and introduce serialization layer
- [ ] Modernize class hierarchy (composition over inheritance)
- [ ] Improve error handling (exceptions or `std::expected`)
- [ ] Maintain save game and multiplayer protocol compatibility
- [ ] Ensure no performance regressions
- [ ] Improve code maintainability and testability

---

## Task 1: Dependency Injection

### 1.1 Analyze Global Dependencies

**Status**: 🔄 Not Started

**Analysis:**
- [ ] Document all global variables from `globals.cpp` and `AM.cpp`
- [ ] Identify singleton patterns
- [ ] Map dependency relationships between globals
- [ ] Identify initialization order dependencies
- [ ] Document which globals are accessed where
- [ ] Create dependency graph

**Deliverables:**
- [ ] Global dependency analysis report
- [ ] Dependency graph visualization
- [ ] Initialization order documentation
- [ ] List of candidate globals for DI migration

**Files to analyze:**
- [ ] `src/globals.cpp`
- [ ] `src/AM.cpp`
- [ ] All files that access global variables

### 1.2 Design DI Container/Service Locator

**Status**: 🔄 Not Started

**Design decisions:**
- [ ] Choose DI approach (container vs service locator vs manual injection)
- [ ] Design service interfaces/abstract classes
- [ ] Design lifetime management (singleton, transient, scoped)
- [ ] Plan migration strategy (gradual vs big bang)
- [ ] Design testing strategy for DI

**Options to evaluate:**
- [ ] Simple service locator pattern
- [ ] Lightweight DI container (or custom)
- [ ] Manual dependency injection
- [ ] Hybrid approach (DI for new code, gradual migration)

**Deliverables:**
- [ ] DI design document
- [ ] Interface definitions
- [ ] Migration strategy document

### 1.3 Create Service Interfaces

**Status**: 🔄 Not Started

**Services to create interfaces for:**
- [ ] `Sys` → `ISystemService` or `SystemService`
- [ ] `Game` → `IGameService` or `GameService`
- [ ] `World` → `IWorldService` or `WorldService`
- [ ] `Vga` → `IVgaService` or `VgaService`
- [ ] `Config` → `IConfigService` or `ConfigService`
- [ ] `Battle` → `IBattleService` or `BattleService`
- [ ] `Power` → `IPowerService` or `PowerService`
- [ ] Other major global services

**Approach:**
- [ ] Start with one service as proof of concept
- [ ] Create abstract interface/base class
- [ ] Implement interface wrapper around existing class
- [ ] Test with minimal code changes

**Deliverables:**
- [ ] Service interface definitions
- [ ] Proof of concept implementation
- [ ] Migration guide for one service

### 1.4 Migrate Services Incrementally

**Status**: 🔄 Not Started

**Migration strategy:**
- [ ] Start with least-used services
- [ ] Migrate one service at a time
- [ ] Keep old global accessible during transition
- [ ] Update call sites gradually
- [ ] Remove global after all call sites migrated
- [ ] Test thoroughly after each migration

**Priority order (suggested):**
- [ ] Low-level services first (Config, File operations)
- [ ] Mid-level services (Battle, Power)
- [ ] High-level services last (Game, World, Sys)

**Deliverables:**
- [ ] Migrated services (one at a time)
- [ ] Updated call sites
- [ ] Test results for each migration
- [ ] Performance benchmarks

### 1.5 Testing & Validation

**Status**: 🔄 Not Started

- [ ] Unit tests for DI container/service locator
- [ ] Integration tests for migrated services
- [ ] Verify game functionality unchanged
- [ ] Performance benchmarking
- [ ] Memory profiling
- [ ] Create `docs/di-migration-report.md`

---

## Task 2: Modern Memory Management

### 2.1 Audit Memory Management

**Status**: 🔄 Not Started

**Analysis:**
- [ ] List all `new`/`delete` usage
- [ ] List all raw pointer usage
- [ ] Identify ownership patterns
- [ ] Identify memory leaks (using tools)
- [ ] Document custom memory allocators
- [ ] Identify RAII opportunities

**Tools:**
- [ ] Valgrind/memcheck
- [ ] AddressSanitizer
- [ ] Static analysis tools
- [ ] Custom scripts

**Deliverables:**
- [ ] Memory management audit report
- [ ] Leak detection report
- [ ] Ownership pattern documentation
- [ ] Priority list for modernization

### 2.2 Replace Raw Pointers with Smart Pointers

**Status**: 🔄 Not Started

**2.2.1 Single Object Ownership**

- [ ] Replace `new T` with `std::make_unique<T>`
- [ ] Replace `delete ptr` with automatic destruction
- [ ] Update function signatures to accept `std::unique_ptr<T>`
- [ ] Handle ownership transfers properly
- [ ] Test after each change

**2.2.2 Shared Ownership**

- [ ] Identify cases needing `std::shared_ptr`
- [ ] Replace appropriate pointers with `std::shared_ptr`
- [ ] Use `std::make_shared` where possible
- [ ] Avoid circular references
- [ ] Consider `std::weak_ptr` for breaking cycles

**2.2.3 Array Ownership**

- [ ] Replace `new T[]` with `std::vector<T>` or `std::unique_ptr<T[]>`
- [ ] Replace `delete[]` with automatic destruction
- [ ] Update array handling code

**Scope:**
- [ ] Start with standalone allocations (not in custom containers)
- [ ] Avoid changes that affect binary layout
- [ ] Test thoroughly after each change

**Deliverables:**
- [ ] Code updated with smart pointers
- [ ] Memory leak fixes
- [ ] Test suite passing
- [ ] Performance benchmarks

### 2.3 Apply RAII Consistently

**Status**: 🔄 Not Started

**Areas to modernize:**
- [ ] File handles (use RAII wrappers)
- [ ] Resource locks (use `std::lock_guard`, `std::unique_lock`)
- [ ] Network connections
- [ ] Graphics resources
- [ ] Custom resource types

**Patterns to apply:**
- [ ] RAII wrappers for C-style resources
- [ ] Move semantics where appropriate
- [ ] Exception-safe resource management
- [ ] Custom deleters for smart pointers if needed

**Deliverables:**
- [ ] RAII wrappers created
- [ ] Resource management modernized
- [ ] Exception safety improved
- [ ] Test suite passing

### 2.4 Remove Manual Memory Management

**Status**: 🔄 Not Started

**Final cleanup:**
- [ ] Remove all `new`/`delete` (except in custom allocators if needed)
- [ ] Remove all raw pointer ownership
- [ ] Update documentation
- [ ] Verify no memory leaks
- [ ] Performance validation

**Deliverables:**
- [ ] Zero manual `new`/`delete` (except where necessary)
- [ ] All memory managed automatically
- [ ] Memory leak test report
- [ ] Performance validation report

### 2.5 Testing & Validation

**Status**: 🔄 Not Started

- [ ] Run memory leak detection tools
- [ ] Run AddressSanitizer
- [ ] Performance benchmarking
- [ ] Stress testing
- [ ] Long-running game session tests
- [ ] Create `docs/memory-modernization-report.md`

---

## Task 3: Remove `#pragma pack(1)` and Serialization

### 3.1 Audit `#pragma pack(1)` Usage

**Status**: 🔄 Not Started

**Analysis:**
- [ ] List all `#pragma pack(1)` directives
- [ ] Document which structs use packed layout
- [ ] Identify save game file formats
- [ ] Identify network message formats
- [ ] Document binary compatibility requirements
- [ ] Create struct layout documentation

**Files to check:**
- [ ] `include/OGF_REC.h` (save game records)
- [ ] `include/OCONFIG.h`
- [ ] `include/OINFO.h`
- [ ] `include/OGAMHALL.h`
- [ ] `include/OUNITRES.h`
- [ ] `src/OFONT.cpp`
- [ ] Other files with `#pragma pack`

**Deliverables:**
- [ ] Complete list of packed structs
- [ ] Struct layout documentation
- [ ] Binary format specification
- [ ] Compatibility requirements document

### 3.2 Design Serialization Layer

**Status**: 🔄 Not Started

**Design requirements:**
- [ ] Maintain save game compatibility (version old format)
- [ ] Support new format with proper alignment
- [ ] Version save files
- [ ] Provide migration path
- [ ] Handle endianness
- [ ] Handle platform differences

**Serialization approach:**
- [ ] Custom serialization functions
- [ ] Template-based serialization
- [ ] Visitor pattern
- [ ] Reflection-based (if available in C++23)
- [ ] Hybrid approach

**Design decisions:**
- [ ] Save file versioning scheme
- [ ] Backward compatibility strategy
- [ ] Forward compatibility strategy
- [ ] Migration tool design

**Deliverables:**
- [ ] Serialization design document
- [ ] API design
- [ ] Versioning strategy
- [ ] Migration plan

### 3.3 Implement Serialization Layer

**Status**: 🔄 Not Started

**Implementation:**
- [ ] Create serialization base classes/functions
- [ ] Implement serialization for one struct type (proof of concept)
- [ ] Test serialization/deserialization
- [ ] Verify binary compatibility with old format
- [ ] Implement version detection
- [ ] Implement migration functions

**Priority structs:**
- [ ] Start with simple structs
- [ ] Progress to complex nested structures
- [ ] Handle arrays and dynamic data
- [ ] Handle pointers (convert to indices or offsets)

**Deliverables:**
- [ ] Serialization framework
- [ ] Proof of concept implementation
- [ ] Test suite for serialization
- [ ] Compatibility verification

### 3.4 Migrate Structs Incrementally

**Status**: 🔄 Not Started

**Migration process:**
- [ ] Remove `#pragma pack(1)` from one struct
- [ ] Add serialization functions
- [ ] Update save/load code
- [ ] Test with old save files
- [ ] Test with new save files
- [ ] Verify multiplayer protocol unchanged
- [ ] Repeat for next struct

**Migration order:**
- [ ] Non-critical structs first
- [ ] Simple structs before complex
- [ ] Test thoroughly after each
- [ ] Document changes

**Deliverables:**
- [ ] Migrated structs (one at a time)
- [ ] Updated save/load code
- [ ] Compatibility test results
- [ ] Migration documentation

### 3.5 Version Save Files

**Status**: 🔄 Not Started

**Versioning:**
- [ ] Add version header to save files
- [ ] Implement version detection
- [ ] Implement migration from old to new format
- [ ] Test migration with real save files
- [ ] Provide migration tool/function
- [ ] Document version history

**Deliverables:**
- [ ] Versioned save file format
- [ ] Migration functions
- [ ] Migration tool
- [ ] Version documentation

### 3.6 Remove All `#pragma pack(1)`

**Status**: 🔄 Not Started

**Final cleanup:**
- [ ] Remove all `#pragma pack(1)` directives
- [ ] Remove all `#pragma pack()` directives
- [ ] Verify all structs use natural alignment
- [ ] Update documentation
- [ ] Final compatibility testing

**Deliverables:**
- [ ] Zero `#pragma pack` directives
- [ ] All structs use natural alignment
- [ ] Compatibility maintained
- [ ] Final test report

### 3.7 Testing & Validation

**Status**: 🔄 Not Started

- [ ] Test save/load with old save files
- [ ] Test save/load with new save files
- [ ] Test migration from old to new
- [ ] Test multiplayer protocol compatibility
- [ ] Test on different platforms (endianness)
- [ ] Performance benchmarking
- [ ] Create `docs/serialization-report.md`

---

## Task 4: Modernize Class Hierarchy

### 4.1 Analyze Class Hierarchy

**Status**: 🔄 Not Started

**Analysis:**
- [ ] Document inheritance hierarchies
- [ ] Identify deep inheritance chains
- [ ] Identify virtual function usage
- [ ] Identify composition opportunities
- [ ] Document coupling between classes
- [ ] Identify interfaces vs implementations

**Tools:**
- [ ] Dependency analysis tools
- [ ] Class diagram generation
- [ ] Code metrics tools

**Deliverables:**
- [ ] Class hierarchy documentation
- [ ] Inheritance diagram
- [ ] Coupling analysis
- [ ] Refactoring opportunities list

### 4.2 Identify Composition Opportunities

**Status**: 🔄 Not Started

**Patterns to identify:**
- [ ] "is-a" vs "has-a" relationships
- [ ] Classes that could use composition
- [ ] Inheritance used for code reuse (should be composition)
- [ ] Multiple inheritance cases
- [ ] Virtual inheritance cases

**Areas to focus:**
- [ ] Game entities (Unit, Town, Firm)
- [ ] UI components
- [ ] Resource management
- [ ] AI systems

**Deliverables:**
- [ ] Composition opportunities list
- [ ] Refactoring plan
- [ ] Priority ranking

### 4.3 Create Interfaces/Abstract Classes

**Status**: 🔄 Not Started

**Interfaces to create:**
- [ ] `IRenderable` for renderable objects
- [ ] `IUpdatable` for updateable objects
- [ ] `ISerializable` for serializable objects
- [ ] `IGameEntity` for game entities
- [ ] Other domain-specific interfaces

**Approach:**
- [ ] Extract interfaces from existing classes
- [ ] Use pure virtual functions
- [ ] Keep implementations separate
- [ ] Test with one interface first

**Deliverables:**
- [ ] Interface definitions
- [ ] Proof of concept
- [ ] Interface usage guide

### 4.4 Refactor to Composition

**Status**: 🔄 Not Started

**Refactoring process:**
- [ ] Start with one class hierarchy
- [ ] Replace inheritance with composition
- [ ] Use interfaces for polymorphism
- [ ] Update all call sites
- [ ] Test thoroughly
- [ ] Repeat for next hierarchy

**Priority areas:**
- [ ] Simple hierarchies first
- [ ] Low-risk areas
- [ ] Well-tested code
- [ ] Document changes

**Deliverables:**
- [ ] Refactored classes
- [ ] Updated code using composition
- [ ] Test results
- [ ] Performance validation

### 4.5 Evaluate ECS (Entity Component System)

**Status**: 🔄 Not Started

**Evaluation:**
- [ ] Research ECS patterns
- [ ] Evaluate if ECS fits game entities
- [ ] Design ECS architecture (if beneficial)
- [ ] Prototype ECS implementation
- [ ] Compare with current design
- [ ] **Decision point**: Adopt ECS or keep current design?

**Considerations:**
- [ ] Performance implications
- [ ] Code complexity
- [ ] Migration effort
- [ ] Benefits vs costs

**Deliverables:**
- [ ] ECS evaluation report
- [ ] Prototype (if created)
- [ ] Decision document

### 4.6 Testing & Validation

**Status**: 🔄 Not Started

- [ ] Unit tests for refactored classes
- [ ] Integration tests
- [ ] Game functionality tests
- [ ] Performance benchmarking
- [ ] Memory profiling
- [ ] Create `docs/class-hierarchy-modernization-report.md`

---

## Task 5: Error Handling Modernization

### 5.1 Audit Error Handling

**Status**: 🔄 Not Started

**Analysis:**
- [ ] List all error code returns
- [ ] List all error flag patterns
- [ ] Identify error handling inconsistencies
- [ ] Document current error handling patterns
- [ ] Identify exception usage (if any)
- [ ] Identify error propagation paths

**Deliverables:**
- [ ] Error handling audit report
- [ ] Error code catalog
- [ ] Error handling pattern documentation

### 5.2 Design Error Handling Strategy

**Status**: 🔄 Not Started

**Options to evaluate:**
- [ ] Exceptions for exceptional cases
- [ ] `std::expected<T, E>` (C++23) for recoverable errors
- [ ] `std::optional<T>` for simple nullable cases
- [ ] Hybrid approach
- [ ] Error code enum classes

**Design decisions:**
- [ ] When to use exceptions vs `std::expected`
- [ ] Error type definitions
- [ ] Error propagation strategy
- [ ] Error recovery strategies
- [ ] Backward compatibility

**Deliverables:**
- [ ] Error handling design document
- [ ] Error type definitions
- [ ] Migration strategy

### 5.3 Implement Error Types

**Status**: 🔄 Not Started

**Implementation:**
- [ ] Define error code enums (if using)
- [ ] Define error types for `std::expected`
- [ ] Define exception types (if using)
- [ ] Create error conversion utilities
- [ ] Implement error formatting/display

**Deliverables:**
- [ ] Error type definitions
- [ ] Error utilities
- [ ] Error handling helpers

### 5.4 Migrate Error Handling Incrementally

**Status**: 🔄 Not Started

**Migration process:**
- [ ] Start with one module/function
- [ ] Replace error codes with `std::expected` or exceptions
- [ ] Update call sites
- [ ] Handle error propagation
- [ ] Test thoroughly
- [ ] Repeat for next module

**Priority areas:**
- [ ] File I/O operations
- [ ] Network operations
- [ ] Resource loading
- [ ] Configuration loading
- [ ] Game logic

**Deliverables:**
- [ ] Migrated error handling
- [ ] Updated call sites
- [ ] Test results
- [ ] Error handling improvements

### 5.5 Testing & Validation

**Status**: 🔄 Not Started

- [ ] Test error cases
- [ ] Test error propagation
- [ ] Test error recovery
- [ ] Performance impact assessment
- [ ] Create `docs/error-handling-modernization-report.md`

---

## Task 6: Compatibility & Safety

### 6.1 Binary Compatibility Verification

**Status**: 🔄 Not Started

**Critical checks:**
- [ ] Verify save game file format compatibility (with versioning)
- [ ] Verify multiplayer protocol compatibility
- [ ] Verify network message format compatibility
- [ ] Test save/load with old save files (via migration)
- [ ] Test multiplayer with old clients (if possible)
- [ ] Document any breaking changes

**Deliverables:**
- [ ] Compatibility test report
- [ ] Migration guide
- [ ] Breaking changes documentation

### 6.2 FPU Determinism (Multiplayer)

**Status**: 🔄 Not Started

**Critical for multiplayer:**
- [ ] Verify 387 FPU settings still applied
- [ ] Test multiplayer determinism after refactoring
- [ ] Ensure floating-point operations remain deterministic
- [ ] Document FPU requirements

**Deliverables:**
- [ ] FPU determinism test report

### 6.3 Performance Validation

**Status**: 🔄 Not Started

- [ ] Benchmark critical paths before/after
- [ ] Profile memory usage
- [ ] Profile CPU usage
- [ ] Compare performance metrics
- [ ] Document any performance changes

**Deliverables:**
- [ ] Performance benchmark report
- [ ] Performance regression analysis

---

## Success Criteria

- [ ] All major global variables replaced with DI
- [ ] All manual memory management replaced with smart pointers/RAII
- [ ] All `#pragma pack(1)` removed
- [ ] Serialization layer implemented and tested
- [ ] Class hierarchies modernized (composition over inheritance)
- [ ] Error handling modernized
- [ ] Save game compatibility maintained (with versioning)
- [ ] Multiplayer protocol compatibility maintained
- [ ] Performance equal or better than before
- [ ] FPU determinism maintained for multiplayer
- [ ] Code quality metrics improved
- [ ] Test coverage improved
- [ ] Documentation complete

---

## Timeline

**Weeks 1-4: Dependency Injection**
- Week 1: Analysis and design
- Week 2: Create interfaces and proof of concept
- Week 3-4: Migrate services incrementally

**Weeks 5-8: Memory Management**
- Week 5: Audit and planning
- Week 6-7: Replace raw pointers with smart pointers
- Week 8: Apply RAII and final cleanup

**Weeks 9-14: Serialization**
- Week 9: Audit `#pragma pack` usage
- Week 10: Design serialization layer
- Week 11-12: Implement serialization
- Week 13: Migrate structs incrementally
- Week 14: Version save files and remove `#pragma pack`

**Weeks 15-18: Class Hierarchy**
- Week 15: Analyze hierarchies
- Week 16: Create interfaces
- Week 17-18: Refactor to composition

**Weeks 19-20: Error Handling**
- Week 19: Design and implement error types
- Week 20: Migrate error handling

**Weeks 21-24: Testing & Validation**
- Weeks 21-22: Comprehensive testing
- Week 23: Performance validation
- Week 24: Documentation and review

---

## Risks & Mitigation

**Risk**: Breaking save game compatibility
- *Mitigation*: Version save files, implement migration, extensive testing

**Risk**: Breaking multiplayer protocol compatibility
- *Mitigation*: Maintain protocol, version if needed, extensive testing

**Risk**: Performance regressions from smart pointers
- *Mitigation*: Benchmark at each stage, profile, use `std::unique_ptr` where possible

**Risk**: Circular dependencies in DI
- *Mitigation*: Careful design, use `std::weak_ptr` where needed, avoid cycles

**Risk**: Serialization complexity
- *Mitigation*: Start simple, test thoroughly, maintain backward compatibility

**Risk**: Refactoring breaking game logic
- *Mitigation*: Test after each change, use version control, incremental approach

**Risk**: Scope creep
- *Mitigation*: Strict phase boundaries, prioritize critical paths, defer non-essential changes

**Risk**: Team knowledge loss
- *Mitigation*: Document decisions, code reviews, pair programming on critical sections

---

## Notes

- **Incremental approach**: One module/component at a time, test thoroughly
- **Binary compatibility**: Maintain where possible, version when breaking
- **Test-driven**: Write tests before/while refactoring
- **Performance first**: Don't sacrifice performance for modernity
- **Compatibility critical**: Save games and multiplayer protocol must work
- **Documentation**: Document all architectural decisions
- **Hybrid approach**: New code modern, legacy code gradually modernized

---

## Feature Usage Guidelines

### When to Use Dependency Injection

**Use DI for:**
- Services with clear interfaces
- Testable components
- Components with dependencies
- Singleton-like services

**Avoid DI for:**
- Simple value types
- Performance-critical hot paths (consider carefully)
- Trivial dependencies

### When to Use Smart Pointers

**Use `std::unique_ptr<T>`:**
- Single ownership
- Transfer ownership
- Default choice

**Use `std::shared_ptr<T>`:**
- Shared ownership needed
- Multiple owners
- Use sparingly (performance cost)

**Use `std::weak_ptr<T>`:**
- Breaking circular references
- Observing without owning
- Cache-like patterns

### When to Use Composition

**Prefer composition when:**
- "has-a" relationship
- Code reuse without polymorphism
- Avoiding deep inheritance
- More flexible design

**Use inheritance when:**
- True "is-a" relationship
- Polymorphism needed
- Interface implementation

### When to Use Exceptions vs `std::expected`

**Use exceptions for:**
- Exceptional, unrecoverable errors
- Programming errors (assertions)
- System-level failures

**Use `std::expected<T, E>` for:**
- Recoverable errors
- Expected failure cases
- Function return values that may fail
- Error codes that need to be handled

**Use `std::optional<T>` for:**
- Simple nullable values
- Optional parameters
- "value or nothing" cases

---

*Created: 2025-01-27*  
*Last updated: 2025-01-27*  
*Status: Phase 3 not started - awaiting Phase 2 completion*
