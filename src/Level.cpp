#include "Level.hpp"

// Draw
void Level::draw(GameState state) {
  ++frameCount;
  if (frameCount >= 12) {
    frameCount = 0;
    count = (count + 1) % 3;
  }
  BeginDrawing();
  ClearBackground(BLACK);
  drawMap();
  drawEntities();
  if (state == GameState::won) {
    DrawText("¡Ganaste!", 10, 10, 20, GREEN);
  } else if (state == GameState::lost) {
    DrawText("¡Perdiste!", 10, 10, 20, RED);
  }
  EndDrawing();
}

void Level::drawTail(int id, int row, int col) {
  Rectangle src = {
    static_cast<float>((id % this->tiles.cols)) * this->tiles.width,
    static_cast<float>(id / this->tiles.cols) * this->tiles.height,
    static_cast<float>(this->tiles.width),
    static_cast<float>(this->tiles.height)};

  Rectangle dst = {
    this->offsetX + static_cast<float>(col * this->level.cell_width),
    this->offsetY + static_cast<float>(row * this->level.cell_heigth),
    static_cast<float>(this->level.cell_width),
    static_cast<float>(this->level.cell_heigth)};
  Vector2 origin = {0.0f, 0.0f};
  DrawTexturePro(this->tiles.texture, src, dst, origin, 0, WHITE);
}

void Level::drawMap() {
  for (int i = 0; i < this->level.rows; ++i) {
    for (int j = 0; j < this->level.cols; ++j) {
      int id = this->level.tiles_id[this->level.mapa[i][j]];
      this->drawTail(id, i, j);
    }
  }
}

void Level::drawEntities() {
  for (const Entity& e : this->level.entities) {
    int id = 0;

    if (e.type == "instruction") {
      id = this->level.tiles_id[e.symbol] + this->count;
    } else if (e.type == "player") {
      id = this->level.tiles_id['*'] + this->count;
    } else if (e.type == "rock") {
      id = this->level.tiles_id['$'] + this->count;
    } else if (e.type == "flag") {
      id = this->level.tiles_id['&'] + this->count;
    } else if (e.type == "wall") {
      id = this->level.tiles_id['#'] + this->count;
    } else if (e.type == "water") {
      id = this->level.tiles_id['~'] + this->count;
    } else if (e.type == "skull") {
      id = this->level.tiles_id['+'] + this->count;
    } else if (e.type == "lava") {
      id = this->level.tiles_id['-'] + this->count;
    } else if (e.type == "grass") {
      id = this->level.tiles_id['h'] + this->count;
    } else if (e.type == "flower") {
      id = this->level.tiles_id['f'] + this->count;
    } else if (e.type == "floor") {
      id = this->level.tiles_id['Z'] + this->count;
    }

    this->drawTail(id, e.pos.first, e.pos.second);
  }
}

// Update
void Level::undo() {
  if (!this->history.empty()) {
    auto [prevMap, prevEntities] = this->history.top();
    this->level.mapa = prevMap;
    this->level.entities = prevEntities;
    this->history.pop();
  }
}

// Loading
void Level::loadTextures() {
  Image image = LoadImage("./assets/images/baba.png");
  this->tiles.texture = LoadTextureFromImage(image);
  UnloadImage(image);

  // Estan fijos porque son las medidas del sprite sheet del ejemplo de Sokoban
  // pero son intercambiables ez por si usamos otro
  this->tiles.rows = 8;
  this->tiles.cols = 9;
  this->tiles.width = 24;
  this->tiles.height = 24;
}

void Level::loadTileIDs() {
  std::ifstream idInput("./assets/data/tile_ids.txt");

  char symbol;
  int tileID;
  while (idInput >> symbol >> tileID) {
    this->level.tiles_id[symbol] = tileID;
  }

  idInput.close();
}

void Level::loadLevel() {
  std::ifstream input("./assets/data/lvl1.txt");

  input >> this->level.rows >> this->level.cols;
  input.ignore();

  this->level.mapa.clear();
  this->level.entities.clear();
  this->level.mapa = std::vector<std::vector<char>>(
    this->level.rows, std::vector<char>(this->level.cols, '0'));

  this->history = {};

  int entityId = 0;

  for (int i = 0; i < this->level.rows; ++i) {
    std::string line;
    std::getline(input, line);

    for (int j = 0; j < this->level.cols; ++j) {
      char ch = (j < static_cast<int>(line.size())) ? line[j] : '0';
      this->level.mapa[i][j] = ch;

      if (ch == '0') continue;

      std::string type = entityString(ch);
      if (type == "") continue;

      std::map<std::string, bool> tags = {
        {"isPush", true},
        {"isYou", false},
        {"isLose", false},
        {"isWin", false},
        {"isStop", false}
      };

      if (type == "player") {
        tags["isYou"] = true;
        tags["isStop"] = false;
      }

      this->level.entities.push_back(Entity{
        entityId++, {i, j}, type, tags, ch
      });
    }
  }

  input.close();
  this->loadTileIDs();
  this->adjustToFitScreen();
}

