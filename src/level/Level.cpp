// Copyright 2025 Jocsan Fernández <jocsan.fernandezsalas@ucr.ac.cr>
// && Isaac Araya <isaac.arayaquesada@ucr.ac.cr>
// && May Retana <may.retana@ucr.ac.cr>
#include "level/Level.hpp"

#include <string>
#include "config/Config.hpp"
#include "utilities/Utilities.hpp"


/**
 * @brief Draws the current level state on screen
 *
 * This function handles three possible game states:
 * - `GameState::lost`: Displays the defeat screen
 * - `GameState::won`: Displays the victory or end-of-game screen
 * - `GameState::playing`: Draws the map and active entities
 *
 * It also updates the game animation based on the configured FPS
 *
 * @param state Current game state (playing, won, or lost)
 * @param level_counter Current level number used to display relevant messages
 */
void Level::draw(GameState state, const int level_counter) {
  // Increment the frame counter for animations
  ++this->frameCount;
  if (this->frameCount >= ANIMATION_BY_FPS) {
    this->frameCount = 0;
    this->count = (this->count + 1) % 3;
  }

  BeginDrawing();

  // Display the defeat screen if the player lost
  if (state == GameState::lost) {
    ClearBackground(BLACK);

    const char* text = "YOU LOST";
    int fontSize = FONT_SIZE;
    Color textColor = RED;

    Vector2 textSize = MeasureTextEx(GetFontDefault(), text, fontSize, 1);
    Vector2 textPosition = {(GetScreenWidth() - textSize.x) / 2,
      (GetScreenHeight() - textSize.y) / 2};

    DrawText(text, textPosition.x, textPosition.y, fontSize, textColor);

    const char* suggestion = "Press ENTER or R to restart";
    int smallFontSize = FONT_SIZE / 3;
    Vector2 instSize = MeasureTextEx(GetFontDefault(), suggestion,
      smallFontSize, 1);
    DrawText(suggestion, (GetScreenWidth() - instSize.x) / 2,
      textPosition.y + textSize.y + 30, smallFontSize, WHITE);
  // Display the victory screen if the player won the level
  } else if (state == GameState::won) {
    ClearBackground(BLACK);

    const char* text = (level_counter != MAX_LEVEL) ? "YOU WON" :
      "END OF GAME";
    int fontSize = FONT_SIZE;
    Color textColor = GREEN;

    Vector2 textSize = MeasureTextEx(GetFontDefault(), text, fontSize, 1);
    Vector2 textPosition = {(GetScreenWidth() - textSize.x) / 2,
      (GetScreenHeight() - textSize.y) / 2};

    DrawText(text, textPosition.x, textPosition.y, fontSize, textColor);

    const char* suggestion = (level_counter != MAX_LEVEL)
      ? "Press ENTER to continue": "Thanks for playing";
    int smallFontSize = FONT_SIZE / 3;
    Vector2 instSize = MeasureTextEx(GetFontDefault(), suggestion,
      smallFontSize, 1);
    DrawText(suggestion, (GetScreenWidth() - instSize.x) / 2,
      textPosition.y + textSize.y + 30, smallFontSize, WHITE);
  // Draw normal gameplay
  } else {
    ClearBackground(this->GREY);
    int mapW = this->level.cols * this->level.cell_width;
    int mapH = this->level.rows * this->level.cell_heigth;
    DrawRectangle(this->offsetX, this->offsetY, mapW, mapH, BLACK);
    drawEntities();
  }

  EndDrawing();
}

/**
 * @brief Draws an individual sprite (tile) at the given position
 *
 * Uses a tile identifier (`id`) to determine which part of the sprite sheet to
 * draw and calculates the destination position on screen using each cell's
 * size and the level offsets
 *
 * @param id Sprite ID within the sprite sheet
 * @param row Row where the sprite is drawn
 * @param col Column where the sprite is drawn
 */
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

/**
 * @brief Draws all level entities in the correct layer order
 *
 * The function first draws background elements, then instructions, valid
 * objects (such as walls and boxes), the flag (goal), and finally the current
 * player
 */
