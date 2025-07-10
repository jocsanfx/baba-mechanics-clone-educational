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

  void adjustToFitScreen();
  bool isBlocked(int row, int col);
  bool tryPush(int row, int col, int dx, int dy);
  std::string entityString(char c);
};

#endif  // LEVEL_HPP
