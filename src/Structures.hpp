#ifndef STRUCTURES_HPP
#define STRUCTURES_HPP

#include <raylib.h>
#include <map>
#include <string>
#include <utility>
#include <vector>

struct TileData {
  Texture2D texture;
  int rows;
  int cols;
  int width;
  int height;
};

struct Entity {
  int id;
  std::pair<int, int> pos;
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
  std::vector<Entity> entities;
};

#endif  // STRUCTURES_HPP