void Level::drawEntities() {
  // Determine the current player-controlled avatar
  char currentAvatar = getCurrentPlayer();

  // Draw the background layer
  drawByLayer(backgrounds, currentAvatar);
  // Draw instructions (with animation)
  for (const Entity& e : this->level.entities) {
    if (e.type == "instruction") {
      int id = this->level.tiles_id[e.symbol];
      id += this->count;
      this->drawTail(id, e.pos.first, e.pos.second);
    }
  }

  // Draw valid objects such as obstacles or interactive objects
  drawByLayer(valid_objects, currentAvatar);
  // Draw the flag (victory symbol)
  drawByLayer(FLAG);
  // Draw the player-controlled avatar
  drawByLayer(currentAvatar);
}

/**
 * @brief Draws every entity whose symbol belongs to the given set, excluding
 * the player
 *
 * This function is useful for drawing layers (background, objects, and so on)
 * without interfering with the player's avatar
 *
 * @param conjunct Set of symbols to draw
 * @param player Current player symbol, which must be omitted
 */
void Level::drawByLayer(const std::set<char>& conjunct, const char& player) {
  for (const Entity& e : this->level.entities) {
    if (e.symbol != player && conjunct.count(e.symbol) == 1) {
      int id = this->level.tiles_id[e.symbol];
      id += this->count;
      this->drawTail(id, e.pos.first, e.pos.second);
    }
  }
}

/**
 * @brief Draws all entities with a specific symbol
 *
 * This function draws a particular layer, such as the flag or player,
 * according to its symbol
 *
 * @param symbol Symbol of the entity to draw
 */
void Level::drawByLayer(const char& symbol) {
  for (const Entity& e : this->level.entities) {
    if (e.symbol == symbol) {
      int id = this->level.tiles_id[e.symbol];
      id += this->count;
      this->drawTail(id, e.pos.first, e.pos.second);
    }
  }
}

/**
 * @brief Loads the sprite sheet and stores its texture and properties
 * (rows, columns, and size).
 *
 * This function reads the image from the path specified by `SPRITE_SHEET_PATH`
 * and configures the values required to represent each individual tile.
 */
void Level::loadTextures() {
  Image image = LoadImage(SPRITE_SHEET_PATH);
  this->tiles.texture = LoadTextureFromImage(image);
  UnloadImage(image);
  this->tiles.rows = SPRITE_SHEET_ROW;
  this->tiles.cols = SPRITE_SHEET_COL;
  this->tiles.width = SPRITE_SHEET_WIDTH;
  this->tiles.height = SPRITE_SHEET_HEIGHT;
}

/**
 * @brief Loads the mapping from symbols to tile IDs from a file
 *
 * Reads the file specified by `TILE_ID_PATH`, which contains character and
 * numeric tile identifier pairs, and stores them in the `tiles_id` map
 */
void Level::loadTileIDs() {
  std::ifstream idInput(TILE_ID_PATH);
  char symbol;
  int tileID;
  while (idInput >> symbol >> tileID) {
    this->level.tiles_id[symbol] = tileID;
  }
  idInput.close();
}

/**
 * @brief Loads a level from a text file
 *
 * This function reads the level file corresponding to the given number,
 * initializes the entities, clears the movement history, and adjusts the
 * screen
 *
 * @param level_counter Number of the level to load
 */
