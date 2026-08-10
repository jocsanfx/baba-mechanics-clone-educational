// Copyright 2025 Jocsan Fernández <jocsan.fernandezsalas@ucr.ac.cr>
// && Isaac Araya <isaac.arayaquesada@ucr.ac.cr>
// && May Retana <may.retana@ucr.ac.cr>
#include <iostream>

#include "game/Game.hpp"

/**
 * @brief Main program function
 *
 * Starts the Game object's lifecycle:
 * - Initializes resources and configuration
 * - Runs the main loop
 * - Releases the resources used
 *
 * @return int Program exit code
 */
int main() {
  Game g;

  g.init();
  g.run();
  g.destroy();

  return 0;
}
