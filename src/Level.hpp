// Copyright 2025 Jocsan Fernández <jocsan.fernandezsalas@ucr.ac.cr>
// && Isaac Araya <isaac.arayaquesada@ucr.ac.cr>
// && May Retana <may.retana@ucr.ac.cr>
#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <raylib.h>

#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>
#include <stack>
#include <utility>
#include <vector>
#include <map>
#include <set>

#include "Structures.hpp"
#include "Config.hpp"

/**
 * @class Level
 * @brief Clase que gestiona la lógica y representación de un nivel del juego
 *
 * Esta clase se encarga de cargar los niveles, interpretar las reglas del juego,
 * manejar el estado de las entidades, el historial de movimientos y dibujar todo
 * en pantalla usando la biblioteca raylib
 */
class Level {
 private:
  int frameCount = 0;
  int count = 0;
  Color GREY = {21, 24, 31, 255};
  float offsetX = 0;
  float offsetY = 0;
  friend class Game;

  TileData tiles;
  LevelData level;

  std::stack<std::vector<Entity>> history;

 private:
  /** @brief Carga el sprite sheet y prepara las texturas. */
  void loadTextures();

  /**
   * @brief Carga un nivel específico desde archivo.
   * @param level_counter Número del nivel a cargar.
   */
  void loadLevel(const int level_counter);

  /** @brief Carga la asociación de caracteres con los IDs del sprite sheet. */
  void loadTileIDs();

  /**
   * @brief Procesa la entrada del jugador (teclas) y actualiza el estado del juego
   * @return GameState Estado resultante tras el movimiento (ganaste, 
   * perdiste, jugando...)
   */
  GameState handleInput();

  /** @brief Actualiza el estado interno del nivel (animaciones, 
   * reglas, etc.) 
   * 
   */
  void update();

  /** @brief Deshace el último movimiento realizado */
  void undo();

  /**
   * @brief Dibuja el estado actual del juego en pantalla
   * @param state Estado actual del juego (jugando, ganado, perdido)
   * @param level_counter Nivel actual (usado para mostrar mensajes)
   */
  void draw(GameState state, const int level_counter);

  /**
   * @brief Dibuja un tile específico en el tablero
   * @param id ID del tile en el sprite sheet
   * @param row Fila donde se dibuja
   * @param col Columna donde se dibuja
   */
  void drawTail(int id, int row, int col);

  /** @brief Dibuja todas las entidades del nivel según capas */
  void drawEntities();

  /** @brief Interpreta y aplica las reglas en base a las instrucciones del 
   * nivel 
   */
  void handleRules();

  /**
   * @brief Intenta mover una entidad en la dirección indicada
   * @param mover Referencia a la entidad a mover
   * @param dr Dirección en la fila
   * @param dc Dirección en la columna
   * @return true si el movimiento fue exitoso
   */
  bool tryMove(Entity& mover, int dr, int dc);

  /**
   * @brief Dibuja todas las entidades que coincidan con el conjunto dado
   * @param conjunct Conjunto de símbolos a dibujar
   * @param player Símbolo actual del jugador (se evita dibujarlo aquí)
   */
  void drawByLayer(const std::set<char>& conjunct, const char& player);

  /**
   * @brief Dibuja todas las entidades que tengan el símbolo especificado
   * @param symbol Símbolo de las entidades a dibujar
   */
  void drawByLayer(const char& symbol);

  /**
   * @brief Obtiene el símbolo del jugador actual
   * @return char Símbolo del jugador ("isYou")
   */
  char getCurrentPlayer();

  /** @brief Ajusta el tamaño y posición del nivel para centrarse en la 
   * pantalla 
   */
  void adjustToFitScreen();

  /**
   * @brief Indica si una celda está bloqueada por una entidad con "isStop"
   * @param row Fila a revisar
   * @param col Columna a revisar
   * @return true si la celda está bloqueada
   */
  bool isBlocked(int row, int col);

  /**
   * @brief Intenta empujar entidades en una dirección específica
   * @param row Fila inicial
   * @param col Columna inicial
   * @param dx Dirección en X
   * @param dy Dirección en Y
   * @return true si el empuje fue exitoso
   */
  bool tryPush(int row, int col, int dx, int dy);

  /**
   * @brief Cambia el tipo y símbolo de una entidad
   * @param old Símbolo actual
   * @param current Nuevo símbolo
   */
  void setSymbol(const char& old, const char& current);

  /**
   * @brief Asigna una etiqueta (tag) a entidades con cierto símbolo
   * @param c Símbolo de la entidad
   * @param a Acción (tag) a asignar
   */
  void setTag(const char& c, const char& a);

  /** @brief Procesa interacciones destructivas entre entidades */
  void processRemove();

  /**
   * @brief Actualiza la apariencia del jugador según la dirección
   * @param dir Dirección presionada ('w', 'a', 's', 'd')
   */
  void updatePlayerSprite(const char& dir);

  /**
   * @brief Devuelve los símbolos de todas las entidades en una celda
   * @param r Fila de la celda
   * @param c Columna de la celda
   * @return Vector de caracteres presentes en la celda
   */
  std::vector<char> charsAt(int r, int c);
};

#endif  // LEVEL_HPP
