#include "Game.hpp"

// Game handle
void Game::init() {
  this->title = "Baba is you";
  this->window_width = 500;
  this->window_height = 500;
  this->fps = 60;

  InitWindow(this->window_width, this->window_height, this->title.c_str());
  SetTargetFPS(this->fps);

  this->loadTextures();
  this->loadLevel();
}

void Game::run() {
  while (!WindowShouldClose()) {
    this->draw();
  }
}

void Game::destroy() {
  UnloadTexture(this->tiles.texture);
  CloseWindow();
}

// Draw
void Game::draw() {
  BeginDrawing();

  ClearBackground(RAYWHITE);
  this->drawMap();
  this->drawEntities();

  EndDrawing();
}

void Game::drawTail(int id, int row, int col) {
  Rectangle src = {
    static_cast<float>((id % this->tiles.cols) - 1) * this->tiles.width,
    static_cast<float>(id / this->tiles.cols) * this->tiles.height,
    static_cast<float>(this->tiles.width),
    static_cast<float>(this->tiles.height)
  };

  Rectangle dst = {
    static_cast<float>(col * this->level.cell_width),
    static_cast<float>(row * this->level.cell_heigth),
    static_cast<float>(this->level.cell_width),
    static_cast<float>(this->level.cell_heigth)
  };
  Vector2 origin = {0.0f, 0.0f};
  DrawTexturePro(this->tiles.texture, src, dst, origin, 0, WHITE);
}

void Game::drawMap() {
  for (int i = 0; i < this->level.rows; ++i) {
    for (int j = 0; j < this->level.cols; ++j) {
      int id = this->level.tiles_id[this->level.mapa[i][j]];
      this->drawTail(id, i, j);
    } 
  }
}

void Game::drawEntities() {
  for (const Entity& e : this->level.entities) {
    int id = 0;

    if (e.type == "player") {
      id = this->level.tiles_id['*'];
    } else if (e.type == "instruction") {
      id = this->level.tiles_id['B'];
    } else if (e.type == "rock") {
      id = this->level.tiles_id['$'];
    } else if (e.type == "flag") {
      id = this->level.tiles_id['&'];
    } else if (e.type == "wall") {
      id = this->level.tiles_id['#'];
    } else if (e.type == "water") {
      id = this->level.tiles_id['~'];
    }

    this->drawTail(id, e.pos.first, e.pos.second);
  }
}

// Loading
void Game::loadTextures() {
  Image image = LoadImage("./assets/images/sokoban.png");
  this->tiles.texture = LoadTextureFromImage(image);
  UnloadImage(image);

  // Estan fijos porque son las medidas del sprite sheet del ejemplo de Sokoban
  // pero son intercambiables ez por si usamos otro
  this->tiles.rows = 8;
  this->tiles.cols = 13;
  this->tiles.width = 64;
  this->tiles.height = 64;
}

void Game::loadTileIDs() {
  std::ifstream idInput("./assets/data/tile_ids.txt");

  char symbol;
  int tileID;
  while (idInput >> symbol >> tileID) {
    this->level.tiles_id[symbol] = tileID;
  }

  idInput.close();
}

void Game::loadLevel() {
  std::ifstream input("./assets/data/lvl3.txt");

  input >> this->level.rows >> this->level.cols;
  input >> this->level.cell_width >> this->level.cell_heigth;
  char empty = 'e';
  this->level.mapa.clear();
  this->level.entities.clear();

  this->level.mapa = std::vector<std::vector<char>>(
    this->level.rows, std::vector<char>(this->level.cols, empty));

  int n, row, col;
  int entityId = 0;


  // Esta cargando con sistemas x que por el momento no hacen nada

  // --- Instrucciones ---
  input >> n;
  for (int i = 0; i < n; ++i) {
    input >> row >> col;
    this->level.entities.push_back(Entity{
      entityId++, {row, col}, "instruction", {{"isPush", true}}
    });
  }

  // --- Jugador ---
  input >> row >> col;
  this->level.entities.push_back(Entity{
    entityId++, {row, col}, "player", {{"isYou", true}}
  });

  // --- Piedras ---
  input >> n;
  for (int i = 0; i < n; ++i) {
    input >> row >> col;
    this->level.entities.push_back(Entity{
      entityId++, {row, col}, "rock", {{"isPush", true}}
    });
  }

  // --- Bandera ---
  input >> n;
  for (int i = 0; i < n; ++i) {
    input >> row >> col;
    this->level.entities.push_back(Entity{
      entityId++, {row, col}, "flag", {{"isWin", true}}
    });
  }

  // --- Muros ---
  input >> n;
  for (int i = 0; i < n; ++i) {
    input >> row >> col;
    this->level.entities.push_back(Entity{
      entityId++,
      {row, col},
      "wall",
      {{"isStop", true}}
    });
  }

  // --- Agua ---
  input >> n;
  for (int i = 0; i < n; ++i) {
    input >> row >> col;
    this->level.entities.push_back(Entity{
      entityId++,
      {row, col},
      "water",
      {{"isLose", true}}
    });
  }

  input.close();

  this->loadTileIDs();

  this->window_width = this->level.cols * this->level.cell_width;
  this->window_height = this->level.rows * this->level.cell_heigth;
  SetWindowSize(this->window_width, this->window_height);
}