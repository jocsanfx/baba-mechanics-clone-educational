// Copyright 2025 Jocsan Fernández <jocsan.fernandezsalas@ucr.ac.cr>
// && Isaac Araya <isaac.arayaquesada@ucr.ac.cr>
// && May Retana <may.retana@ucr.ac.cr>
#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include "Config.hpp"
#include "Level.hpp"

/**
 * @class Game
 * @brief Clase principal que administra el flujo del juego Baba Is You
 *
 * Se encarga de inicializar la ventana, controlar el bucle principal del juego,
 * manejar los niveles, estados y destruir los recursos al finalizar
 */
class Game {
 private:
  // Título de la ventana del juego
  std::string title = "";
  // Objeto que representa el nivel actual
  Level level = {};
  // Ancho de la ventana
  int window_width = DEFAULT_INIT_SIZE;
  // Alto de la ventana
  int window_height = DEFAULT_INIT_SIZE;
  // Cuadros por segundo del juego
  int fps = DEFAULT_INIT_FPS;
  // Número del nivel actual
  int level_counter = MIN_LEVEL;
  // Estado actual del juego (jugando, ganado o perdido)
  GameState state = GameState::playing;

 public:
  /**
   * @brief Inicializa la ventana, carga texturas y el primer nivel
   */
  void init();

  /**
   * @brief Ejecuta el bucle principal del juego
   */
  void run();

  /**
   * @brief Libera recursos y cierra la ventana del juego
   */
  void destroy();
};

#endif  // GAME_HPP
