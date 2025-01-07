# -Wfatal-errors will only show the first error
#  as opposed to showing all at once

CC = g++
COMPILER_FLAGS = -Wall -Wfatal-errors
LANG_STD = -std=c++17
INCLUDE_PATHS = -I"./libs/"
SOURCE_FILES = src/Game/*.cpp src/Logger/*.cpp src/*.cpp
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua
OUTPUT = ShibeEngine

build:
		$(CC) $(COMPILER_FLAGS) $(LANG_STD) $(INCLUDE_PATHS) $(SOURCE_FILES) $(LINKER_FLAGS) -o $(OUTPUT);

run:
		./$(OUTPUT)

clean:
		rm $(OUTPUT)
