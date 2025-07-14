#include "Utilities.hpp"

char getSymbolForEntity(const std::string &type) {
  if (type == "player") { return '*'; }
  if (type == "instruction") { return 'I'; }
  if (type == "rock") { return '$'; }
  if (type == "flag") { return '&'; }
  if (type == "wall") { return '#'; }
  if (type == "water") { return '~'; }
  if (type == "lava") { return '-'; }
  if (type == "skull") { return '+'; }
  if (type == "grass") { return 'h'; }
  if (type == "flower") { return 'f'; }
  if (type == "floor") { return 'Z'; }
  if (type == "brick") { return 'b'; }
  if (type == "melt") { return 'M'; }
  if (type == "defeat") { return 'D'; }
  if (type == "sink") { return 'K'; }
  return '0';
}

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
