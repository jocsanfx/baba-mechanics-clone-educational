#include "Level.hpp"

#include "Config.hpp"
#include "Utilities.hpp"

void Level::draw(GameState state) {
  ++this->frameCount;
  if (this->frameCount >= 12) {
    this->frameCount = 0;
    this->count = (this->count + 1) % 3;
  }

  BeginDrawing();
  ClearBackground({21, 24, 31, 255});
  int mapW = this->level.cols * this->level.cell_width;
  int mapH = this->level.rows * this->level.cell_heigth;
  DrawRectangle(this->offsetX, this->offsetY, mapW, mapH, BLACK);

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

void Level::drawEntities() {
  for (const Entity &e : this->level.entities) {
    int id = 0;
    if (e.type == "instruction") {
      id = this->level.tiles_id[e.symbol];
    } else {
      id = this->level.tiles_id[getSymbolForEntity(e.type)];
    }
    id += this->count;
    this->drawTail(id, e.pos.first, e.pos.second);
  }
}

void Level::undo() {
  if (!this->history.empty()) {
    auto [prevMap, prevEntities] = this->history.top();
    this->level.mapa = prevMap;
    this->level.entities = prevEntities;
    this->history.pop();
  }
}

void Level::loadTextures() {
  Image image = LoadImage(SPRITE_SHEET_PATH);
  this->tiles.texture = LoadTextureFromImage(image);
  UnloadImage(image);
  this->tiles.rows = SPRITE_SHEET_ROW;
  this->tiles.cols = SPRITE_SHEET_COL;
  this->tiles.width = SPRITE_SHEET_WIDTH;
  this->tiles.height = SPRITE_SHEET_HEIGHT;
}

void Level::loadTileIDs() {
  std::ifstream idInput(TILE_ID_PATH);
  char symbol;
  int tileID;
  while (idInput >> symbol >> tileID) {
    this->level.tiles_id[symbol] = tileID;
  }
  idInput.close();
}

void Level::loadLevel() {
  std::ifstream input(LEVEL_TO_LOAD);
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
      if (entityString(ch) == "") {
        this->level.mapa[i][j] = '0';
      } else {
        this->level.mapa[i][j] = ch;
      }
      if (ch == '0') continue;
      std::string type = entityString(ch);
      if (type == "") continue;
      std::map<std::string, bool> tags = {
        {"isPush", false}, {"isYou", false},  {"isLose", false},
        {"isWin", false},  {"isStop", false}, {"isBreak", false}};
      this->level.entities.push_back(
        Entity{entityId++, {i, j}, type, tags, ch});
    }
  }
  input.close();
  this->loadTileIDs();
  this->adjustToFitScreen();
  this->handleRules();
}

void Level::handleRules() {
  for (Entity &e : this->level.entities) {
    std::map<std::string, bool> tags = {{"isPush", false}, {"isYou", false},
      {"isLose", false}, {"isWin", false}, {"isStop", false},
      {"isBreak", false}};
    e.tags = tags;
    if (e.type == "floor") {
      e.tags = {};
    }
    if (e.type == "instruction") {
      e.tags["isPush"] = true;
    }
  }

  for (Entity &e : this->level.entities) {
    if (e.symbol == 'I') {
      char toChance;
      char toChancefor;

      if (e.pos.second - 1 < this->level.cols && e.pos.second - 1 >= 0 &&
          e.pos.second + 1 < this->level.cols && e.pos.second + 1 >= 0) {
        char prevChar = this->level.mapa[e.pos.first][e.pos.second - 1];
        char nextChar = this->level.mapa[e.pos.first][e.pos.second + 1];
        if (relation.count(prevChar) == 1 && relation.count(nextChar) == 1) {
          toChance = relation.at(prevChar);
          toChancefor = relation.at(nextChar);
          std::cout << toChance << " " << toChancefor << std::endl;
          setSymbol(toChance, toChancefor);
        }

        if (relation.count(prevChar) == 1 && action.count(nextChar) == 1) {
          toChance = relation.at(prevChar);
          toChancefor = nextChar;
          setTag(toChance, toChancefor);
        }
      }

      if (e.pos.first - 1 < this->level.rows && e.pos.first - 1 >= 0 &&
          e.pos.first + 1 < this->level.rows && e.pos.first + 1 >= 0) {
        char upperChar = this->level.mapa[e.pos.first - 1][e.pos.second];
        char underChar = this->level.mapa[e.pos.first + 1][e.pos.second];

        if (relation.count(upperChar) == 1 && relation.count(underChar) == 1) {
          toChance = relation.at(upperChar);
          toChancefor = relation.at(underChar);
          std::cout << toChance << " " << toChancefor << std::endl;
          setSymbol(toChance, toChancefor);
        }

        if (relation.count(upperChar) == 1 && action.count(underChar) == 1) {
          toChance = relation.at(upperChar);
          toChancefor = underChar;
          setTag(toChance, toChancefor);
        }
      }
    }
  }

  setTag('~', '|');
}

