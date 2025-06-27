CXX ?= g++
MODE ?= debug

WARN = -Werror -Wall -Wextra -Wpedantic -Wformat=2 -Wshadow -Wwrite-strings \
       -Wcast-qual -Wcast-align -Wconversion -Wno-switch \
       -Wno-ignored-optimization-argument
ifeq ($(CXX),clang++)
WARN += -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic \
        -Wno-c++20-compat -Wno-c++20-extensions -Wno-c99-extensions \
        -Wno-zero-as-null-pointer-constant -Wno-padded \
        -Wno-global-constructors -Wno-exit-time-destructors \
        -Wno-unused-macros
else ifeq ($(CXX),g++)
WARN += -fconcepts-diagnostics-depth=3
endif

SANITIZE = -fsanitize=address,undefined -fsanitize-address-use-after-scope
ifeq ($(MODE),release)
OPT = -O3
else
OPT = -ggdb3 -O0 $(SANITIZE)
endif

CXXFLAGS = -std=c++23 $(WARN) $(OPT) -Iinclude -Isrc
SRC_ALL = $(wildcard src/*.cpp src/token/*.cpp src/scanner/*.cpp \
                       src/parser/*.cpp src/visitors/*.cpp)
SRC = $(SRC_ALL)
SRC_TEST = $(filter-out src/main.cpp,$(SRC_ALL))
TEST_SRC = $(wildcard tests/*.cpp)
BUILD_DIR = build
TARGET = $(BUILD_DIR)/compiler
TEST_BIN = $(BUILD_DIR)/tests

all: compiler

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

compiler: $(TARGET)

$(TARGET): $(SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

.PHONY: tests clean

tests: $(TEST_BIN)

$(TEST_BIN): $(TEST_SRC) $(SRC_TEST) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(TEST_SRC) $(SRC_TEST) -lgtest -lgtest_main -lpthread -o $(TEST_BIN)
	cd tests && ./run_tests.sh

clean:
	rm -rf $(BUILD_DIR)
