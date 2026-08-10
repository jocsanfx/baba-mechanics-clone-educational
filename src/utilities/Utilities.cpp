// Copyright 2025 Jocsan Fernández <jocsan.fernandezsalas@ucr.ac.cr>
// && Isaac Araya <isaac.arayaquesada@ucr.ac.cr>
// && May Retana <may.retana@ucr.ac.cr>
#include "utilities/Utilities.hpp"

/**
 * @brief Returns an entity's text type given its graphical symbol
 *
 * This function also includes multiple characters that correspond to game
 * instructions and groups them under the "instruction" type
 *
 * @param c Character representing an entity in the level
 * @return Entity type as text (for example, "wall" or "instruction")
 */
std::string entityString(char c) {
  switch (c) {
    case '#':
      return "wall";
      break;
    case '$':
      return "rock";
      break;
    case '*':
      return "player";
      break;
    case '&':
      return "flag";
      break;
    case '~':
      return "water";
      break;
    case '+':
      return "skull";
      break;
    case '-':
      return "lava";
      break;
    case 'h':
      return "grass";
      break;
    case 'f':
      return "flower";
      break;
    case 'Z':
      return "floor";
      break;
    case 'b':
      return "brick";
      break;

    case 'M':
    case 'B':
    case 'C':
    case 'D':
    case 'K':
    case 'A':
    case 'I':
    case 'S':
    case 'Y':
    case 'U':
    case 'F':
    case 'N':
    case 'R':
    case 'P':
    case 'L':
    case 'O':
    case 'W':
    case 'H':
    case 'G':
      return "instruction";
      break;

    default:
      return "";
  }
}
