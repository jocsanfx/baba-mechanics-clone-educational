#	target: dependencias | pre-requisitos
#		acciones

# $@ -> nombre del target
# $^ -> nombre de todas las dependencias
# $< -> nombre de la primera dependencia

# VARIABLES
XC=c++
STD=-std=c++20
WFLAGS=-Wall -Wextra
INCLUDES=-lraylib -lGL -lm -lpthread -ldl -lrt -lX11
APPNAME=program

# DIRECTORIOS
SRC=src
BUILD=build
BIN=bin

# ARCHIVOS
DIR=$(shell find -L $(SRC) -type d)
FHEADER=$(wildcard $(DIR:%=%/*.hpp))
FSOURCE=$(wildcard $(DIR:%=%/*.cpp))
FOBJECT=$(FSOURCE:$(SRC)/%.cpp=$(BUILD)/%.o)

.PHONY: all clean run

all: $(BIN)/$(APPNAME)

$(BIN)/$(APPNAME): $(FOBJECT) | $(BIN)
	$(XC) $(STD) $(WFLAGS) -g $^ $(INCLUDES) -o $@

$(BUILD)/%.o: $(SRC)/%.cpp | $(BUILD)
	$(XC) $(STD) $(WFLAG) -c -g $^ -o $@

$(BUILD):
	mkdir -p $@

$(BIN):
	mkdir -p $@

clean:
	rm -rf $(BUILD) $(BIN)

run:
	./$(BIN)/$(APPNAME)

show:
	echo $(FOBJECT)
