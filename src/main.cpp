// Copyright 2025 Jocsan Fernández <jocsan.fernandezsalas@ucr.ac.cr>
// && Isaac Araya <isaac.arayaquesada@ucr.ac.cr>
// && May Retana <may.retana@ucr.ac.cr>
#include <iostream>

#include "Game.hpp"

/**
 * @brief Función principal del programa
 *
 * Inicia el ciclo de vida del objeto Game:
 * - Inicializa recursos y configuración
 * - Ejecuta el bucle principal
 * - Libera los recursos utilizados
 *
 * @return int Código de salida del programa
 */
int main() {
  Game g;

  g.init();
  g.run();
  g.destroy();

  return 0;
}
