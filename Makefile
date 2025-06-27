CXX ?= g++
CXXFLAGS = -std=c++23 -Wall -Wextra -Iinclude -Isrc
SRC = $(wildcard src/*.cpp src/compiler/*.cpp)
TEST_SRC = tests/sample.cpp
BUILD_DIR = build
TARGET = $(CXX)
TEST_BIN = gtest

all: compiler

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

compiler: $(TARGET)

$(TARGET): $(SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

.PHONY: tests clean

tests: $(TEST_BIN)

$(TEST_BIN): $(TEST_SRC) $(TARGET) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(TEST_SRC) -lgtest -lgtest_main -lpthread -o $(TEST_BIN)
	cd tests && ./run_tests.sh

clean:
	rm -rf $(BUILD_DIR)
