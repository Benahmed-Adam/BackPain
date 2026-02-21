# BackPain

A first-person game with a surreal narrative about pizza, babies, and magical flutes. Built with C++ and Raylib.

**Note** : This project was created as part of a Game Jam, with the constraint of creating an immersive and original experience within a limited time.

## The Story

In a world where reality bends at the seams, you find yourself caught in an ancient conflict between two factions:

**The Pizza Man** - A mysterious delivery man whose back pain can only be soothed by magical flutes. He claims these instruments were stolen from him and promises you a magnificent pizza in exchange for their return.

**The Baby Order** - An enigmatic group of babies who stole the flutes to prevent the Pizza Man from unleashing chaos upon the world. They believe silence is the only defense against his malevolent power.

The truth is yours to discover. Who will you trust?

## Gameplay

### Core Mechanics

- **First-Person Exploration**: Navigate maze-like map rendered in 3D
- **Dialogue System**: Interact with NPCs to uncover the story
- **Combat**: Use fireballs to defeat enemies
- **Objective**: Collect 4 magical flutes scattered throughout the map

### Two Playable Scenes

#### Scene 1: Player Scene
- You play as yourself
- Hunt for the flutes to help the Pizza Man
- Defend yourself against hostile Baby minions using fireballs
- If you die, you switch to the Baby Scene

#### Scene 2: Baby Scene  
- You play as a recruit of the Baby Order
- Protect the flutes from the Pizza Man's influence
- Beware of "Player Enemies" - corrupted versions of yourself that shoot fireballs
- No fireballs for you - survival is key

## Characters

| Character | Description |
|-----------|-------------|
| **Pizza Man** | The delivery man with mysterious powers. Wants his flutes back. Offers pizza as reward. |
| **Baby Man** | Leader of the Baby Order. Opposes the Pizza Man's plan. |
| **Baby** | Minions of the Baby Order. Chase and attack the player. |
| **Player Enemy** | Corrupted clones that shoot fireballs. |
| **Flute** | Magical instruments scattered throughout. Collect 4 to win. |

## Controls

| Key | Action |
|-----|--------|
| **ZQSD** | Move |
| **Mouse** | Look around |
| **Left Click** | Shoot fireball |
| **E** | Interact with NPCs / Pick up flutes |
| **Space** | Jump |
| **Shift** | Sprint |

## Multiple Endings

The game features **4 different endings** based on your choices:

1. **Victory Player** - Collect all 4 flutes in Player Scene.

2. **Victory Player Weird** - Kill the Baby Man leader in Player Scene.

3. **Victory Baby** - Collect all 4 flutes in Baby Scene.

4. **Victory Baby Weird** - Kill the Pizza Man in Baby Scene.

## Building

### Prerequisites

- g++
- [Raylib](https://www.raylib.com/)

### Build & Run

```bash
make        # Build the game
make run    # Build and run
make clean  # Clean everything
```

The executable is placed in `bin/main`.

## Project Structure

```
BackPain/
├── assets/
├── src/
│   ├── main.cpp
│   ├── Game.h/cpp
│   ├── Dialogue/
│   ├── Entity/
│   │   ├── Entity.h/cpp
│   │   ├── Player/
│   │   ├── PizzaMan/
│   │   ├── BabyMan/
│   │   ├── Flute/
│   │   ├── PlayerEnemy/
│   │   └── AnimatedEntity/
│   │       ├── Baby/
│   │       ├── FireBall/
│   │       └── Explosion/
│   ├── Scene/
│   │   ├── Scene.h/cpp
│   │   ├── PlayerScene/
│   │   ├── BabyScene/
│   │   ├── VictoryPlayerScene/
│   │   ├── VictoryPlayerSceneWeird/
│   │   ├── VictoryBabyScene/
│   │   └── VictoryBabySceneWeird/
│   └──  PathFinder/
├── obj/
├── bin/
├── Makefile
└── README.md
```

## How It Works

### Scene System

The game uses a scene-based architecture where each scene handles its own events, updates, and rendering. The `Game` class manages scene transitions through an index system:

```
Scene 0: PlayerScene
Scene 1: VictoryPlayerScene
Scene 2: VictoryPlayerSceneWeird  
Scene 3: BabyScene
Scene 4: VictoryBabyScene
Scene 5: VictoryBabySceneWeird
```

### Entity System

All game objects inherit from the base `Entity` class. The `AnimatedEntity` subclass adds sprite animation capabilities for characters like Babies and FireBalls.

### Map Format

Levels are defined as text maps where:
- `1` = Wall
- `0` = Empty space
- `j` = Player spawn
- `p` = Pizza Man NPC
- `B` = Baby Man NPC
- `b` = Baby minion
- `f` = Flute collectible
- `J` = PlayerEnemy (Baby Scene only)

### Collision Detection

The game uses:
- Sphere-to-sphere collision for entity interactions
- Grid-based collision for wall detection
- Radius-based player collision with the map