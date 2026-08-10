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
 * @brief Contains the sprite sheet data
 *
 * This structure stores the complete sprite sheet "Texture2D", along with
 * its number of rows and columns and the dimensions of each sprite
 */
struct TileData {
  // Texture containing all sprites used in the game
  Texture2D texture;

  // Number of rows in the sprite sheet
  int rows;

  // Number of columns in the sprite sheet
  int cols;

  // Width in pixels of each individual sprite
  int width;

  // Height in pixels of each individual sprite
  int height;
};

/**
 * @struct Entity
 * @brief Represents any object in the game
 *
 * An entity has a grid position, a type, a unique symbol, and a set of Boolean
 * tags that define its behavior
 */
struct Entity {
  // Unique entity identifier
  int id;

  // Entity grid position as a (row, column) pair
  std::pair<int, int> pos;

  // Entity type as text (for example, "player" or "wall")
  std::string type;

  /**
   * Map that associates property names (such as "isPush" and "isYou") with
   * a Boolean value indicating whether the entity has that property.
   */
  std::map<std::string, bool> tags;

  // Symbolic character that represents the entity visually or logically
  char symbol;
};

/**
 * @brief Contains general information about a game level.
 *
 * Stores the grid dimensions, visual scaling, active entities, and the
 * association between symbols and sprites.
 */
struct LevelData {
  // Number of rows in the level grid
  int rows;

  // Number of columns in the level grid
  int cols;

  // Visual width of each level cell
  int cell_width;

  // Visual height of each level cell
  int cell_heigth;

  // Mapping between entity symbols and their corresponding sprite IDs
  std::map<char, int> tiles_id;

  // Collection of entities in the level
  std::vector<Entity> entities;
};

#endif  // STRUCTURES_HPP
