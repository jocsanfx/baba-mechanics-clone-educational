#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <raylib.h>
#define SCALE_FACTOR 5/7
#define SPRITE_SHEET_ROW 10
#define SPRITE_SHEET_COL 9
#define SPRITE_SHEET_WIDTH 24
#define SPRITE_SHEET_HEIGHT 24
#define SPRITE_SHEET_PATH "./assets/images/baba.png"
#define TILE_ID_PATH "./assets/data/tile_ids.txt"
#define LEVEL_TO_LOAD "./assets/data/lvl0.txt"

enum class GameState { playing, won, lost };

// Valores Game
inline const std::string gameTitle = "Baba Is You";
inline const int gameFPS = 60;
inline const int gameWW = GetScreenWidth();
inline const int gameWH = GetScreenHeight();

#endif  // CONFIG_HPP