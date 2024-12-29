#include "Game.hpp"
#include <iostream>

Game::Game() {
  std::cout << "Game Constructor Called" << std::endl;
}

Game::~Game() {
  std::cout << "Game Destructor Called" << std::endl;
}

void Game::Initialize() {

};

void Game::Run() {
  while (1) {
    ProcessInput();
    Update();
    Render();
  }

};

void Game::ProcessInput() {

};

void Game::Update() {

};

void Game::Render() {

};

void Game::Destroy() {

};
