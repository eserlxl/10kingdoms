# Class Hierarchy

Generated: $(date)

## Main Inheritance Trees

### Sprite Hierarchy
```
Sprite
├── Unit (sprite with game unit behavior)
│   ├── UnitGod
│   ├── UnitMonster
│   ├── UnitVehicle
│   ├── UnitExpCart
│   ├── UnitCaravan
│   └── UnitMarine
├── Bullet
│   ├── BulletHoming
│   ├── BulletFlame
│   └── Projectile
├── Tornado
└── Effect
```

### Firm Hierarchy
```
Firm (base building class)
├── FirmBase
├── FirmInn
├── FirmMine
├── FirmMarket
├── FirmWar
├── FirmFactory
├── FirmHarbor
├── FirmResearch
├── FirmCamp
└── FirmMonster
```

### Array/Container Hierarchy
```
DynArray (dynamic array base)
├── DynArrayB
│   ├── FirmArray
│   ├── TownArray
│   ├── NationArray
│   ├── SiteArray
│   ├── RockArray
│   ├── RebelArray
│   ├── SpyArray
│   ├── FirmDieArray
│   └── MLink
├── SpriteArray
│   ├── UnitArray
│   ├── BulletArray
│   └── TornadoArray
└── GameFileArray
```

### File I/O Hierarchy
```
File (base file class)
├── Database
├── Resource
├── ResourceIdx
│   └── ImageRes
├── FileTxt
└── LongLog

InputStream
├── FileInputStream
└── MemInputStream

OutputStream
└── FileOutputStream

AudioStream
└── WavStream
```

### Game Entity Hierarchy
```
NationBase
└── Nation

AudioBase
└── OpenALAudio

Matrix
├── MapMatrix
└── ZoomMatrix
```

### UI Components
```
VBrowse
└── VBrowseIF
```

## Key Base Classes

- **Sprite**: Base class for all visual game entities (units, bullets, effects)
- **Firm**: Base class for all buildings and structures
- **Unit**: Base class for all game units (soldiers, workers, etc.)
- **DynArray**: Base class for dynamic arrays
- **File**: Base class for file operations
- **Resource**: Base class for game resources

## Design Patterns

- **Inheritance**: Extensive use of inheritance for code reuse
- **Polymorphism**: Virtual functions used for derived class behavior
- **Array Containers**: Specialized array classes for different entity types
- **Resource Management**: Hierarchical resource loading system
