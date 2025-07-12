#include "Game.hpp"
#include "Config.hpp"

void Game::init() {
  this->title = gameTitle;
  this->window_width = gameWW;
  this->window_height = gameWH;
  this->fps = gameFPS;

  InitWindow(this->window_width, this->window_height, this->title.c_str());
  SetTargetFPS(this->fps);

  this->level.loadTextures();
  this->level.loadLevel(this->level_counter);
  this->level.adjustToFitScreen();
}

void Game::run() {
  while (!WindowShouldClose()) {

    if (this->state == GameState::playing) {
      this->state = this->level.handleInput();
    }

    level.draw(state);

    if ((this->state == GameState::lost && IsKeyPressed(KEY_ENTER)) || IsKeyPressed(KEY_R)) {
      this->level.loadLevel(this->level_counter);
      this->state = GameState::playing;
    } else if (this->state == GameState::won) {
      if (this->level_counter < MAX_LEVEL) {
        this->level_counter++;
        this->level.loadLevel(this->level_counter);
        this->state = GameState::playing;
      } else {
        level.draw(state);
      }
    }
  }
}

void Game::destroy() {
  UnloadTexture(this->level.tiles.texture);
  CloseWindow();
}