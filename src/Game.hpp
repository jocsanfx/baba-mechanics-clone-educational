#ifndef GAME_HPP
#define GAME_HPP

#include "Common.hpp"
#include "Level.hpp"

class Game {
 private:
  std::string title = "";
  Level level;

  int window_width = 0;
  int window_height = 0;
  int fps = 0;
  GameState state = GameState::playing;

 public:
  // Inicializa parametros de la clase para la ventana
  void init();
  // Loop del juego
  void run();
  // Liberar memoria
  void destroy();
};

#endif  // GAME_HPP