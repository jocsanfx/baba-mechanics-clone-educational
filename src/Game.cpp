#include "Game.hpp"
#include "Config.hpp"

void Game::init() {
  this->title = GAME_TITLE;
  this->window_width = GAME_WW;
  this->window_height = GAME_WH;
  this->fps = GAME_FPS;

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

    level.draw(state, this->level_counter);

    if ((this->state == GameState::lost && IsKeyPressed(KEY_ENTER)) ||
      IsKeyPressed(KEY_R)) {
      this->level.loadLevel(this->level_counter);
      this->state = GameState::playing;
    } else if (this->state == GameState::won && IsKeyPressed(KEY_ENTER)) {
      if (this->level_counter < MAX_LEVEL) {
        this->level_counter++;
        this->level.loadLevel(this->level_counter);
        this->state = GameState::playing;
      } else {
        level.draw(state, this->level_counter);
      }
    }
  }
}

void Game::destroy() {
  UnloadTexture(this->level.tiles.texture);
  CloseWindow();
}