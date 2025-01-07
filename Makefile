build:
		g++ -Wall -std=c++17 -I"./libs/" src/Game/*.cpp src/Logger/*.cpp src/*.cpp -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua -o ShibeEngine;

run:
		./ShibeEngine

clean:
		rm ShibeEngine
