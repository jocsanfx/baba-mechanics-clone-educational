# Carpetas
BUILD := build
BIN := bin
SRC := src
INCLUDE := include

# Variables
APPNAME := program
XC := g++
WFLAGS := -Wall -Wextra
INCLUDES := -I$(INCLUDE)
LIBS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
SFLAGS := 

# Búsqueda de archivos
FSOURCE := $(wildcard $(SRC)/*.cpp $(SRC)/*/*.cpp)
FOBJECT := $(patsubst $(SRC)/%.cpp,$(BUILD)/%.o,$(FSOURCE))

.PHONY: all clean run asan msan

all: $(BIN)/$(APPNAME)

$(BIN)/$(APPNAME): $(FOBJECT) | $(BIN)
	$(XC) $(WFLAGS) $(SFLAGS) -g $^ -o $@ $(LIBS)

$(BUILD)/%.o: $(SRC)/%.cpp | $(BUILD)
	mkdir -p $(@D)
	$(XC) $(WFLAGS) $(SFLAGS) $(INCLUDES) -c -g $< -o $@

$(BIN):
	mkdir -p $@

$(BUILD):
	mkdir -p $@

run:
	./$(BIN)/$(APPNAME) $(ARGS)

asan: SFLAGS += -fsanitize=address
asan: all

msan: SFLAGS += -fsanitize=memory
msan: XC = clang++
msan: all

clean:
	rm -rf $(BUILD) $(BIN)
