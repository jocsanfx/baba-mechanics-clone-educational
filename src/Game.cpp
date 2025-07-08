#include "Game.hpp"

// Game handle
void Game::init() {
  this->title = "Papa Dio quiero pasar";
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
    (float)((id % this->tiles.cols) - 1) * this->tiles.width,
    (float)(id / this->tiles.cols) * this->tiles.height,
    (float)(this->tiles.width),
    (float)(this->tiles.height)
  };

  Rectangle dst = {
    (float)(col * this->level.cell_width),
    (float)(row * this->level.cell_heigth),
    (float)(this->level.cell_width),
    (float)(this->level.cell_heigth)
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

void Game::loadLevel() {
  std::ifstream input("./assets/data/lvl0.txt");

  input >> this->level.rows >> this->level.cols;
  input >> this->level.cell_width >> this->level.cell_heigth;

  this->level.mapa.clear();
  this->level.entities.clear();

  // --- Leer mapa base ---
  for (int i = 0; i < this->level.rows; ++i) {
    this->level.mapa.push_back({});
    for (int j = 0; j < this->level.cols; ++j) {
      char cell;
      input >> cell;
      this->level.mapa[i].push_back(cell);
    }
  }

  int n, row, col;
  int entityId = 0;

  // Esta cargando con sistemas x que por el momento no hacen nada

  // --- Instrucciones ---
  input >> n;
  for (int i = 0; i < n; ++i) {
    input >> row >> col;
    this->level.entities.push_back(Entity{
      entityId++,
      {row, col},
      "instruction",
      {{"isPush", true}}
    });
  }

  // --- Jugador ---
  input >> row >> col;
  this->level.entities.push_back(Entity{
    entityId++,
    {row, col},
    "player",
    {{"isYou", true}}
  });

  // --- Piedras ---
  input >> n;
  for (int i = 0; i < n; ++i) {
    input >> row >> col;
    this->level.entities.push_back(Entity{
      entityId++,
      {row, col},
      "rock",
      {{"isPush", true}}
    });
  }

  // --- Bandera ---
  input >> row >> col;
  this->level.entities.push_back(Entity{
    entityId++,
    {row, col},
    "flag",
    {{"isWin", true}}
  });

  // --- Leyenda de símbolos ---
  char symbol;
  int tileID;
  while (input >> symbol >> tileID) {
    this->level.tiles_id[symbol] = tileID;
  }

  this->window_width = this->level.cols * this->level.cell_width;
  this->window_height = this->level.rows * this->level.cell_heigth;
  SetWindowSize(this->window_width, this->window_height);

  input.close();
}