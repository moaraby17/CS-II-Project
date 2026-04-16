# Dungeon Realms (Milestone 1)

## Project Overview
Dungeon Realms is a turn-based strategy prototype built with **C++ and Qt Widgets** for CSCE 1101 (Fundamentals of Computing II).
This Milestone 1 submission includes one complete playable level with player movement, enemy AI turns, traps, combat, health, restart-on-death, and a win condition.

## Features Implemented in Milestone 1
- Single playable **Level 1** on a 10x10 grid.
- Keyboard-controlled player (arrow keys or WASD).
- Turn-based flow: player acts, then enemies take automatic turns.
- Two enemies with simple chase-and-attack behavior.
- Walls (blocked tiles), traps (damage tiles), and goal tile.
- Health system for player and enemies.
- Player attacks by moving into an enemy tile.
- Player death triggers automatic Level 1 restart.
- Win condition: defeat all enemies and reach goal tile.
- “Level 1 Complete” popup message when objective is met.

## Build Instructions
### Requirements
- CMake 3.16+
- C++17 compiler
- Qt 5 or Qt 6 with Widgets module

### Build (Linux/macOS/Windows with terminal)
```bash
cd DungeonRealms
cmake -S . -B build
cmake --build build
```

## Run Instructions
After building:
```bash
./build/DungeonRealms
```

> On Windows, executable may be `build\\Debug\\DungeonRealms.exe` or similar depending on generator.

## Controls
- **Move Up:** Up Arrow or `W`
- **Move Down:** Down Arrow or `S`
- **Move Left:** Left Arrow or `A`
- **Move Right:** Right Arrow or `D`

Gameplay notes:
- Moving into a wall is blocked.
- Moving into an enemy tile attacks that enemy.
- Stepping on a trap damages player HP.
- After each player action, living enemies move/attack.

## Brief Class Overview
- **MainWindow**: Builds UI, handles keyboard input, and renders board/status.
- **Game**: Main game loop logic for turns, combat, win/loss checks, and restart.
- **Level**: Stores map data (tiles), start/goal positions, and enemy placements.
- **Entity**: Base class for position/health.
- **Player**: Derived from Entity; contains player attack value.
- **Enemy**: Derived from Entity; contains enemy attack value and uses Game AI logic.
- **Tile / TileType**: Represents floor, wall, trap, and goal tiles.

## Milestone 1 UML
PlantUML source file is provided at:
- `uml/dungeon_realms_milestone1.puml`

## How this satisfies Milestone 1
- Includes an initial UML class diagram matching the implemented classes.
- Provides a fully functioning Level 1 with:
  - interactive player control,
  - enemy automated behavior,
  - hazard and obstacle tiles,
  - HP and combat,
  - restart on death,
  - visual updates,
  - clear level completion message.
