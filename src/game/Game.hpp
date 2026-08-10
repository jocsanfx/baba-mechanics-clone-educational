// Copyright 2025 Jocsan Fernández <jocsan.fernandezsalas@ucr.ac.cr>
// && Isaac Araya <isaac.arayaquesada@ucr.ac.cr>
// && May Retana <may.retana@ucr.ac.cr>
#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include "config/Config.hpp"
#include "level/Level.hpp"

/**
 * @class Game
 * @brief Main class that manages the flow of the Baba Is You game
 *
 * Initializes the window, controls the main game loop, manages levels and
 * states, and destroys resources on exit
 */
class Game {
 private:
  // Game window title
  std::string title = "";
  // Object representing the current level
  Level level = {};
  // Window width
  int window_width = DEFAULT_INIT_SIZE;
  // Window height
  int window_height = DEFAULT_INIT_SIZE;
  // Game frames per second
  int fps = DEFAULT_INIT_FPS;
  // Current level number
  int level_counter = MIN_LEVEL;
  // Current game state (playing, won, or lost)
  GameState state = GameState::playing;

 public:
  /**
   * @brief Initializes the window and loads textures and the first level
   */
  void init();

  /**
   * @brief Runs the main game loop
   */
  void run();

  /**
   * @brief Releases resources and closes the game window
   */
  void destroy();
};

#endif  // GAME_HPP
