# Compiler and flags
CXX = g++
CXXFLAGS = -Iinclude -Wall -Wextra -std=c++17 `pkg-config --cflags gtkmm-3.0`
LDFLAGS = `pkg-config --libs gtkmm-3.0`

# Directories
SRC_DIR = src
BUILD_DIR = build

# Find all source files recursively
SRC_FILES = $(shell find $(SRC_DIR) -name "*.cpp")
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))

# Target executable
TARGET = gzahl

# Rule to build the executable
$(TARGET): $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) -o $(TARGET) $(LDFLAGS)

# Rule to compile each source file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -rf $(BUILD_DIR) $(TARGET)