void Level::setSymbol(const char& old, const char& current) {
  for (Entity &e : this->level.entities) {
    if (e.symbol == old) {
      e.type = entityString(current);
      e.symbol = current;
      this->level.mapa[e.pos.first][e.pos.second] = current;
    }
  }
}

void Level::setTag(const char &c, const char &a) {
  for (Entity &e : this->level.entities) {
    if (e.symbol == c) {
      e.tags[action.at(a)] = true;
    }
  }
}

void Level::adjustToFitScreen() {
  float scaleW = static_cast<float>(GetScreenWidth()) /
    (this->level.cols * this->tiles.width) * SCALE_FACTOR;
  float scaleH = static_cast<float>(GetScreenHeight()) /
    (this->level.rows * this->tiles.height) * SCALE_FACTOR;
  float finalScale = std::min(scaleW, scaleH);
  this->level.cell_width = this->tiles.width * finalScale;
  this->level.cell_heigth = this->tiles.height * finalScale;
  float totalMapWidth = this->level.cols * this->level.cell_width;
  float totalMapHeight = this->level.rows * this->level.cell_heigth;
  this->offsetX = (GetScreenWidth() - totalMapWidth) / 2.0f;
  this->offsetY = (GetScreenHeight() - totalMapHeight) / 2.0f;
}

bool Level::isBlocked(int row, int col) {
  for (const Entity &e : this->level.entities) {
    if (e.pos == std::make_pair(row, col)) {
      if (e.tags.count("isStop") && e.tags.at("isStop")) {
        return true;
      }
    }
  }
  return false;
}

void Level::moveEntityOnMap(const Entity &entity, int oldRow, int oldCol,
    int newRow, int newCol) {
  this->level.mapa[oldRow][oldCol] = '0';
  this->level.mapa[newRow][newCol] = entity.symbol;
}

bool Level::tryPush(int row, int col, int dx, int dy) {
  int nextRow = row + dy;
  int nextCol = col + dx;
  if (nextRow < 0 || nextRow >= this->level.rows || nextCol < 0 ||
    nextCol >= this->level.cols) {
    return false;
  }
  auto it =
    std::find_if(this->level.entities.begin(), this->level.entities.end(),
      [row, col](const Entity &e) {
        return e.pos == std::make_pair(row, col);
    });
  if (it == this->level.entities.end()) {
    return true;
  }
  Entity &current = *it;
  if (!current.tags["isPush"]) {
    return false;
  }
  auto front =
    std::find_if(this->level.entities.begin(), this->level.entities.end(),
      [nextRow, nextCol](const Entity &e) {
        return e.pos == std::make_pair(nextRow, nextCol);
      });
  if (front != this->level.entities.end()) {
    if (front->tags["isStop"]) {
      return false;
    }
    if (front->tags["isPush"]) {
      if (!tryPush(nextRow, nextCol, dx, dy)) {
        return false;
      }
    }
  }
  int oldRow = current.pos.first;
  int oldCol = current.pos.second;
  current.pos = {nextRow, nextCol};
  moveEntityOnMap(current, oldRow, oldCol, nextRow, nextCol);
  return true;
}

GameState Level::handleInput() {
  Vector2 dir = {0, 0};
  if (IsKeyPressed(KEY_RIGHT)) {
    dir.x = 1;
  } else if (IsKeyPressed(KEY_LEFT)) {
    dir.x = -1;
  } else if (IsKeyPressed(KEY_DOWN)) {
    dir.y = 1;
  } else if (IsKeyPressed(KEY_UP)) {
    dir.y = -1;
  }
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
  if (dir.x == 0 && dir.y == 0) {
    return result;
  }
  int dx = static_cast<int>(dir.x);
  int dy = static_cast<int>(dir.y);
  history.push({this->level.mapa, this->level.entities});
  for (Entity &e : this->level.entities) {
    if (e.tags["isYou"]) {
      int newRow = e.pos.first + dy;
      int newCol = e.pos.second + dx;
      if (newRow < 0 || newRow >= this->level.rows || newCol < 0 ||
        newCol >= this->level.cols) {
        continue;
      }
      auto it =
        std::find_if(this->level.entities.begin(), this->level.entities.end(),
          [newRow, newCol](const Entity &other) {
            return other.pos == std::make_pair(newRow, newCol);
          });

      bool canMove = true;

      if (it != this->level.entities.end()) {
        Entity &other = *it;
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
        int oldRow = e.pos.first;
        int oldCol = e.pos.second;
        e.pos = {newRow, newCol};
        moveEntityOnMap(e, oldRow, oldCol, newRow, newCol);
        for (const Entity &ent : this->level.entities) {
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
  handleRules();
  return result;
}