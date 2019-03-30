SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

TARGET = vreme
CXX = g++
CXXFLAGS = -Wall -g
CPPFLAGS = -I $(INC_DIR)/

.PHONY: clean

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJECTS): $(SOURCES)
	@mkdir -p $(OBJ_DIR)/
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(TARGET)
