// Copyright 2025 Jocsan Fernández <jocsan.fernandezsalas@ucr.ac.cr>
// && Isaac Araya <isaac.arayaquesada@ucr.ac.cr>
// && May Retana <may.retana@ucr.ac.cr>

// Archivo principal de ejecución del juego
// Contiene la implementación de los métodos de la clase Game

#include "Game.hpp"
#include "Config.hpp"

/**
 * @brief Inicializa la ventana, el FPS y carga el primer nivel.
 */
void Game::init() {
  this->title = GAME_TITLE;
  this->window_width = GAME_WW;
  this->window_height = GAME_WH;
  this->fps = GAME_FPS;

  InitWindow(this->window_width, this->window_height, this->title.c_str());
  SetTargetFPS(this->fps);

  // Carga texturas y nivel inicial
  this->level.loadTextures();
  this->level.loadLevel(this->level_counter);
  this->level.adjustToFitScreen();
}

/**
 * @brief Bucle principal del juego.
 * 
 * Maneja entradas del usuario, renderiza el nivel, y cambia entre estados 
 * de juego: jugando, ganado y perdido.
 */
void Game::run() {
  while (!WindowShouldClose()) {
    if (this->state == GameState::playing) {
      this->state = this->level.handleInput();
    }

    // Dibuja el estado actual del nivel
    level.draw(state, this->level_counter);

    // Reinicia el nivel actual si el jugador ha perdido y presiona Enter o R
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
 * @brief Libera recursos y cierra la ventana del juego.
 */
void Game::destroy() {
  UnloadTexture(this->level.tiles.texture);
  CloseWindow();
}
