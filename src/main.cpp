#include <iostream>

#include "Game.hpp"

int main() {
  Game g;

  g.init();
  g.run();
  g.destroy();

  return 0;
}