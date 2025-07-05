CXX = clang++
CXX := ccache $(CXX)
LD_FLAGS :=
MAKEFLAGS += -j$(shell nproc)

MODE = debug

# THROW_ON_ERROR = -Werror
THROW_ON_ERROR =

WARN = ${THROW_ON_ERROR} -Wall -Wextra -Wpedantic -Wformat=2 -Wshadow -Wwrite-strings \
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
OPT = -ggdb3 $(SANITIZE)
endif

DEPFLAGS = -MMD -MP
CXXFLAGS = -std=c++20 $(WARN) $(OPT) $(DEPFLAGS) -Iinclude -Isrc
NONDEPFLAGS = $(filter-out $(DEPFLAGS),$(CXXFLAGS))

SRC_ALL := $(wildcard src/*.cpp src/token/*.cpp src/scanner/*.cpp \
                       src/parser/*.cpp src/visitors/*.cpp src/executor/*.cpp)
SRC := $(filter-out src/api.cpp,$(SRC_ALL))
API_SRC := $(filter-out src/main.cpp,$(SRC_ALL))
SRC_TEST := $(filter-out src/main.cpp src/api.cpp,$(SRC_ALL))

ifdef FILE
TEST_SRC := $(addprefix tests/,$(FILE))
else
TEST_SRC := $(wildcard tests/*.cpp)
endif

BUILD_DIR := build
TARGET := $(BUILD_DIR)/compiler
API_BIN := $(BUILD_DIR)/api
TEST_BIN := $(BUILD_DIR)/tests

OBJS := $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(SRC))
API_OBJS := $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(API_SRC))

.PHONY: all api tests clean
all: compiler

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/%.o: src/%.cpp | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LD_FLAGS) $^ -o $@

compiler: $(TARGET)

api: $(API_BIN)
$(API_BIN): $(API_OBJS)
	$(CXX) $(CXXFLAGS) $(LD_FLAGS) $^ -o $@

ifdef FILE
tests: clean $(TEST_BIN)
else
tests: $(TEST_BIN)
endif
	cd tests && ./run_tests.sh $(FILTER)

$(TEST_BIN): $(TEST_SRC) $(SRC_TEST) | $(BUILD_DIR)
	$(CXX) $(NONDEPFLAGS) $(LD_FLAGS) $(TEST_SRC) $(SRC_TEST) -lgtest -lgtest_main -lpthread -o $(TEST_BIN)

-include $(BUILD_DIR)/**/*.d
-include $(BUILD_DIR)/*.d
