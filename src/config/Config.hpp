// Copyright 2025 Jocsan Fernández <jocsan.fernandezsalas@ucr.ac.cr>
// && Isaac Araya <isaac.arayaquesada@ucr.ac.cr>
// && May Retana <may.retana@ucr.ac.cr>
#ifndef CONFIG_HPP
#define CONFIG_HPP

/**
 * @file Config.hpp
 * @brief Global configuration file for the Baba Is You game
 *
 * Contains macros, constants, resource paths, and global variables that
 * define the game's visual and logical behavior
 */

#include <raylib.h>
#include <string>

// Default player symbol
#define PLAYER_DEFAULT_SYMBOL '*'

// Frames per animation
#define ANIMATION_BY_FPS 6

// Base in-game font size
#define FONT_SIZE 60

// Symbol representing Baba facing right
#define BABA_RIGHT '*'

// Symbol representing the flag (goal)
#define FLAG '&'

// Baba facing up
#define BABA_UP 'x'

// Baba facing left
#define BABA_LEFT 'y'

// Baba facing down
#define BABA_DOWN 'z'

// Default ID for unknown symbols
#define DEFAULT_SYMBOL_ID 9

// Standard initial size
#define DEFAULT_INIT_SIZE 0

// Initial FPS before the game starts
#define DEFAULT_INIT_FPS 0

// Minimum level number
#define MIN_LEVEL 0

// Maximum level number
#define MAX_LEVEL 7

// Visual scale applied to sprites
#define SCALE_FACTOR 5/7

// Number of rows in the sprite sheet
#define SPRITE_SHEET_ROW 10

// Number of columns in the sprite sheet
#define SPRITE_SHEET_COL 9

// Width of each sprite (px)
#define SPRITE_SHEET_WIDTH 24

// Height of each sprite (px)
#define SPRITE_SHEET_HEIGHT 24

// Path to the sprite sheet
#define SPRITE_SHEET_PATH "./assets/images/baba.png"

// Path to the ID file
#define TILE_ID_PATH "./assets/data/tile_ids.txt"

// Base path for levels
#define PATH_LEVEL_TO_LOAD "./assets/data/lvl"

// Level file extension
#define EXT_LEVEL_TO_LOAD ".txt"

// Game window title
#define GAME_TITLE "Baba Is You"

// Frame rate in frames per second
#define GAME_FPS 60

/**
 * @enum GameState
 * @brief Represents the current game state
 */
enum class GameState { playing, won, lost };

// Game window width (obtained dynamically)
inline const int GAME_WW = GetScreenWidth();
// Game window height (obtained dynamically)
inline const int GAME_WH = GetScreenHeight();

#endif  // CONFIG_HPP
