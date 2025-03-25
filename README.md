# Player

Player is a c++ program. Given a starting location, a heading, and a speed the Player will travel until interrupted.  Periodically, Player will output its position in the form of a GeoJSON doc.


## Requirements

- git: the version control system
- c++: the compiler
- make: the c++ build tool
- cmake: a Makefile builder
- conan: a cmake file builder




## Building

- Clone the player repo
- CD into the Player directory
- Setup conan
- - conan profile detect --force
- Run the conan install
- - conan install . --output-folder=build --build=missing
- Setup cmake
- - cd build
- - cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
- Run the Makefile to build the Player executable
- - make
- Execute player
- - ./player

## Operations

By default,  Player starts at  39.7811° N, 84.1104° W and travels East at 150 KPH.  

Initial condition is configurable by Environment variables.

| Environment Variable | Description |
| - | - |
| PLAYER_NAME | the player's name |
| PLAYER_LATITUDE_DEG | the player's intial latitude |
| PLAYER_LONGITUDE_DEG | the player's initial longitude |
| PLAYER_ALTITUDE_DEG | the player's initial altitude |
| PLAYER_BEARING_DEG | the player's direction of travel in compass degrees |
| PLAYER_RATE | the player's rate of travel in KPH |

### Example: A a batch file to specify a player's initial position/velocity

```
#!/bin/bash

export PLAYER_NAME="Tom"
export PLAYER_LATITUDE_DEG="20.3"
export PLAYER_LONGITUDE_DEG="45.6"
export PLAYER_ALTITUDE_DEG="78.9"

export PLAYER_BEARING_DEG="45.2"
export PLAYER_RATE="100.1"

./player

```