// Aux
void Level::adjustToFitScreen() {
  float scaleW =
    static_cast<float>(800) / (this->level.cols * this->tiles.width);
  float scaleH =
    static_cast<float>(800) / (this->level.rows * this->tiles.height);

  float finalScale = std::min(scaleW, scaleH);

  this->level.cell_width = this->tiles.width * finalScale;
  this->level.cell_heigth = this->tiles.height * finalScale;

  float totalMapWidth = this->level.cols * this->level.cell_width;
  float totalMapHeight = this->level.rows * this->level.cell_heigth;

  this->offsetX = (800 - totalMapWidth) / 2.0f;
  this->offsetY = (800 - totalMapHeight) / 2.0f;
}

bool Level::isBlocked(int row, int col) {
  for (const Entity& e : this->level.entities) {
    if (e.pos == std::make_pair(row, col)) {
      if (e.tags.count("isStop") && e.tags.at("isStop")) {
        return true;
      }
    }
  }
  return false;
}

char getSymbolForEntity(const std::string& type) {
  if (type == "player") return '*';
  if (type == "instruction") return 'I';
  if (type == "rock") return '$';
  if (type == "flag") return '&';
  if (type == "wall") return '#';
  if (type == "water") return '~';
  if (type == "lava") return '-';
  if (type == "skull") return '+';
  if (type == "grass") return 'h';
  if (type == "flower") return 'f';
  if (type == "floor") return 'Z';
  return '0';
}

bool Level::tryPush(int row, int col, int dx, int dy) {
  int nextRow = row + dy;
  int nextCol = col + dx;

  auto it =
    std::find_if(this->level.entities.begin(), this->level.entities.end(),
      [row, col](const Entity& e) {
        return e.pos == std::make_pair(row, col);
      });

  if (it == this->level.entities.end()) return true;
  Entity& current = *it;
  if (!current.tags["isPush"]) return false;
  auto front =
    std::find_if(this->level.entities.begin(), this->level.entities.end(),
      [nextRow, nextCol](const Entity& e) {
        return e.pos == std::make_pair(nextRow, nextCol);
      });

  if (front != this->level.entities.end()) {
    if (front->tags["isStop"]) return false;
    if (front->tags["isPush"]) {
      if (!tryPush(nextRow, nextCol, dx, dy)) return false;
    }
  }

  this->level.mapa[row][col] = 'e';
  current.pos = {nextRow, nextCol};
  this->level.mapa[nextRow][nextCol] = getSymbolForEntity(current.type);

  return true;
}

std::string Level::entityString(char c) {
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

    case 'B':
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
      return "instruction";
      break;

    default:
      return "";
  }
}

// Input
GameState Level::handleInput() {
  Vector2 dir = {0, 0};

  if (IsKeyPressed(KEY_RIGHT)) dir.x = 1;
  else if (IsKeyPressed(KEY_LEFT)) dir.x = -1;
  else if (IsKeyPressed(KEY_DOWN)) dir.y = 1;
  else if (IsKeyPressed(KEY_UP)) dir.y = -1;

  if (IsKeyPressed(KEY_Z)) {
    if (!history.empty()) {
      auto snapshot = history.top();
      history.pop();
      this->level.mapa = snapshot.first;
      this->level.entities = snapshot.second;
    }
    return GameState::playing;
  }

  GameState result = GameState::playing;

  if (dir.x == 0 && dir.y == 0) return result;

  int dx = static_cast<int>(dir.x);
  int dy = static_cast<int>(dir.y);

  history.push({this->level.mapa, this->level.entities});

  for (Entity& e : this->level.entities) {
    if (e.tags["isYou"]) {
      int newRow = e.pos.first + dy;
      int newCol = e.pos.second + dx;

      auto it = std::find_if(
        this->level.entities.begin(), this->level.entities.end(),
        [newRow, newCol](const Entity& other) {
          return other.pos == std::make_pair(newRow, newCol);
        }
      );

      bool canMove = true;

      if (it != this->level.entities.end()) {
        Entity& other = *it;
        if (other.tags["isStop"]) {
          canMove = false;
        } else if (other.tags["isPush"]) {
          bool pushSuccess = tryPush(newRow, newCol, dx, dy);
          if (!pushSuccess) {
            canMove = false;
          }
        }
      }

      if (canMove) {
        this->level.mapa[e.pos.first][e.pos.second] = '0';
        e.pos = {newRow, newCol};
        this->level.mapa[newRow][newCol] = getSymbolForEntity(e.type);

        for (const Entity& ent : this->level.entities) {
          if (ent.pos == e.pos) {
            if (ent.tags.count("isWin") && ent.tags.at("isWin")) {
              result = GameState::won;
            }
            if (ent.tags.count("isLose") && ent.tags.at("isLose")) {
              result = GameState::lost;
            }
          }
        }
      }
    }
  }

  return result;
}