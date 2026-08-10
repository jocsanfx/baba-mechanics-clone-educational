// Copyright 2025 Jocsan Fernández <jocsan.fernandezsalas@ucr.ac.cr>
// && Isaac Araya <isaac.arayaquesada@ucr.ac.cr>
// && May Retana <may.retana@ucr.ac.cr>
#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <string>
#include <set>
#include <map>
#include <vector>
#include "config/Config.hpp"

/**
 * @brief Returns the text type associated with an entity symbol
 *
 * @param c Graphical symbol of the entity
 * @return Entity type as text (for example, "wall" or "instruction")
 */
std::string entityString(char c);

/**
 * @brief Dictionary that associates letters (instructions) with entity symbols
 * 
 * Example: 'B' represents the instruction that affects '*'
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
 * @brief Dictionary of actions associated with instruction characters.
 * 
 * Actions define behaviors such as "isPush" and "isWin".
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
 * @brief Set of symbols considered backgrounds or surfaces
 * 
 * They are rendered at the back of the map before objects are drawn
 */
inline const std::set<char> backgrounds{'~', 'Z', 'h', 'b', '#'};

/**
 * @brief Set of valid symbols for interactive or visible objects
 * 
 * Includes the player, obstacles, enemies, and so on
 */
inline const std::set<char> valid_objects{'*', '$', '+', '-', 'f', 'x'
  , 'y', 'z', '0'};

#endif
