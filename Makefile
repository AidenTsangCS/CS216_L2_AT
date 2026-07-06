CXX := clang++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -g -O0
BUILD_DIR := build

SOURCES := $(wildcard *.cpp)
TARGETS := $(patsubst %.cpp,$(BUILD_DIR)/%,$(SOURCES))

.PHONY: all clean run

all: $(TARGETS)

$(BUILD_DIR)/%: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Usage: make run FILE=main
run: $(BUILD_DIR)/$(FILE)
	./$(BUILD_DIR)/$(FILE)

clean:
	rm -rf $(BUILD_DIR)
