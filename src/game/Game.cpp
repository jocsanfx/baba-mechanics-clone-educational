// Copyright 2025 Jocsan Fernández <jocsan.fernandezsalas@ucr.ac.cr>
// && Isaac Araya <isaac.arayaquesada@ucr.ac.cr>
// && May Retana <may.retana@ucr.ac.cr>

// Main game execution file
// Contains the implementation of the Game class methods

#include "game/Game.hpp"
#include "config/Config.hpp"

/**
 * @brief Initializes the window and FPS, then loads the first level.
 */
void Game::init() {
  this->title = GAME_TITLE;
  this->window_width = GAME_WW;
  this->window_height = GAME_WH;
  this->fps = GAME_FPS;

  InitWindow(this->window_width, this->window_height, this->title.c_str());
  SetTargetFPS(this->fps);

  // Load textures and the initial level
  this->level.loadTextures();
  this->level.loadLevel(this->level_counter);
  this->level.adjustToFitScreen();
}

/**
 * @brief Main game loop.
 * 
 * Handles user input, renders the level, and switches between the playing,
 * won, and lost game states.
 */
void Game::run() {
  while (!WindowShouldClose()) {
    if (this->state == GameState::playing) {
      this->state = this->level.handleInput();
    }

    // Draw the current level state
    level.draw(state, this->level_counter);

    // Restart the current level if the player lost and presses Enter or R
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

/**
 * @brief Releases resources and closes the game window.
 */
void Game::destroy() {
  UnloadTexture(this->level.tiles.texture);
  CloseWindow();
}
