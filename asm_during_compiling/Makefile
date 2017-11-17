CPP = g++
CFLAGS = -std=c++17 -Wall -I./include
LDFLAG = 
SOURCES = main.cpp
OBJ_DIR = ./objs
OBJECTS = $(addprefix $(OBJ_DIR)/, $(notdir $(SOURCES:.cpp=.o)))
TARGET = main

.PHONY: all make_dir clean

all: make_dir $(SOURCES) $(TARGET)
make_dir: $(OBJ_DIR)

$(TARGET): $(OBJECTS)
	$(CPP) $(LDFLAG) $^ -o $@

$(OBJ_DIR)/%.o: %.cpp
	$(CPP) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $@

clean:
	@rm -rf $(OBJ_DIR) $(TARGET)
