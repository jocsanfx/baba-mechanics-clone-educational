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

#include "Structures.hpp"
#include "Config.hpp"

class Level {
 private:
  int frameCount = 0;
  int count = 0;
  float offsetX = 0;
  float offsetY = 0;
  friend class Game;

  TileData tiles;
  LevelData level;

  std::stack<std::pair<std::vector<std::vector<char>>, std::vector<Entity>>>
    history;

 private:
  void loadTextures();
  void loadLevel();
  void loadTileIDs();

  GameState handleInput();

  void update();
  void undo();

  void draw(GameState state);
  void drawTail(int id, int row, int col);
  void drawMap();
  void drawEntities();

  void handleRules();
  bool tryMove(Entity &mover, int dr, int dc);

  void adjustToFitScreen();
  bool isBlocked(int row, int col);
  bool tryPush(int row, int col, int dx, int dy);
  void setSymbol(const char& old, const char& current);
  void setTag(const char& c, const char& a);
  void moveEntityOnMap(const Entity &entity, int oldRow, int oldCol, int newRow,
    int newCol);
};

#endif  // LEVEL_HPP
