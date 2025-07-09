#ifndef GAME_HPP
#define GAME_HPP

#include <raylib.h>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <utility>
#include <deque>
#include <iostream>

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

class Game {
 private:
  std::string title = "";

  int window_width = 0;
  int window_height = 0;
  int fps = 0;

  TileData tiles;
  LevelData level;

 public:
  // Inicializa parametros de la clase para la ventana
  void init();
  // Loop del juego
  void run();
  // Liberar memoria
  void destroy();

 private:
  // Loading
  void loadTextures();
  void loadLevel();
  void loadTileIDs();
  // Input
  void handle_input();
  // Update
  void update();
  // Draw
  void draw();
  void drawTail(int id, int row, int col);
  void drawMap();
  void drawEntities();
};

#endif // GAME_HPP