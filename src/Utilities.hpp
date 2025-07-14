// Copyright 2025 Jocsan Fernández <jocsan.fernandezsalas@ucr.ac.cr>
// && Isaac Araya <isaac.arayaquesada@ucr.ac.cr>
// && May Retana <may.retana@ucr.ac.cr>
#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include "Config.hpp"
#include <string>
#include <set>
#include <map>
#include <vector>

/**
 * @brief Devuelve el símbolo correspondiente a un tipo textual de entidad
 *
 * @param type Tipo textual de la entidad (por ejemplo, "player", "rock")
 * @return Símbolo que representa gráficamente a la entidad
 */
char getSymbolForEntity(const std::string& type);

/**
 * @brief Devuelve el tipo textual asociado a un símbolo de entidad
 *
 * @param c Símbolo gráfico de la entidad
 * @return Tipo textual de la entidad (por ejemplo, "wall", "instruction")
 */
std::string entityString(char c);

/**
 * @brief Diccionario que asocia letras (instrucciones) con símbolos de entidades
 * 
 * Ejemplo: 'B' representa la instrucción para que '*' sea afectado
 */
inline const std::map<char, char> relation {
  {'B', '*'},
  {'W', '#'},
  {'G', 'h'},
  {'F', '&'},
  {'R', '$'},
  {'A', '~'},
  {'L', '-'},
  {'C', '+'},
};

/**
 * @brief Diccionario de acciones asociadas a caracteres de instrucción.
 * 
 * Las acciones definen comportamientos como "isPush", "isWin", etc.
 */
inline const std::map<char, std::string> action {
  {'N', "isWin"},
  {'D', "isLose"},
  {'H', "isLose"},
  {'K', "isLose"},
  {'Y', "isYou"},
  {'S', "isStop"},
  {'P', "isPush"},
  {'M', "isMelt"},
  {'|', "isBreak"},
};

/**
 * @brief Conjunto de símbolos que se consideran fondos o superficies
 * 
 * Se renderizan al fondo del mapa antes de dibujar objetos
 */
inline const std::set<char> backgrounds{'~', 'Z', 'h', 'b', '#'};

/**
 * @brief Conjunto de símbolos válidos para objetos interactivos o visibles
 * 
 * Incluye jugador, obstáculos, enemigos, etc
 */
inline const std::set<char> valid_objects{'*', '$', '+', '-', 'f', 'x'
  , 'y', 'z', '0'};

#endif
