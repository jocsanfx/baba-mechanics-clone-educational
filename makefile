# Carpetas
BUILD := build
BIN := bin
SRC := src
INCLUDE := include

# Variables
APPNAME := program.exe
XC := C:/raylib/w64devkit/bin/g++.exe
WFLAGS := -Wall -Wextra
INCLUDES := -I$(INCLUDE) -IC:/raylib/w64devkit/x86_64-w64-mingw32/include
LIBS := -LC:/raylib/w64devkit/x86_64-w64-mingw32/lib -lraylib -lopengl32 -lgdi32 -lwinmm
SFLAGS := 

# Búsqueda de archivos
FSOURCE := $(wildcard $(SRC)/*.cpp $(SRC)/*/*.cpp)
FOBJECT := $(patsubst $(SRC)/%.cpp,$(BUILD)/%.o,$(FSOURCE))

.PHONY: all clean run asan msan

all: $(BIN)/$(APPNAME)

$(BIN)/$(APPNAME): $(FOBJECT) | $(BIN)
	$(XC) $(WFLAGS) $(SFLAGS) -g $^ -o $@ $(LIBS)

$(BUILD)/%.o: $(SRC)/%.cpp | $(BUILD)
	@if not exist "$(@D)" mkdir "$(@D)"
	$(XC) $(WFLAGS) $(SFLAGS) $(INCLUDES) -c -g $< -o $@

$(BIN):
	@if not exist "$@" mkdir "$@"

$(BUILD):
	@if not exist "$@" mkdir "$@"

run:
	$(BIN)/$(APPNAME) $(ARGS)

asan: SFLAGS += -fsanitize=address
asan: $(BIN)/$(APPNAME)

msan: SFLAGS += -fsanitize=memory
msan: XC = clang++
msan: $(BIN)/$(APPNAME)

clean:
	@if exist "$(BUILD)" rmdir /s /q "$(BUILD)"
	@if exist "$(BIN)" rmdir /s /q "$(BIN)"