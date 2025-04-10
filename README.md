# Player

Player simulates a object traveling around the globe.  Given a starting location, heading, and speed the Player will 'travel' until interrupted.  

The Player outputs its position as a GeoJSON doc to STDOUT every second.

Player is a C++ program.  It is built using the Conan package manager and the CMake build system generator.

## Build Requirements

- git: the version control system
- c++: the compiler
- make: the build tool
- cmake: the build system generator
- conan: the c++ package manager

### Example of an Ubuntu Setup

        apt install -y cmake build-essential python3 python3-pip git
        pip3 install --break-system-packages conan


## Building

* Clone the player repo
* CD into the Player directory
* Setup conan
        
         conan profile detect --force

* Run the conan install

        conan install . --build=missing

- Setup the cmake build
  
        cd build
        
        cmake .. -DCMAKE_TOOLCHAIN_FILE=Release/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release

- Run the Build to create the Player executable and Unit Tests

        cmake --build .

- Run Unit Tests

        ./test_player

- Run player

        ./player


## Operations

By default,  Player starts at  39.7811° N, 84.1104° W and travels East at 150 KPH.  

Player' initial conditions are configurable by Environment variables.

| Environment Variable | Description |
| - | - |
| PLAYER_NAME | the player's name |
| PLAYER_LATITUDE_DEG | the player's initial latitude in decimal degrees |
| PLAYER_LONGITUDE_DEG | the player's initial longitude  in decimal degrees|
| PLAYER_ALTITUDE_M | the player's altitude in meters|
| PLAYER_BEARING_DEG | the player's direction of travel in compass degrees |
| PLAYER_RATE | the player's rate of travel in KPH |

### Example: A batch file to specify a player's initial position/velocity

```
#!/bin/bash

export PLAYER_NAME="Tom"
export PLAYER_LATITUDE_DEG="20.3"
export PLAYER_LONGITUDE_DEG="45.6"
export PLAYER_ALTITUDE_M="78.9"

export PLAYER_BEARING_DEG="45.2"
export PLAYER_RATE="100.1"

./player

```

### The service port

Player's velocity vector can be adjusted by POSTing a JSON document to port 8080.


```
{
        "bearing": 181.0, 
        "kph": 250.0 
}
```


## Dockerized player

Player can be a packaged into a lightweight container.

### Build the player-docker image

```
docker build -t player-docker .
```

### Run the player-docker image

#### To start a player from the command line with default options:
```
docker run -t --name player -p 8080:8080 player-docker
```


#### To start a player from the command line with custom options:
```
docker run \
    -t \
    --name player \
    -e PLAYER_NAME="BUBBA" \
    -e PLAYER_LATITUDE_DEG="21.315603" \
    -e PLAYER_LONGITUDE_DEG="-157.858093" \
    -e PLAYER_BEARING_DEG="0.0" \
    -p 8080:8080 \
    player-docker
```

#### To start a player from the command line with custom options specified in an environment file:
1. create a file with the environment variable you want to set. 

Example: the file 'player-config' 
```
PLAYER_NAME=Tom Sawyer
PLAYER_LATITUDE_DEG=39.7084
PLAYER_LONGITUDE_DEG=-91.3585
PLAYER_ALTITUDE_M=0
PLAYER_BEARING_DEG=0
PLAYER_RATE=4
```

2. specify the config file when staring the container

Example:
```
docker run -t -p 8080:8080 --env-file player-config player-docker
```

## GitLab Pipeline

This project comes with a .gitlab-ci.yml that will build/package the project automatically (in Gitlab).  the following artifacts are produced:

| Stage | Artifact | Description |
| - | - | - |
| BUILD | artifacts.zip: build/player | the player executable |
| TEST | junit.xml.gz  | Results of unit tests |
| PACKAGE-IMAGE | registry.gitlab.com/.../player | The player executable packaged in a docker image.  Accessible in the docker container registry. |

