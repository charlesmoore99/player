# Player

Given a starting location, a heading, and a speed the Player will 'travel' until interrupted.  Periodically, Player will output its position in the form of a GeoJSON doc.

Player is a c++ program.  It is build using the Conan c++ package manager and the CMake build system generator.

## Requirements

- git: the version control system
- c++: the compiler
- make: a build tool
- cmake: a build system generator
- conan: a c++ package manager


## Building

* Clone the player repo
* CD into the Player directory
* Setup conan
        
         conan profile detect --force

* Run the conan install

        conan install . --output-folder=build --build=missing

- Setup cmake
        
        cmake . -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake  -DCMAKE_POLICY_DEFAULT_CMP0091=NEW -DCMAKE_BUILD_TYPE=Release

- Run the Makefile to build the Player executable

        make

- Execute player

        ./player


## Operations

By default,  Player starts at  39.7811° N, 84.1104° W and travels East at 150 KPH.  

The default condition is configurable by Environment variables.

| Environment Variable | Description |
| - | - |
| PLAYER_NAME | the player's name |
| PLAYER_LATITUDE_DEG | the player's intial latitude |
| PLAYER_LONGITUDE_DEG | the player's initial longitude |
| PLAYER_ALTITUDE_DEG | the player's initial altitude |
| PLAYER_BEARING_DEG | the player's direction of travel in compass degrees |
| PLAYER_RATE | the player's rate of travel in KPH |

### Example: A batch file to specify a player's initial position/velocity

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

## Dockerized player

Player can be a packaged into a lightweight container

### Build the player-docker image

```
docker build -t player-docker .
```


### Run the player-docker image

#### To start a player with default options:
```
docker run -t --name player player-docker
```


#### To start a custom player from the command line:
```
docker run \
    -t \
    --name player \
    -e PLAYER_NAME="BUBBA" \
    -e PLAYER_LATITUDE_DEG="21.315603" \
    -e PLAYER_LONGITUDE_DEG="-157.858093" \
    -e PLAYER_BEARING_DEG="0.0" \
    player-docker
```

#### To start a custom player from with an environment file:
1. create a file with the environment variable you want to set. 

Example: the file 'player-config' 
```
PLAYER_NAME=Tom Sawyer
PLAYER_LATITUDE_DEG=39.7084
PLAYER_LONGITUDE_DEG=-91.3585
PLAYER_ALTITUDE_DEG=0
PLAYER_BEARING_DEG=0
PLAYER_RATE=4
```

2. specify the config file when stargin the container

Example:
```
docker run -t --env-file player-config player-docker
```