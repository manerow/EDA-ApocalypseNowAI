# EDA Subject Project
Programming an AI player.
## Getting Started
This is the EDA subject course project which consists in programming an AI player for the game Apocalypse Now 2018. The information about the game can be found in _ENUNCIAT.pdf_.
The main objective of the game is to use a Breadth-First Search Algorithm in order to guide the player through the map. The algorithm applied is A*. The code of the AI is located at _AIOakSaver.cc_
### Dependencies
```
g++ compiler
build-essential
```
### Running the project and simulatig the game
The project comes with a makefile. In order to compile and simulate the game, do the following steps:
```
Unpack or clone the project
$ tar -zxvf Game.tar.gz
$ make all
$ ./Game AIOakSaver Demo Demo Demo -s 30 -i default.cnf -o default.out
```
In order to visualize an interactive version of the simulated game:
```
Open /Viewer/viewer.html in your default browser
Choose default.out from the root of the game directory
```

