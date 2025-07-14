// Copyright 2025 Jocsan Fernández <jocsan.fernandezsalas@ucr.ac.cr>
// && Isaac Araya <isaac.arayaquesada@ucr.ac.cr>
// && May Retana <may.retana@ucr.ac.cr>
#ifndef STRUCTURES_HPP
#define STRUCTURES_HPP

#include <raylib.h>
#include <map>
#include <string>
#include <utility>
#include <vector>

/**
 * @struct TileData
 * @brief Contiene los datos de la hoja de sprites
 *
 * Esta estructura almacena el "Texture2D" completo de la hoja de sprites,
 * así como la cantidad de filas y columnas, y las dimensiones individuales
 * de cada sprite
 */
struct TileData {
  // Textura que contiene todos los sprites utilizados en el juego
  Texture2D texture;

  // Número de filas que contiene la hoja de sprites
  int rows;
  
  // Número de columnas que contiene la hoja de sprites
  int cols;

  // Ancho en píxeles de cada sprite individual
  int width;

  // Alto en píxeles de cada sprite individual
  int height;
};

/**
 * @struct Entity
 * @brief Representa cualquier objeto dentro del juego
 *
 * Una entidad tiene una posición en la grilla, un tipo, una simbología única,
 * y un conjunto de etiquetas booleanas ("tags") que definen su comportamiento
 */
struct Entity {
  // Identificador único de la entidad
  int id;

  // Posición de la entidad en la grilla como par (fila, columna)
  std::pair<int, int> pos;

  // Tipo textual de la entidad (por ejemplo: "player", "wall")
  std::string type;

  /**
   * Mapa que asocia nombres de propiedades (como "isPush", "isYou", etc.)
   * con un valor booleano que indica si la entidad posee esa propiedad.
   */
  std::map<std::string, bool> tags;

  // Carácter simbólico que representa visual o lógicamente a la entidad
  char symbol;
};

/**
 * @brief Contiene la información general de un nivel del juego.
 *
 * Almacena las dimensiones de la grilla, el escalado visual, 
 * las entidades activas y la asociación entre símbolos y sprites.
 */
struct LevelData {
  // Cantidad de filas en la grilla del nivel
  int rows;

  // Cantidad de columnas en la grilla del nivel
  int cols;

  // Ancho visual de cada celda del nivel
  int cell_width;

  // Alto visual de cada celda del nivel
  int cell_heigth;

  // Mapeo entre símbolos de entidades y su ID de sprite correspondiente
  std::map<char, int> tiles_id;

  // Conjunto de entidades presentes en el nivel
  std::vector<Entity> entities;
};

#endif  // STRUCTURES_HPP
