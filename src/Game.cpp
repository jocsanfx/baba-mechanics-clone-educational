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

    BeginDrawing();
    ClearBackground(RAYWHITE);
    this->level.drawMap();
    this->level.drawEntities();

    if (this->state == GameState::won) {
      DrawText("¡Ganaste!", 10, 10, 20, GREEN);
    } else if (this->state == GameState::lost) {
      DrawText("¡Perdiste!", 10, 10, 20, RED);
    }

    EndDrawing();

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