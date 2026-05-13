# Dungeon Realms: A Turn-Based Strategy Game

## Short Description
Dungeon Realms is a small **C++ Qt Widgets** turn-based strategy game. The player controls one adventurer on a 10x10 dungeon grid, fights enemies, avoids traps, opens locked doors, and clears five levels of increasing difficulty.

The implementation is intentionally simple and student-friendly: the board is made from readable string maps, the game loop is turn based, and the main classes have clear responsibilities.

## Game Objective
1. Move through the dungeon as the blue player character.
2. Defeat every enemy on the current level.
3. Reach the green treasure/exit tile marked `X`.
4. Clear all five levels to win the game.

If the player health reaches 0, the current level restarts from the beginning.

## Controls
- **Move:** Arrow keys or `W`, `A`, `S`, `D`
- **Attack adjacent enemy:** `Space` or `F`
- You can also attack by trying to move into an enemy tile.

## Tile and Character Legend
- `P` / blue circle: player adventurer
- `E` / red circle: basic enemy that chases the player
- `A` / purple circle: archer enemy that can shoot when aligned with the player
- `B` / dark red circle: brute enemy with more health and damage
- `#` / gray tile: wall, blocks movement
- `^` / orange tile: trap, damages the player
- `K` / yellow tile: key pickup
- `D` / brown tile: locked door, opens only after collecting a key
- `X` / green tile: treasure/exit

## Features Implemented
- C++17 project using Qt Widgets for all GUI rendering.
- Object-oriented design with separate classes for the window, game controller, level, tiles, player, enemies, and entities.
- Five playable levels with increasing difficulty.
- Grid-based movement using keyboard input.
- Turn-based loop: after the player moves or attacks, enemies automatically take a turn.
- Enemy AI:
  - Basic enemies move toward the player and attack when adjacent.
  - Archers shoot a projectile in a straight line when they have line of sight.
  - Brutes have more health and hit harder.
- Health system for player and enemies.
- Damage from enemy attacks and traps.
- Death handling that restarts the current level.
- Walls, traps, locked doors, keys, and exits.
- HUD showing level number, level name, health, key status, enemies remaining, and status messages.
- Projectile animation for player attacks and archer attacks.
- Victory message after completing Level 5.

## How to Build and Run

### Requirements
- CMake 3.16 or newer
- C++17 compiler
- Qt 5 or Qt 6 with the Widgets module installed

### Build
```bash
cd DungeonRealms
cmake -S . -B build
cmake --build build
```

### Run
```bash
./build/DungeonRealms
```

On Windows, the executable may be in a generator-specific folder, such as:
```bash
build\Debug\DungeonRealms.exe
```

## Class Overview
- **MainWindow**: Qt main window. It handles keyboard input, draws the board with `QGraphicsScene`, updates the HUD, and shows popup messages.
- **Game**: Main game controller. It manages the current level, player turns, enemy turns, combat, projectiles, death/restart, level progression, and victory.
- **Level**: Stores one dungeon layout. It loads levels from simple string maps and owns the tile grid, enemy list, player start, and exit position.
- **Tile / TileType**: Represents each board square, such as floor, wall, trap, locked door, key, or goal.
- **Entity**: Base class for objects with position and health.
- **Player**: Derived from `Entity`. Stores attack damage and key state.
- **Enemy**: Derived from `Entity`. Stores enemy type, health, damage, and display letter.
- **Projectile**: Small value class used by the window when drawing straight-line projectile animations.

## Known Limitations
- Save/load is not implemented because the final milestone requirements were focused on gameplay completion.
- Art is intentionally simple: colored shapes and letters are used instead of external sprite files.
- Enemy AI is basic and easy to explain: enemies either attack, shoot if aligned, or step toward the player.
