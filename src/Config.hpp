// Copyright 2025 Jocsan Fernández <jocsan.fernandezsalas@ucr.ac.cr>
// && Isaac Araya <isaac.arayaquesada@ucr.ac.cr>
// && May Retana <may.retana@ucr.ac.cr>
#ifndef CONFIG_HPP
#define CONFIG_HPP

/**
 * @file config.hpp
 * @brief Archivo de configuración global para el juego Baba Is You
 *
 * Contiene macros, constantes, rutas de recursos, y variables globales que 
 * definen el comportamiento visual y lógico del juego
 */

#include <raylib.h>
#include <string>

// Símbolo por defecto del jugador
#define PLAYER_DEFAULT_SYMBOL '*'

// Cuadros por segundo entre animaciones
#define ANIMATION_BY_FPS 6

// Tamaño base de fuente en el juego
#define FONT_SIZE 60

// Símbolo que representa a Baba mirando a la derecha
#define BABA_RIGHT '*'

// Símbolo que representa la bandera (meta)
#define FLAG '&'

// Baba mirando hacia arriba
#define BABA_UP 'x'

// Baba mirando a la izquierda
#define BABA_LEFT 'y'

// Baba mirando hacia abajo
#define BABA_DOWN 'z'

// ID por defecto para símbolos desconocidos
#define DEFAULT_SYMBOL_ID 9

// Tamaño inicial estándar
#define DEFAULT_INIT_SIZE 0

// FPS inicial antes de que empiece el juego
#define DEFAULT_INIT_FPS 0

// Número mínimo de nivel
#define MIN_LEVEL 0

// Número máximo de nivel
#define MAX_LEVEL 7

// Escala visual aplicada a sprites
#define SCALE_FACTOR 5/7

// Número de filas en la hoja de sprites
#define SPRITE_SHEET_ROW 10

// Número de columnas en la hoja de sprites
#define SPRITE_SHEET_COL 9

// Ancho de cada sprite (px)
#define SPRITE_SHEET_WIDTH 24

// Alto de cada sprite (px)
#define SPRITE_SHEET_HEIGHT 24

// Ruta a la hoja de sprites
#define SPRITE_SHEET_PATH "./assets/images/baba.png"

// Ruta al archivo de IDs
#define TILE_ID_PATH "./assets/data/tile_ids.txt"

// Ruta base de niveles
#define PATH_LEVEL_TO_LOAD "./assets/data/lvl"

// Extensión de archivos de nivel
#define EXT_LEVEL_TO_LOAD ".txt"

// Título de la ventana del juego
#define GAME_TITLE "Baba Is You"

// Velocidad en cuadros por segundo
#define GAME_FPS 60

/**
 * @enum GameState
 * @brief Representa el estado actual del juego
 */
enum class GameState { playing, won, lost };

// Ancho de la ventana del juego (obtenido dinámicamente)
inline const int GAME_WW = GetScreenWidth();
// Alto de la ventana del juego (obtenido dinámicamente)
inline const int GAME_WH = GetScreenHeight();

#endif  // CONFIG_HPP
