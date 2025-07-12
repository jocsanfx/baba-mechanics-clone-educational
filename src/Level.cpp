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

void Level::loadLevel(const int level_counter) {
  std::string path = PATH_LEVEL_TO_LOAD + std::to_string(level_counter) + EXT_LEVEL_TO_LOAD;
  std::ifstream input(path);
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

  for (Entity &e : level.entities) {
    e.tags = {{"isPush", false}, {"isYou", false},  {"isLose", false},
      {"isWin", false},  {"isStop", false}, {"isBreak", false}};
    if (e.type == "floor") e.tags.clear();
    if (e.type == "instruction") e.tags["isPush"] = true;
  }

  for (Entity &e : level.entities) {
    if (e.symbol != 'I') continue;

    if (e.pos.second - 1 >= 0 && e.pos.second + 1 < level.cols) {
      auto left = charsAt(e.pos.first, e.pos.second - 1);
      auto right = charsAt(e.pos.first, e.pos.second + 1);
      for (char l : left)
        for (char r : right) {
          if (relation.count(l) && relation.count(r))
            setSymbol(relation.at(l), relation.at(r));
          if (relation.count(l) && action.count(r)) setTag(relation.at(l), r);
        }
    }

    if (e.pos.first - 1 >= 0 && e.pos.first + 1 < level.rows) {
      auto upper = charsAt(e.pos.first - 1, e.pos.second);
      auto under = charsAt(e.pos.first + 1, e.pos.second);
      for (char u : upper)
        for (char d : under) {
          if (relation.count(u) && relation.count(d))
            setSymbol(relation.at(u), relation.at(d));
          if (relation.count(u) && action.count(d)) setTag(relation.at(u), d);
        }
    }
  }

  setTag('~', '|');
  setTag('h', 'S');
}

std::vector<char> Level::charsAt (int row, int col) {
  std::vector<char> vec;
  for (const Entity &ent : this->level.entities)
    if (ent.pos == std::make_pair(row, col)) vec.push_back(ent.symbol);
  if (vec.empty()) vec.push_back('0');
  return vec;
};

void Level::setTag(const char &c, const char &a) {
  for (Entity &e : this->level.entities) {
    if (e.symbol == c) {
      e.tags[action.at(a)] = true;
    }
  }
}

bool Level::tryMove(Entity &mover, int dr, int dc) {
  int nr = mover.pos.first + dr;
  int nc = mover.pos.second + dc;

  if (nr < 0 || nr >= level.rows || nc < 0 || nc >= level.cols) return false;

  std::vector<Entity *> dest;
  for (Entity &e : level.entities)
    if (e.pos == std::make_pair(nr, nc)) dest.push_back(&e);

  bool hayStopNoPush = false;
  bool hayPush = false;
  for (Entity *e : dest) {
    if (e->tags["isPush"])
      hayPush = true;
    else if (e->tags["isStop"])
      hayStopNoPush = true;
  }
  if (hayStopNoPush && !hayPush) return false;

  for (Entity *e : dest) {
    if (!e->tags["isPush"]) continue;
    if (!tryMove(*e, dr, dc)) return false;
  }

  mover.pos = {nr, nc};
  return true;
}

void Level::setSymbol(const char &old, const char &current) {
  for (Entity &e : this->level.entities) {
    if (e.symbol == old) {
      e.type = entityString(current);
      e.symbol = current;
      this->level.mapa[e.pos.first][e.pos.second] = current;
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
  if (nextRow < 0 || nextRow >= level.rows || nextCol < 0 ||
      nextCol >= level.cols)
    return false;

  std::vector<Entity *> here;
  for (Entity &e : level.entities)
    if (e.pos == std::make_pair(row, col)) here.push_back(&e);

  bool hasPush = false;
  bool hasStopNoPush = false;
  for (Entity *e : here) {
    if (e->tags["isPush"])
      hasPush = true;
    else if (e->tags["isStop"])
      hasStopNoPush = true;
  }
  if (hasStopNoPush && !hasPush) return false;

  if (hasPush) {
    if (!tryPush(nextRow, nextCol, dx, dy)) return false;
    for (Entity *e : here) {
      if (!e->tags["isPush"]) continue;
      int oldRow = e->pos.first, oldCol = e->pos.second;
      e->pos = {nextRow, nextCol};
      moveEntityOnMap(*e, oldRow, oldCol, nextRow, nextCol);
    }
  }
  return true;
}

GameState Level::handleInput() {
  Vector2 dir = {0, 0};
  if (IsKeyPressed(KEY_RIGHT))
    dir.x = 1;
  else if (IsKeyPressed(KEY_LEFT))
    dir.x = -1;
  else if (IsKeyPressed(KEY_DOWN))
    dir.y = 1;
  else if (IsKeyPressed(KEY_UP))
    dir.y = -1;

  if (IsKeyPressed(KEY_Z)) {
    if (!history.empty()) {
      auto snap = history.top();
      history.pop();
      level.mapa = snap.first;
      level.entities = snap.second;
    }
    return GameState::playing;
  }

  GameState result = GameState::playing;
  if (dir.x == 0 && dir.y == 0) return result;

  int dx = static_cast<int>(dir.x);
  int dy = static_cast<int>(dir.y);
  history.push({level.mapa, level.entities});

  for (Entity &e : level.entities) {
    if (!e.tags["isYou"]) continue;

    int newRow = e.pos.first + dy;
    int newCol = e.pos.second + dx;
    if (newRow < 0 || newRow >= level.rows || newCol < 0 ||
      newCol >= level.cols)
    continue;

    std::vector<Entity *> dest;
    for (Entity &o : level.entities)
      if (o.pos == std::make_pair(newRow, newCol)) dest.push_back(&o);

    bool hasPush = false, hasStopNoPush = false;
    for (Entity *o : dest) {
      if (o->tags["isPush"])
        hasPush = true;
      else if (o->tags["isStop"])
        hasStopNoPush = true;
    }

    bool canMove = !(hasStopNoPush && !hasPush);
    if (canMove && hasPush && !tryPush(newRow, newCol, dx, dy)) canMove = false;

    if (canMove) {
      int oldRow = e.pos.first;
      int oldCol = e.pos.second;
      e.pos = {newRow, newCol};
      moveEntityOnMap(e, oldRow, oldCol, newRow, newCol);

      for (const Entity &ent : level.entities) {
        if (ent.pos != e.pos) continue;
        if (ent.tags.count("isWin") && ent.tags.at("isWin"))
          result = GameState::won;
        if (ent.tags.count("isLose") && ent.tags.at("isLose"))
          result = GameState::lost;
      }
    }
  }
  handleRules();
  return result;
}
