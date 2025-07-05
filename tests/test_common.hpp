#pragma once
#include "test_utils.hpp"
#include <gtest/gtest.h>

using pascal::AST;
using pascal::Lexer;
using pascal::Parser;
using pascal::Token;
using std::make_unique;
using std::unique_ptr;
using std::vector;
using test_utils::run_full;
using test_utils::run_validation_fail;
using TT = pascal::TokenType;
