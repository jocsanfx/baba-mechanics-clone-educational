#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <raylib.h>
#define PLAYER_DEFAULT_SYMBOL '*'
#define BABA_RIGHT '*'
#define BABA_UP 'x'
#define BABA_LEFT 'y'
#define BABA_DOWN 'z'
#define DEFAULT_SYMBOL_ID 9
#define DEFAULT_INIT_SIZE 0
#define DEFAULT_INIT_FPS 0
#define MIN_LEVEL 4
#define MAX_LEVEL 7
#define SCALE_FACTOR 5/7
#define SPRITE_SHEET_ROW 10
#define SPRITE_SHEET_COL 9
#define SPRITE_SHEET_WIDTH 24
#define SPRITE_SHEET_HEIGHT 24
#define SPRITE_SHEET_PATH "./assets/images/baba.png"
#define TILE_ID_PATH "./assets/data/tile_ids.txt"
#define PATH_LEVEL_TO_LOAD "./assets/data/lvl"
#define EXT_LEVEL_TO_LOAD ".txt"

enum class GameState { playing, won, lost };

// Valores Game
inline const std::string gameTitle = "Baba Is You";
inline const int gameFPS = 60;
inline const int gameWW = GetScreenWidth();
inline const int gameWH = GetScreenHeight();

#endif  // CONFIG_HPP