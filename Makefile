# === Compilateur et options ===
CXX = g++
CXXFLAGS = -Wall -O2 -std=c++17 -Iinclude

# === Dossiers ===
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# === Fichiers sources ===
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# === Nom de l'exécutable ===
EXEC = $(BIN_DIR)/pfsp_solver

# === Règle par défaut ===
all: $(EXEC)

# === Compilation de l'exécutable ===
$(EXEC): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# === Compilation des .o ===
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# === Créer dossiers bin/ et obj/ si nécessaires ===
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# === Nettoyage ===
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean
