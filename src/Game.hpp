#ifndef GAME_HPP
#define GAME_HPP

#include "Config.hpp"
#include "Level.hpp"

class Game {
 private:
  std::string title = "";
  Level level;
  int window_width = DEFAULT_INIT_SIZE;
  int window_height = DEFAULT_INIT_SIZE;
  int fps = DEFAULT_INIT_FPS;
  int level_counter = MIN_LEVEL;
  GameState state = GameState::playing;

 public:
  void init();
  void run();
  void destroy();
};

#endif  // GAME_HPP