void Level::loadLevel(const int level_counter) {
  std::string path =
    PATH_LEVEL_TO_LOAD + std::to_string(level_counter) + EXT_LEVEL_TO_LOAD;
  std::ifstream input(path);
  input >> this->level.rows >> this->level.cols;
  input.ignore();
  this->level.entities.clear();
  this->history = {};
  int entityId = 0;
  for (int i = 0; i < this->level.rows; ++i) {
    std::string line;
    std::getline(input, line);
    for (int j = 0; j < this->level.cols; ++j) {
      char ch = (j < static_cast<int>(line.size())) ? line[j] : '0';
      if (ch == '0') {
        continue;
      }
      std::string type = entityString(ch);
      if (type == "") {
        continue;
      }
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

/**
 * @brief Processes the level rules based on the instructions present
 *
 * Resets all entity tags and evaluates "A IS B" grammatical structures both
 * horizontally and vertically to define dynamic game behaviors (such as
 * `isYou`, `isWin`, and `isStop`)
 */
void Level::handleRules() {
  for (Entity& e : level.entities) {
    e.tags = {{"isPush", false}, {"isYou", false},  {"isLose", false},
      {"isWin", false},  {"isStop", false}, {"isBreak", false}};
    if (e.type == "floor") {
      e.tags.clear();
    }
    if (e.type == "instruction") {
      e.tags["isPush"] = true;
    }
  }

  for (Entity& e : level.entities) {
    if (e.symbol != 'I') {
      continue;
    }

    if (e.pos.second - 1 >= 0 && e.pos.second + 1 < level.cols) {
      auto left = charsAt(e.pos.first, e.pos.second - 1);
      auto right = charsAt(e.pos.first, e.pos.second + 1);
      for (char l : left)
        for (char r : right) {
          if (relation.count(l) && relation.count(r)) {
            setSymbol(relation.at(l), relation.at(r));
          }
          if (relation.count(l) && action.count(r)) {
            setTag(relation.at(l), r);
          }
        }
      }

    if (e.pos.first - 1 >= 0 && e.pos.first + 1 < level.rows) {
      auto upper = charsAt(e.pos.first - 1, e.pos.second);
      auto under = charsAt(e.pos.first + 1, e.pos.second);
      for (char u : upper)
        for (char d : under) {
          if (relation.count(u) && relation.count(d)) {
            setSymbol(relation.at(u), relation.at(d));
          }
          if (relation.count(u) && action.count(d)) {
            setTag(relation.at(u), d);
          }
        }
      }
    }
  }

  /**
 * @brief Returns the symbols in a specific board cell
 *
 * If the cell contains no entities, returns a vector containing only '0'
 *
 * @param row Row to query
 * @param col Column to query
 * @return std::vector<char> Vector containing the symbols found in the cell
 */
std::vector<char> Level::charsAt(int row, int col) const {
  std::vector<char> vec;
  for (const Entity& ent : this->level.entities)
    if (ent.pos == std::make_pair(row, col)) {
      vec.push_back(ent.symbol);
    }
  if (vec.empty()) {
    vec.push_back('0');
  }
  return vec;
}

/**
 * @brief Assigns a tag (such as "isYou" or "isWin") to all entities with the
 * specified symbol
 *
 * This function is used while evaluating rules to activate behaviors. It has
 * a special condition for the '~' symbol, which also activates "isStop"
 *
 * @param c Symbol of the entity to modify
 * @param a Action represented by a letter ('W', 'S', 'L', and so on)
 */
void Level::setTag(const char& c, const char& a) {
  for (Entity& e : this->level.entities) {
    if (e.symbol == c) {
      e.tags[action.at(a)] = true;
      if (e.symbol == '~') {
        e.tags[action.at('|')] = true;
      }
    }
  }
}

/**
 * @brief Attempts to move an entity in a specific direction
 *
 * If the destination cell contains entities tagged "isPush", the function
 * attempts to move them recursively. If "isStop" is present without
 * "isPush", movement is blocked
 *
 * @param entity Entity to move
 * @param dr Row movement direction (-1, 0, or 1)
 * @param dc Column movement direction (-1, 0, or 1)
 * @return true If the move succeeded
 * @return false If the move was blocked
 */
bool Level::tryMove(Entity* entity, int dr, int dc) {
  int nr = entity->pos.first + dr;
  int nc = entity->pos.second + dc;

  if (nr < 0 || nr >= level.rows || nc < 0 || nc >= level.cols) {
    return false;
  }

  std::vector<Entity *> dest;
  for (Entity& e : level.entities)
    if (e.pos == std::make_pair(nr, nc)) {
      dest.push_back(&e);
    }

  bool hasStopNoPush = false;
  bool hasPush = false;
  for (Entity* e : dest) {
    if (e->tags["isPush"]) {
      hasPush = true;
    } else if (e->tags["isStop"]) {
      hasStopNoPush = true;
    }
  }

  if (hasStopNoPush && !hasPush) {
    return false;
  }

  for (Entity* e : dest) {
    if (!e->tags["isPush"]) {
      continue;
    }

    if (!tryMove(e, dr, dc)) {
      return false;
    }
  }

  entity->pos = {nr, nc};
  return true;
}

/**
 * @brief Changes the symbol and type of all entities with a given symbol
 *
 * Used mainly while processing rules such as "WALL IS FLAG", where entities
 * of the previous type change to a new type and symbol
 *
 * @param old Current symbol to replace
 * @param current New symbol to apply to matching entities
 */
void Level::setSymbol(const char& old, const char& current) {
  for (Entity& e : this->level.entities) {
    if (e.symbol == old) {
      e.type = entityString(current);
      e.symbol = current;
    }
  }
}

/**
 * @brief Adjusts the level cell size so it scales proportionally to the screen
 *
 * Calculates the width and height scale factors and centers the map on screen.
 * This method runs once when the level is loaded
 */
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

/**
 * @brief Attempts to push entities from a cell in one direction
 *
 * Used to process "isPush" mechanics. It attempts to recursively move entities
 * stacked in the given direction, checking that "isStop" without "isPush"
 * does not block them
 *
 * @param row Current row of the pushing entity
 * @param col Current column of the pushing entity
 * @param dx Horizontal push direction (-1 left, 1 right)
 * @param dy Vertical push direction (-1 up, 1 down)
 * @return true If all entities were pushed successfully
 * @return false If an entity could not be pushed
 */
bool Level::tryPush(int row, int col, int dx, int dy) {
  std::vector<Entity *> here;
  for (Entity& e : level.entities)
    if (e.pos == std::make_pair(row, col)) {
      here.push_back(&e);
    }

  bool hasPush = false;
  bool hasStopNoPush = false;
  for (Entity* e : here) {
    if (e->tags["isPush"]) {
      hasPush = true;
    } else if (e->tags["isStop"]) {
      hasStopNoPush = true;
    }
  }

  if (hasStopNoPush && !hasPush) {
    return false;
  }

  if (!hasPush) {
    return true;
  }

  int nextRow = row + dy;
  int nextCol = col + dx;
  if (nextRow < 0 || nextRow >= level.rows || nextCol < 0 ||
    nextCol >= level.cols) {
      return false;
    }

  if (!tryPush(nextRow, nextCol, dx, dy)) {
    return false;
  }

  for (Entity* e : here) {
    if (!e->tags["isPush"]) {
      continue;
    }
    e->pos = {nextRow, nextCol};
  }
  return true;
}

/**
 * @brief Handles keyboard input and updates the game state
 *
 * Processes movement keys (arrows) for entities with "isYou", performs pushes
 * with "tryPush", evaluates collisions with "isWin", and allows undoing with
 * "Z". It also updates the rules after movement
 *
 * @return GameState Resulting game state (playing, won, or lost)
 */
GameState Level::handleInput() {
  Vector2 dir = {0, 0};
  if (IsKeyPressed(KEY_RIGHT)) {
    dir.x = 1;
    updatePlayerSprite('d');
  } else if (IsKeyPressed(KEY_LEFT)) {
    dir.x = -1;
    updatePlayerSprite('a');
  } else if (IsKeyPressed(KEY_DOWN)) {
    dir.y = 1;
    updatePlayerSprite('s');
  } else if (IsKeyPressed(KEY_UP)) {
    dir.y = -1;
    updatePlayerSprite('w');
  }

  if (IsKeyPressed(KEY_Z)) {
    if (!history.empty()) {
      auto snap = history.top();
      history.pop();
      level.entities = snap;
    }
    return GameState::playing;
  }

  GameState result = GameState::playing;
  if (dir.x == 0 && dir.y == 0) {
    return result;
  }

  int dx = static_cast<int>(dir.x);
  int dy = static_cast<int>(dir.y);

  auto snapshotEntities = level.entities;
  bool didMove = false;

  for (Entity& e : level.entities) {
    if (!e.tags["isYou"]) {
      continue;
    }

    int newRow = e.pos.first + dy;
    int newCol = e.pos.second + dx;
    if (newRow < 0 || newRow >= level.rows || newCol < 0 ||
        newCol >= level.cols) {
          continue;
        }

    std::vector<Entity *> dest;
    for (Entity& o : level.entities)
      if (o.pos == std::make_pair(newRow, newCol)) {
        dest.push_back(&o);
      }

    bool hasPush = false, hasStopNoPush = false;
    for (Entity* o : dest) {
      if (o->tags["isPush"]) {
        hasPush = true;
      } else if (o->tags["isStop"]) {
        hasStopNoPush = true;
      }
    }

    bool canMove = !(hasStopNoPush && !hasPush);
    if (canMove && hasPush && !tryPush(newRow, newCol, dx, dy)) {
      canMove = false;
    }

    if (canMove) {
      e.pos = {newRow, newCol};
      didMove = true;

      for (const Entity& ent : level.entities) {
        if (ent.pos != e.pos) {
          continue;
        }
        if (ent.tags.count("isWin") && ent.tags.at("isWin")) {
          result = GameState::won;
        }
      }
    }
  }

  processRemove();

  if (didMove) {
    history.push(snapshotEntities);
    handleRules();
  }

  bool theresIsYou = false;

  for (Entity& e : level.entities) {
    if (e.tags.count("isYou") && e.tags.at("isYou")) {
      theresIsYou = true;
      break;
    }
  }

  if (!theresIsYou) {
    result = GameState::lost;
  }

  return result;
}

/**
 * @brief Removes game entities according to destructive interactions
 *
 * This function processes collisions between entities in the same position.
 * Entities are removed according to the following rules:
 * - If an "isYou" entity collides with an "isLose" entity, "isYou" is removed
 * - If a non-"isYou" entity collides with an "isBreak" entity, both are removed
 */
void Level::processRemove() {
  std::vector<bool> remove(level.entities.size(), false);
  for (size_t i = 0; i < level.entities.size(); ++i) {
    for (size_t j = i + 1; j < level.entities.size(); ++j) {
      if (level.entities[i].pos != level.entities[j].pos) {
        continue;
      }
      if (level.entities[i].tags.count("isYou") &&
          level.entities[i].tags.at("isYou") &&
          level.entities[j].tags.count("isLose") &&
          level.entities[j].tags.at("isLose")) {
        remove[i] = true;
      }
      if (level.entities[j].tags.count("isYou") &&
          level.entities[j].tags.at("isYou") &&
          level.entities[i].tags.count("isLose") &&
          level.entities[i].tags.at("isLose")) {
        remove[j] = true;
      }
      if (!(level.entities[i].tags.count("isYou") &&
            level.entities[i].tags.at("isYou")) &&
          level.entities[j].tags.count("isBreak") &&
          level.entities[j].tags.at("isBreak")) {
        remove[i] = true;
        remove[j] = true;
      }
      if (!(level.entities[j].tags.count("isYou") &&
            level.entities[j].tags.at("isYou")) &&
          level.entities[i].tags.count("isBreak") &&
          level.entities[i].tags.at("isBreak")) {
        remove[i] = true;
        remove[j] = true;
      }
    }
  }
  for (int i = static_cast<int>(level.entities.size()) - 1; i >= 0; --i) {
    if (remove[i]) {
      level.entities.erase(level.entities.begin() + i);
    }
  }
}

/**
 * @brief Updates the player sprite according to the direction pressed
 *
 * Changes the sprite direction of the player-controlled character ("isYou")
 * only if the current symbol represents a directional BABA variant
 *
 * @param dir Movement direction: 'w' (up), 'a' (left), 's' (down), or
 * 'd' (right)
 */
void Level::updatePlayerSprite(const char& dir) {
  char currentAvatar = getCurrentPlayer();
  if (currentAvatar == BABA_LEFT || currentAvatar == BABA_UP ||
      currentAvatar == BABA_RIGHT || currentAvatar == BABA_DOWN) {
    int newSprite;
    switch (dir) {
      case 'w':
        newSprite = this->level.tiles_id[BABA_UP];
        break;
      case 'a':
        newSprite = this->level.tiles_id[BABA_LEFT];
        break;
      case 's':
        newSprite = this->level.tiles_id[BABA_DOWN];
        break;
      case 'd':
        newSprite = DEFAULT_SYMBOL_ID;
        break;
      default:
        newSprite = DEFAULT_SYMBOL_ID;
        break;
    }
    this->level.tiles_id[currentAvatar] = newSprite;
  }
}

/**
 * @brief Gets the current player's symbol
 *
 * Searches all entities for one tagged "isYou" and returns its symbol. Assumes
 * that at least one entity has that tag
 *
 * @return char Symbol of the player-controlled entity
 */
char Level::getCurrentPlayer() {
  std::string rule = "isYou";
  char currentAvatar;
  for (Entity& e : this->level.entities) {
    if (e.tags[rule] == true) {
      currentAvatar = e.symbol;
      break;
    }
  }
  return currentAvatar;
}
