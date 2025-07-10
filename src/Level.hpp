#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <raylib.h>

#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <stack>

#include "Common.hpp"

// Basicamente es la representacion del sprite sheet dentro del codigo
struct TileData {
  // Textura de rylib que contiene el sprite sheet
  Texture2D texture;
  // Filas del sprite sheet
  int rows;
  // Columnas del sprite sheet
  int cols;
  // Ancho de cada sprite
  int width;
  // Altura de cada sprite
  int height;
};

// En la buena teoria esto es la base para ECS
// representa todas las entidades del juego
struct Entity {
  int id;
  // Posicion en el mapa
  std::pair<int, int> pos;
  // Tipo de entidad
  std::string type;
  // Mapa para los sistemas que se le apliquen a la entidad
  // la clave del map es el componente (digamos que la funcionalidad)
  // el bool basicamente indica si lo tiene o no (como si es empujable o no)
  std::map<std::string, bool> tags;
  char symbol;
};

struct LevelData {
  int rows;
  int cols;
  int cell_width;
  int cell_heigth;
  std::vector<std::vector<char>> mapa;
  std::map<char, int> tiles_id;
  // Todas las entidades del nivel
  std::vector<Entity> entities;
};

class Level {
 private:
  int frameCount = 0;
  int count = 0;
  float offsetX = 0;
  float offsetY = 0;
  friend class Game;

  TileData tiles;
  LevelData level;

  std::stack<std::pair<std::vector<std::vector<char>>, std::vector<Entity>>> history;

 private:
  // Loading
  void loadTextures();
  void loadLevel();
  void loadTileIDs();
  // Input
  GameState handleInput();
  // Update
  void update();
  void undo();
  // Draw
  void draw(GameState state);
  void drawTail(int id, int row, int col);
  void drawMap();
  void drawEntities();
  // Aux
  void adjustToFitScreen();
  bool isBlocked(int row, int col);
  bool tryPush(int row, int col, int dx, int dy);
  std::string entityString(char c);
};

#endif  // LEVEL_HPP