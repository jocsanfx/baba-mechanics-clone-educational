// Copyright 2025 Jocsan Fernández <jocsan.fernandezsalas@ucr.ac.cr>
// && Isaac Araya <isaac.arayaquesada@ucr.ac.cr>
// && May Retana <may.retana@ucr.ac.cr>
#include "Utilities.hpp"

/**
 * @brief Devuelve el tipo textual de una entidad dado su símbolo gráfico
 *
 * Esta función también incluye múltiples caracteres que corresponden a
 * instrucciones del juego, y los agrupa bajo el tipo "instruction"
 *
 * @param c Carácter que representa una entidad en el nivel
 * @return Tipo textual de la entidad (por ejemplo: "wall", "instruction")
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
