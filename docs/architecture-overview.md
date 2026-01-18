# Architecture Overview

Generated: $(date)

## System Architecture

10kingdoms is a real-time strategy game built with C++11. The architecture follows an object-oriented design with several major subsystems.

## Major Subsystems

### Game Engine (OGAME, OWORLD)
- **OGAME**: Main game loop and state management
- **OWORLD**: World map, terrain, and spatial management
- Coordinates game flow, scene management, and game state transitions

### Rendering (OVGA, OVGABUF)
- **OVGA**: Video graphics adapter interface
- **OVGABUF**: Video buffer management
- Handles 2D sprite rendering, bitmap operations, and display management
- Uses SDL2 for cross-platform video support

### Networking (enet, multiplayer)
- **multiplayer.cpp**: Multiplayer game session management
- Uses enet library for network communication
- Handles client-server synchronization and game state replication

### AI System (OAI_*)
- Multiple AI modules for different game aspects:
  - OAI_MAIN: Main AI coordinator
  - OAI_ACT, OAI_ACT2: Action planning
  - OAI_ATTK: Attack strategies
  - OAI_BUIL: Building construction
  - OAI_DEFE: Defense strategies
  - OAI_DIPL: Diplomacy
  - OAI_ECO: Economic management
  - OAI_MILI: Military operations
  - OAI_MONS: Monster AI
  - OAI_SPY: Espionage
  - OAI_TOWN: Town management
  - OAI_TRAD: Trade operations
  - OAI_UNIT: Unit control

### Entities
- **OUNIT**: Game units (soldiers, workers, etc.)
- **OFIRM**: Buildings and structures
- **OTOWN**: Towns and settlements
- Each entity type has its own management and AI logic

### Resources (ORES*, OSPRITE)
- **ORES**: Resource management (materials, gold, etc.)
- **OSPRITE**: Sprite and graphics resource management
- Handles loading, caching, and access to game assets

### Audio (OpenAL)
- Uses OpenAL for cross-platform audio
- Music and sound effect management
- Located in `src/openal/`

### Image Processing (imgfun)
- Two implementations:
  - Generic C++ version (`src/imgfun/generic/`)
  - x86 assembly optimized version (`src/imgfun/x86/`)
- Handles bitmap operations, blitting, and image transformations

## Data Flow

1. **Initialization**: `AM.cpp::main()` initializes all subsystems
2. **Game Loop**: `OGAME` manages the main game loop
3. **Input**: `OMOUSE` and input handlers process user input
4. **Update**: Game state updates (AI, physics, etc.)
5. **Render**: `OVGA`/`OVGABUF` render the current frame
6. **Audio**: `OMUSIC` and audio system play sounds

## Threading Model

The game appears to be single-threaded with a main game loop. No explicit threading was observed in the initial analysis.

## Key Design Patterns

- **Singleton Pattern**: Many global objects (sys, game, world, etc.)
- **Resource Management**: Centralized resource loading and caching
- **State Machine**: Game states managed through OGAME
- **Component-based**: Entities have modular components (AI, rendering, etc.)

## External Dependencies

- **SDL2**: Video, input, and windowing
- **OpenAL**: Audio playback
- **enet**: Network communication
- **curl**: Web services (optional)
- **gettext**: Internationalization
