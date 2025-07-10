#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <raylib.h>

enum class GameState { playing, won, lost };

// Valores Game
inline const std::string gameTitle = "Baba Is You";
inline const int gameFPS = 60;
inline const int gameWW = GetScreenWidth();
inline const int gameWH = GetScreenHeight();

#endif  // CONFIG_HPP