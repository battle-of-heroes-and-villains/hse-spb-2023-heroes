![](https://github.com/battle-of-heroes-and-villains/hse-spb-2023-heroes/blob/main/client/game_examples/caption.png)

---

[//]: # (Read this in other languages: [Русский]&#40;./README.ru.md&#41;)


The main aim of the game is to defeat your opponent's units using your own army and spells. During each turn, you can
either move your unit, engage in combat with your opponent, or use a spell – a special magical property that can help
you achieve your objective.

Each player has their own set of spells. With these spells, you can heal your units or
destroy a cell (potentially killing your opponent's unit as a result). Using a spell costs mana, which accumulates over
the course of the game. Therefore, you should carefully consider your strategy before using a spell. Additionally, note
that the map changes dynamically and can be destroyed during gameplay, so you need to be mindful of your movements.

Finally, the most important thing to keep in mind is to enjoy the Battle of Heroes and Villains and have fun!

## Game examples

---

### Screenshots

Start state of the game.

![](https://github.com/battle-of-heroes-and-villains/hse-spb-2023-heroes/blob/main/client/game_examples/game_start.png)

Move your unit.

![](https://github.com/battle-of-heroes-and-villains/hse-spb-2023-heroes/blob/main/client/game_examples/moving.png)

Attack opponent's unit.

![](https://github.com/battle-of-heroes-and-villains/hse-spb-2023-heroes/blob/main/client/game_examples/attack.png)

View unit's characteristics (target to unit and press 'i' to see them).

![](https://github.com/battle-of-heroes-and-villains/hse-spb-2023-heroes/blob/main/client/game_examples/unit_info.png)

Apply spell.

![](https://github.com/battle-of-heroes-and-villains/hse-spb-2023-heroes/blob/main/client/game_examples/spell.png)

### Spells list

- Break cell
- Heal cell
- Attack unit
- Heal unit

## Installation

---

### Dependencies and submodules

- [SFML](https://www.sfml-dev.org)
- [gRPC](https://github.com/grpc/grpc)
- [sqlite](https://www.sqlite.org/download.html)
- [Boost](https://boostorg.jfrog.io/artifactory/main/release/1.82.0/source/)
- [CMake](https://cmake.org/download/)

### Setup

After installing all dependencies, you can build the project executable file through Cmake. The game is cross-platform,
however, there may be problems with installing dependencies on some systems (windows), and specific cases of such
problems can be solved by finding solutions online.

To start the game, run the server on your machine. Then connect to port 50051 via local network. Match making and board
generation will happen automatically. The local history of games and user information will be stored in a database.