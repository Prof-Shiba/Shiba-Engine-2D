# -Wfatal-errors will only show the first error
#  as opposed to showing all at once

CC = g++
COMPILER_FLAGS = -Wall -Wfatal-errors
DEBUG_FLAGS = -g -O0
LANG_STD = -std=c++17
INCLUDE_PATHS = -I"./libs/"
SOURCE_FILES = src/*.cpp \
							 src/Game/*.cpp \
							 src/Logger/*.cpp \
							 src/ECS/*.cpp \
							 src/AssetManager/*.cpp \
							 # src/Components/*.cpp \
							 # src/Systems/*.cpp
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua
OUTPUT = ShibeEngine
DEBUG_OUTPUT = DEBUG_ENGINE_BUILD

build:
		$(CC) $(COMPILER_FLAGS) $(LANG_STD) $(INCLUDE_PATHS) $(SOURCE_FILES) $(LINKER_FLAGS) -o $(OUTPUT);

debug:
	$(CC) $(COMPILER_FLAGS) $(DEBUG_FLAGS) $(LANG_STD) $(INCLUDE_PATHS) $(SOURCE_FILES) $(LINKER_FLAGS) -o $(DEBUG_OUTPUT)

run:
		./$(OUTPUT)

clean:
		rm $(OUTPUT) $(DEBUG_OUTPUT)
