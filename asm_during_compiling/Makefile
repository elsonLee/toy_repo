CPP = g++
CFLAGS = -std=c++17 -Wall -I./include
LDFLAG = 
SOURCES = main.cpp
OBJ_DIR = ./objs
OBJECTS = $(addprefix $(OBJ_DIR)/, $(notdir $(SOURCES:.cpp=.o)))
TARGET = main

TEST_BINARY = AddressingTest
TEST_DIR = ./test
TEST_OBJ_DIR = $(OBJ_DIR)/test
TEST_OBJ = $(TEST_BINARY:%=$(TEST_OBJ_DIR)/%.o)

.PHONY: all test make_dir clean

default: make_dir $(OBJECTS) $(TARGET)
all: make_dir $(OBJECTS) $(TEST_OBJ) $(TARGET) $(TEST_BINARY)
test: make_dir $(TEST_OBJ) $(TEST_BINARY)

make_dir: $(OBJ_DIR) $(TEST_OBJ_DIR)

$(TEST_BINARY): $(TEST_OBJ)
	$(CPP) $(LDFLAG) $^ -o $@

$(TARGET): $(OBJECTS)
	$(CPP) $(LDFLAG) $^ -o $@

$(OBJ_DIR)/%.o: %.cpp
	$(CPP) $(CFLAGS) -c $< -o $@

$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CPP) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $@

$(TEST_OBJ_DIR):
	@mkdir -p $@

clean:
	@rm -rf $(OBJ_DIR) $(TARGET) $(TEST_BINARY)
