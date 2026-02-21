TARGET      = main

SRC_DIR     = src
OBJ_DIR     = obj
BIN_DIR     = bin

SRC         = $(shell find $(SRC_DIR) -name "*.cpp")
OBJ         = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

CXX         = g++
CXXFLAGS    = -Wall -Wextra -O2 -std=c++17 -MMD -MP
LDFLAGS     = -lraylib -lm -lpthread -ldl -lGL -lX11

all: $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(OBJ:.o=.d)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

run: all
	@./$(BIN_DIR)/$(TARGET)
