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

#include "models/Structures.hpp"
#include "config/Config.hpp"

/**
 * @class Level
 * @brief Class that manages a game level's logic and representation
 *
 * This class loads levels, interprets the game rules, manages entity state and
 * movement history, and draws everything on screen using the raylib library
 */
class Level {
 private:
  int frameCount = 0;
  int count = 0;
  Color GREY = {21, 24, 31, 255};
  float offsetX = 0;
  float offsetY = 0;
  friend class Game;

  TileData tiles = {};
  LevelData level = {};

  std::stack<std::vector<Entity>> history;

 private:
  /** @brief Loads the sprite sheet and prepares the textures. */
  void loadTextures();

  /**
   * @brief Loads a specific level from a file.
   * @param level_counter Number of the level to load.
   */
  void loadLevel(const int level_counter);

  /** @brief Loads the association between characters and sprite sheet IDs. */
  void loadTileIDs();

  /**
   * @brief Processes player input (keys) and updates the game state
   * @return GameState State resulting from the move (won, lost, playing, etc.)
   */
  GameState handleInput();

  /** @brief Updates the level's internal state (animations, rules, etc.)
   * 
   */
  void update();

  /**
   * @brief Draws the current game state on screen
   * @param state Current game state (playing, won, or lost)
   * @param level_counter Current level (used to display messages)
   */
  void draw(GameState state, const int level_counter);

  /**
   * @brief Draws a specific tile on the board
   * @param id Tile ID in the sprite sheet
   * @param row Row where the tile is drawn
   * @param col Column where the tile is drawn
   */
  void drawTail(int id, int row, int col);

  /** @brief Draws all level entities by layer */
  void drawEntities();

  /** @brief Interprets and applies rules based on the level instructions
   */
  void handleRules();

  /**
   * @brief Attempts to move an entity in the specified direction
   * @param entity Pointer to the entity to move
   * @param dr Row direction
   * @param dc Column direction
   * @return true if the move succeeded
   */
  bool tryMove(Entity* entity, int dr, int dc);

  /**
   * @brief Draws all entities that match the given set
   * @param conjunct Set of symbols to draw
   * @param player Current player symbol (not drawn here)
   */
  void drawByLayer(const std::set<char>& conjunct, const char& player);

  /**
   * @brief Draws all entities that have the specified symbol
   * @param symbol Symbol of the entities to draw
   */
  void drawByLayer(const char& symbol);

  /**
   * @brief Gets the current player's symbol
   * @return char Player symbol ("isYou")
   */
  char getCurrentPlayer();

  /** @brief Adjusts the level's size and position to center it on screen
   */
  void adjustToFitScreen();

  /**
   * @brief Attempts to push entities in a specific direction
   * @param row Starting row
   * @param col Starting column
   * @param dx X direction
   * @param dy Y direction
   * @return true if the push succeeded
   */
  bool tryPush(int row, int col, int dx, int dy);

  /**
   * @brief Changes an entity's type and symbol
   * @param old Current symbol
   * @param current New symbol
   */
  void setSymbol(const char& old, const char& current);

  /**
   * @brief Assigns a tag to entities with a specific symbol
   * @param c Entity symbol
   * @param a Action (tag) to assign
   */
  void setTag(const char& c, const char& a);

  /** @brief Processes destructive interactions between entities */
  void processRemove();

  /**
   * @brief Updates the player's appearance according to the direction
   * @param dir Direction pressed ('w', 'a', 's', or 'd')
   */
  void updatePlayerSprite(const char& dir);

  /**
   * @brief Returns the symbols of all entities in a cell
   * @param row Cell row
   * @param col Cell column
   * @return Vector of characters present in the cell
   */
  std::vector<char> charsAt(int row, int col) const;
};

#endif  // LEVEL_HPP
