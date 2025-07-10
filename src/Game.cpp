#include "Game.hpp"

// Game handle
void Game::init() {
  this->title = "Baba is you";
  this->window_width = 800;
  this->window_height = 800;
  this->fps = 60;

  InitWindow(this->window_width, this->window_height, this->title.c_str());
  SetTargetFPS(this->fps);

  this->level.loadTextures();
  this->level.loadLevel();
  this->level.adjustToFitScreen();
}

void Game::run() {
  while (!WindowShouldClose()) {
    if (this->state == GameState::playing) {
      this->state = this->level.handleInput();
    }
    level.draw(state);
    if (this->state == GameState::lost && IsKeyPressed(KEY_ENTER)) {
      this->level.loadLevel();
      this->state = GameState::playing;
    }
  }
}

void Game::destroy() {
  UnloadTexture(this->level.tiles.texture);
  CloseWindow();
}