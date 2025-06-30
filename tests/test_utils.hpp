#pragma once

#include "parser/parser.hpp"
#include "parser/validator.hpp"
#include "scanner/lexer.hpp"
#include "visitors/codegen.hpp"
#include <cctype>
#include <gtest/gtest.h>
#include <string_view>
#include <vector>

namespace test_utils {
using pascal::AST;
using pascal::ASTValidator;
using pascal::CodeGenerator;
using pascal::Lexer;
using pascal::Parser;
using pascal::Token;
using pascal::TokenType;

inline Token make_token(std::string_view text) {
  Token tok{};
  if (!text.empty() && std::isdigit(static_cast<unsigned char>(text.front()))) {
    tok.type = TokenType::Number;
  } else if (!text.empty() && text.front() == '\'' && text.back() == '\'') {
    tok.type = TokenType::String;
  } else {
    tok.type = TokenType::Identifier;
  }
  tok.lexeme = std::string(text);
  return tok;
}

inline std::vector<Token> naive_tokenize(std::string_view src) {
  std::vector<Token> tokens;
  std::string current;
  for (char c : src) {
    if (std::isspace(static_cast<unsigned char>(c))) {
      if (!current.empty()) {
        tokens.push_back(make_token(current));
        current.clear();
      }
    } else if (std::ispunct(static_cast<unsigned char>(c))) {
      if (!current.empty()) {
        tokens.push_back(make_token(current));
        current.clear();
      }
      std::string s(1, c);
      tokens.push_back(make_token(s));
    } else {
      current.push_back(c);
    }
  }
  if (!current.empty()) {
    tokens.push_back(make_token(current));
  }
  tokens.push_back({TokenType::EndOfFile, ""});
  return tokens;
}

inline std::vector<Token>
tokens(std::initializer_list<std::string_view> lexemes) {
  std::vector<Token> result;
  for (auto l : lexemes) {
    result.push_back(make_token(l));
  }
  result.push_back({TokenType::EndOfFile, ""});
  return result;
}

struct ExpectedResults {
  std::vector<Token> tokens;
  AST ast{};
  std::string asm_code;
  std::string output;
};

inline ExpectedResults
make_expected(std::initializer_list<std::string_view> lexemes,
              std::string_view asm_code = "section .text",
              std::string_view output = {}) {
  ExpectedResults e{};
  e.tokens = tokens(lexemes);
  e.ast.valid = true;
  e.asm_code = std::string(asm_code);
  e.output = std::string(output);
  return e;
}

inline std::string execute_stub(std::string_view /*asm_code*/) { return {}; }

inline ExpectedResults default_expected(std::string_view src) {
  ExpectedResults e{};
  e.tokens = naive_tokenize(src);
  e.ast.valid = true;
  e.asm_code = "section .text";
  e.output = {};
  return e;
}

inline void run_full(std::string_view src, const ExpectedResults &expected) {
  Lexer lex(src);
  auto tokens = lex.scanTokens();
  ASSERT_EQ(tokens.size(), expected.tokens.size());
  for (size_t i = 0; i < tokens.size(); ++i) {
    EXPECT_EQ(tokens[i].type, expected.tokens[i].type);
    EXPECT_EQ(tokens[i].lexeme, expected.tokens[i].lexeme);
  }

  Parser parser(tokens);
  AST ast{};
  EXPECT_NO_THROW({ ast = parser.parse(); });
  EXPECT_EQ(ast.valid, expected.ast.valid);

  ASTValidator validator;
  EXPECT_TRUE(validator.validate(ast));

  CodeGenerator codegen;
  auto asm_code = codegen.generate(ast);
  EXPECT_EQ(asm_code, expected.asm_code);

  auto output = execute_stub(asm_code);
  EXPECT_EQ(output, expected.output);
}

} // namespace test_utils
