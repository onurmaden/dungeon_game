### README

# Dungeon Game

## Overview

Dungeon Game is a terminal-based dungeon crawler written in C. Players navigate through a procedurally generated dungeon, encountering various creatures and collecting items, with the ultimate goal of reaching the dungeon's exit.

## Features

- **Interactive Gameplay:** Engage with creatures, collect items, and explore rooms.
- **Save and Load Functionality:** Save your progress and continue your adventure later.
- **Terminal-Based Interface:** Experience the game directly in your terminal.

## Getting Started

### Prerequisites

- A C compiler (e.g., `gcc`).
- Make utility.
- Compatible terminal emulator.

### Installation

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/onurmaden/dungeon_game.git
   ```
2. **Navigate to the Project Directory:**
   ```bash
   cd dungeon_game
   ```
3. **Build the Game:**
   ```bash
   make
   ```

### Running the Game

After building the project, execute the game with:
```bash
./dungeon_game
```

Follow the on-screen instructions to navigate through the dungeon.

## Controls

- move <direction>: Move to a different room if possible.
- look: Display the description of the current room and items or creatures in it.
- inventory: List items the player has collected.
- pickup <item>: Add an item to the inventory if present in the room.
- attack: Fight a creature if present in the room.
  
### Menu Commands:
- save <filepath> : saves the current state in to the given file
- load <filepath> : loads the state from the given file
- exit

## Objectives

- **User Interaction:** Develop a responsive interface for player inputs and game feedback.
- **Data Management:** Handle game state data to support features like saving and loading progress.

## System Architecture

The game is structured into several modules, each responsible for different aspects of functionality:

1. **Main Module (`main.c`):**
   - Initializes the game.
   - Manages the game loop.
   - Handles user inputs and game termination.

2. **Game Logic Module (`game.c` / `game.h`):**
   - Contains core game mechanics.
   - Manages game state transitions.
   - Coordinates interactions between player, creatures, and items.

3. **Player Module (`player.c` / `player.h`):**
   - Manages player attributes and actions.
   - Processes player movements and interactions.

4. **Creature Module (`creature.c` / `creature.h`):**
   - Defines creature behaviors and attributes.
   - Handles creature interactions with the player.

5. **Item Module (`item.c` / `item.h`):**
   - Manages in-game items and their effects.
   - Handles item pickups and usage.

6. **Room Module (`room.c` / `room.h`):**
   - Defines room properties and connections.
   - Manages room generation and layout.

7. **Save/Load Module (`save.c` / `save.h`):**
   - Implements functionality to save and load game states.
   - Handles file operations related to game data persistence.

## Key Features

- **Interactive Gameplay:**
  - Allows player movement through the dungeon.
  - Supports interactions with creatures and items.
  - Implements combat mechanics and health management.

- **Save and Load Functionality:**
  - Enables players to save their progress.
  - Allows loading of saved games to continue adventures.


## Conclusion

Dungeon Game serves as a comprehensive project demonstrating the implementation of a terminal-based game in C. Through procedural generation, interactive gameplay, and data management features, the project offers a solid foundation for further development and enhancement in the realm of dungeon crawler games.
