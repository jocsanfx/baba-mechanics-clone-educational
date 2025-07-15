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
LIBS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -lwebpdemux -lwebp
SFLAGS := 

# Búsqueda de archivos
FSOURCE := $(wildcard $(SRC)/*.cpp $(SRC)/*/*.cpp)
FOBJECT := $(patsubst $(SRC)/%.cpp,$(BUILD)/%.o,$(FSOURCE))

.PHONY: all clean run asan msan


lint:
	cppcheck --enable=all --inconclusive --std=c++17 --quiet \
	--suppress=missingIncludeSystem src


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

asan:
	$(MAKE) CC=gcc SFLAGS=-fsanitize=address clean all
	./$(BIN)/$(APPNAME)

masan:
	$(MAKE) CC=clang SFLAGS=-fsanitize=address clean all
	./$(BIN)/$(APPNAME)

clean:
	rm -rf $(BUILD) $(BIN)
