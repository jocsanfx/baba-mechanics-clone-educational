// Copyright 2025 Jocsan Fernández <jocsan.fernandezsalas@ucr.ac.cr>
// && Isaac Araya <isaac.arayaquesada@ucr.ac.cr>
// && May Retana <may.retana@ucr.ac.cr>
#include "Level.hpp"

#include <string>
#include "Config.hpp"
#include "Utilities.hpp"


/**
 * @brief Dibuja el estado actual del nivel en pantalla
 *
 * Esta función maneja tres posibles estados del juego: 
 * - `GameState::lost`: Muestra la pantalla de derrota
 * - `GameState::won`: Muestra la pantalla de victoria o fin del juego
 * - `GameState::playing`: Dibuja el mapa y las entidades activas
 *
 * También se encarga de actualizar la animación del juego en base a los 
 * FPS definidos
 *
 * @param state Estado actual del juego (jugando, ganado o perdido)
 * @param level_counter Número del nivel actual para mostrar mensajes 
 * correspondientes
 */
void Level::draw(GameState state, const int level_counter) {
  // Incrementa el contador de fotogramas para animaciones
  ++this->frameCount;
  if (this->frameCount >= ANIMATION_BY_FPS) {
    this->frameCount = 0;
    this->count = (this->count + 1) % 3;
  }

  BeginDrawing();

  // Si el jugador perdió, se muestra pantalla de derrota
  if (state == GameState::lost) {
    ClearBackground(BLACK);

    const char* texto = "PERDISTE";
    int fontSize = FONT_SIZE;
    Color colorTexto = RED;

    Vector2 textSize = MeasureTextEx(GetFontDefault(), texto, fontSize, 1);
    Vector2 textPosition = {(GetScreenWidth() - textSize.x) / 2,
      (GetScreenHeight() - textSize.y) / 2};

    DrawText(texto, textPosition.x, textPosition.y, fontSize, colorTexto);

    const char* sugerencia = "Presione ENTER o R para reiniciar";
    int smallFontSize = FONT_SIZE / 3;
    Vector2 instSize = MeasureTextEx(GetFontDefault(), sugerencia,
      smallFontSize, 1);
    DrawText(sugerencia, (GetScreenWidth() - instSize.x) / 2,
      textPosition.y + textSize.y + 30, smallFontSize, WHITE);
  // Si ganó el nivel
  } else if (state == GameState::won) {
    ClearBackground(BLACK);

    const char* texto = (level_counter != MAX_LEVEL) ? "GANASTE" :
      "FIN DEL JUEGO";
    int fontSize = FONT_SIZE;
    Color colorTexto = GREEN;

    Vector2 textSize = MeasureTextEx(GetFontDefault(), texto, fontSize, 1);
    Vector2 textPosition = {(GetScreenWidth() - textSize.x) / 2,
      (GetScreenHeight() - textSize.y) / 2};

    DrawText(texto, textPosition.x, textPosition.y, fontSize, colorTexto);

    const char* sugerencia = (level_counter != MAX_LEVEL)
      ? "Presione ENTER para continuar": "Gracias por jugar";
    int smallFontSize = FONT_SIZE / 3;
    Vector2 instSize = MeasureTextEx(GetFontDefault(), sugerencia,
      smallFontSize, 1);
    DrawText(sugerencia, (GetScreenWidth() - instSize.x) / 2,
      textPosition.y + textSize.y + 30, smallFontSize, WHITE);
  // Si se está jugando normalmente
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
 * @brief Dibuja un sprite individual (tile) en la posición dada
 *
 * Utiliza un identificador de tile (`id`) para determinar qué parte del sprite 
 * sheet se debe dibujar, y calcula la posición de destino en pantalla usando 
 * el tamaño de cada celda y los desplazamientos del nivel
 *
 * @param id ID del sprite dentro del sprite sheet
 * @param row Fila donde se debe dibujar el sprite
 * @param col Columna donde se debe dibujar el sprite
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
 * @brief Dibuja todas las entidades del nivel en el orden correcto por capas
 *
 * La función primero dibuja los elementos del fondo, luego las instrucciones,
 * después los objetos válidos (como paredes, cajas, etc.), la bandera (meta)
 * y finalmente el jugador actual
 */
void Level::drawEntities() {
  // Determina el avatar actual controlado por el jugador
  char currentAvatar = getCurrentPlayer();

  // Dibuja la capa de fondo
  drawByLayer(backgrounds, currentAvatar);
  // Dibuja las instrucciones (con animación)
  for (const Entity& e : this->level.entities) {
    if (e.type == "instruction") {
      int id = this->level.tiles_id[e.symbol];
      id += this->count;
      this->drawTail(id, e.pos.first, e.pos.second);
    }
  }

  // Dibuja objetos válidos como obstáculos o interactivos
  drawByLayer(valid_objects, currentAvatar);
  // Dibuja la bandera (símbolo de victoria)
  drawByLayer(FLAG);
  // Dibuja el avatar controlado por el jugador
  drawByLayer(currentAvatar);
}

/**
 * @brief Dibuja todas las entidades cuyo símbolo pertenece al conjunto dado,
 * excluyendo al jugador
 *
 * Esta función es útil para dibujar por capas (fondo, objetos, etc.) 
 * sin interferir con el avatar del jugador
 *
 * @param conjunct Conjunto de símbolos a dibujar
 * @param player Símbolo actual del jugador, que debe ser omitido
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
 * @brief Dibuja todas las entidades con un símbolo específico
 *
 * Esta función se utiliza para dibujar una capa particular, como la bandera
 * o el jugador, según su símbolo
 *
 * @param symbol Símbolo de la entidad a dibujar
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
 * @brief Carga el sprite sheet y almacena su textura y propiedades 
 * (filas, columnas, tamaño).
 *
 * Esta función lee la imagen desde la ruta especificada por 'SPRITE_SHEET_PATH'
 * y configura los valores necesarios para representar cada tile individual.
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
 * @brief Carga el mapeo de símbolos a IDs de tiles desde un archivo
 *
 * Lee el archivo especificado por `TILE_ID_PATH`, que contiene pares de
 * carácter e identificador numérico de tile, y los almacena en el mapa 
 * 'tiles_id'
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
 * @brief Carga un nivel desde un archivo de texto
 *
 * Esta función lee el archivo que representa el nivel correspondiente 
 * al número dado, inicializa las entidades, limpia la historia de 
 * movimientos y ajusta la pantalla
 *
 * @param level_counter Número del nivel a cargar
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
 * @brief Procesa las reglas del nivel con base en las instrucciones presentes
 *
 * Restablece todas las etiquetas (`tags`) de las entidades y evalúa estructuras
 * gramaticales tipo "A IS B" tanto en horizontal como vertical para definir 
 * comportamientos dinámicos del juego (como `isYou`, `isWin`, `isStop`, etc.)
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
 * @brief Devuelve los símbolos que están en una celda específica del 
 * tablero
 *
 * Si no hay ninguna entidad en esa celda, se devuelve un vector con 
 * un solo elemento '0'
 *
 * @param row Fila a consultar
 * @param col Columna a consultar
 * @return std::vector<char> Vector con los símbolos encontrados en la celda
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
 * @brief Asigna una etiqueta (como "isYou", "isWin", etc.) a todas 
 * las entidades con el símbolo especificado
 *
 * Esta función es utilizada durante la evaluación de las reglas 
 * para activar comportamientos
 * Tiene una condición especial para el símbolo '~', al cual también 
 * se le activa "isStop"
 *
 * @param c Símbolo de la entidad a modificar
 * @param a Acción representada por una letra ('W', 'S', 'L', etc.)
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
 * @brief Intenta mover una entidad en una dirección específica
 *
 * Si la casilla destino contiene entidades con la etiqueta "isPush", 
 * se intenta moverlas recursivamente. Si hay `isStop` sin "isPush",
 * el movimiento se bloquea
 *
 * @param mover Entidad que se desea mover
 * @param dr Dirección de movimiento en filas (-1, 0, 1)
 * @param dc Dirección de movimiento en columnas (-1, 0, 1)
 * @return true Si el movimiento fue exitoso
 * @return false Si el movimiento fue bloqueado
 */
bool Level::tryMove(Entity* mover, int dr, int dc) {
  int nr = mover->pos.first + dr;
  int nc = mover->pos.second + dc;

  if (nr < 0 || nr >= level.rows || nc < 0 || nc >= level.cols) {
    return false;
  }

  std::vector<Entity *> dest;
  for (Entity& e : level.entities)
    if (e.pos == std::make_pair(nr, nc)) {
      dest.push_back(&e);
    }

  bool hayStopNoPush = false;
  bool hayPush = false;
  for (Entity* e : dest) {
    if (e->tags["isPush"]) {
      hayPush = true;
    } else if (e->tags["isStop"]) {
      hayStopNoPush = true;
    }
  }

  if (hayStopNoPush && !hayPush) {
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

  mover->pos = {nr, nc};
  return true;
}

/**
 * @brief Cambia el símbolo y tipo de todas las entidades con un símbolo dado
 *
 * Usado principalmente al procesar reglas tipo "WALL IS FLAG", donde las 
 * entidades del tipo anterior cambian a un nuevo tipo y símbolo
 *
 * @param old Símbolo actual que se desea reemplazar
 * @param current Nuevo símbolo que se aplicará a las entidades encontradas
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
 * @brief Ajusta el tamaño de las celdas del nivel para que se escalen
 * proporcionalmente a la pantalla
 *
 * Calcula el factor de escala en ancho y alto, y centra el mapa en pantalla.
 * Este método se ejecuta una vez al cargar el nivel
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
 * @brief Intenta empujar las entidades en una dirección desde una celda
 *
 * Se usa para procesar mecánicas del tipo "isPush". Intenta mover recursivamente
 * las entidades apiladas en la dirección dada, verificando que no haya bloqueo
 * por "isStop" sin "isPush"
 *
 * @param row Fila actual de la entidad que empuja
 * @param col Columna actual de la entidad que empuja
 * @param dx Dirección horizontal del empuje (-1 izquierda, 1 derecha)
 * @param dy Dirección vertical del empuje (-1 arriba, 1 abajo)
 * @return true Si todas las entidades pudieron ser empujadas correctamente
 * @return false Si alguna entidad no pudo ser empujada
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
 * @brief Maneja la entrada del teclado y actualiza el estado del juego
 *
 * Procesa las teclas de movimiento (flechas) para entidades con "isYou", 
 * ejecuta empujes ("tryPush"), evalúa colisiones con "isWin" y permite 
 * deshacer con "Z". También actualiza las reglas luego del movimiento
 *
 * @return GameState Estado del juego resultante (playing, won o lost)
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
 * @brief Elimina entidades del juego según interacciones destructivas
 *
 * Esta función procesa colisiones entre entidades que ocupan la misma posición
 * Se eliminan entidades según las siguientes reglas:
 * - Si una entidad "isYou" colisiona con una "isLose", se elimina "isYou"
 * - Si una entidad no es "isYou" y colisiona con una "isBreak", ambas se eliminan
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
 * @brief Actualiza el sprite del jugador según la dirección presionada
 *
 * Cambia la dirección del sprite del personaje controlado por el jugador 
 * ("isYou"), solo si el símbolo actual representa una variante 
 * direccional de BABA
 *
 * @param dir Dirección del movimiento: 'w' (arriba), 'a' (izquierda), 
 * 's' (abajo), 'd' (derecha)
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
 * @brief Obtiene el símbolo del jugador actual
 *
 * Busca entre todas las entidades aquella marcada con la etiqueta "isYou"
 * y devuelve su símbolo. Se asume que hay al menos una entidad con esa 
 * etiqueta
 *
 * @return char Símbolo de la entidad controlada por el jugador
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
