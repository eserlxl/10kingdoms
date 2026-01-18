# Subsystems Documentation

Generated: $(date)

## Game Engine Subsystem

### OGAME (Game Management)
- Main game loop coordination
- Game state management (menu, playing, paused)
- Scene transitions
- Game initialization and cleanup

### OWORLD (World Management)
- World map representation
- Terrain management
- Spatial queries and pathfinding
- World state updates

## Rendering Subsystem

### OVGA (Video Graphics Adapter)
- Low-level graphics operations
- Display mode management
- Bitmap operations
- Color palette management

### OVGABUF (Video Buffer)
- Frame buffer management
- Double buffering
- Screen updates
- Blitting operations

## Networking Subsystem

### multiplayer.cpp
- Network session management
- Client-server communication
- Game state synchronization
- Uses enet library for transport

## AI Subsystem

### OAI_MAIN (Main AI Coordinator)
- Coordinates all AI subsystems
- Decision making framework
- AI state management

### Specialized AI Modules
- **OAI_ACT/ACT2**: Action planning and execution
- **OAI_ATTK**: Attack strategies
- **OAI_BUIL**: Building construction decisions
- **OAI_DEFE**: Defense strategies
- **OAI_DIPL**: Diplomacy and negotiations
- **OAI_ECO**: Economic management
- **OAI_MILI**: Military operations
- **OAI_MONS**: Monster AI behavior
- **OAI_SPY**: Espionage operations
- **OAI_TOWN**: Town management
- **OAI_TRAD**: Trade operations
- **OAI_UNIT**: Unit control and movement

## Entity Management

### OUNIT (Unit System)
- Unit creation and destruction
- Unit movement and pathfinding
- Unit combat and abilities
- Unit AI integration

### OFIRM (Building System)
- Building construction
- Building operations
- Building AI
- Resource production

### OTOWN (Town System)
- Town management
- Population growth
- Resource collection
- Town AI

## Resource Management

### ORES (Resource System)
- Resource types (gold, materials, etc.)
- Resource storage
- Resource transactions

### OSPRITE (Sprite Resources)
- Sprite loading and caching
- Animation management
- Graphics resource access

## Audio Subsystem

### OpenAL Integration
- Music playback
- Sound effect management
- Audio stream handling
- Cross-platform audio support

## Input Subsystem

### OMOUSE (Mouse Input)
- Mouse position tracking
- Click detection
- Mouse cursor management

### Keyboard Input
- Key event handling
- Input mapping
- Command processing

## Utility Subsystems

### File I/O
- Game save/load
- Configuration files
- Resource file access

### Localization
- Multi-language support
- Text translation
- Locale management
