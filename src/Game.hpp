#ifndef GAME_HPP
#define GAME_HPP

#include "Level.hpp"

class Game {
 private:
  std::string title = "";
  Level level;
  int window_width = 0;
  int window_height = 0;
  int fps = 0;
  int level_counter = 0;
  GameState state = GameState::playing;

 public:
  void init();
  void run();
  void destroy();
};

#endif  // GAME_HPP