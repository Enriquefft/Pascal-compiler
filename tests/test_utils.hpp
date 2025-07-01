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

enum class TestMode { Tokens, TokensAst, TokensAstAsm, All };

inline constexpr TestMode TEST_MODE = TestMode::TokensAstAsm;

inline std::string execute_stub(std::string_view /*asm_code*/) { return {}; }

inline bool ast_equal_node(const pascal::ASTNode *a, const pascal::ASTNode *b) {
  if (!a || !b)
    return a == b;
  if (a->kind != b->kind)
    return false;
  using pascal::NodeKind;
  switch (a->kind) {
  case NodeKind::Program: {
    auto pa = static_cast<const pascal::Program *>(a);
    auto pb = static_cast<const pascal::Program *>(b);
    if (pa->name != pb->name)
      return false;
    return ast_equal_node(pa->block.get(), pb->block.get());
  }
  case NodeKind::Block: {
    auto ba = static_cast<const pascal::Block *>(a);
    auto bb = static_cast<const pascal::Block *>(b);
    if (ba->declarations.size() != bb->declarations.size() ||
        ba->statements.size() != bb->statements.size())
      return false;
    for (size_t i = 0; i < ba->declarations.size(); ++i)
      if (!ast_equal_node(ba->declarations[i].get(), bb->declarations[i].get()))
        return false;
    for (size_t i = 0; i < ba->statements.size(); ++i)
      if (!ast_equal_node(ba->statements[i].get(), bb->statements[i].get()))
        return false;
    return true;
  }
  case NodeKind::AssignStmt: {
    auto aa = static_cast<const pascal::AssignStmt *>(a);
    auto ab = static_cast<const pascal::AssignStmt *>(b);
    return ast_equal_node(aa->target.get(), ab->target.get()) &&
           ast_equal_node(aa->value.get(), ab->value.get());
  }
  case NodeKind::BinaryExpr: {
    auto ba = static_cast<const pascal::BinaryExpr *>(a);
    auto bb = static_cast<const pascal::BinaryExpr *>(b);
    return ba->op == bb->op && ast_equal_node(ba->left.get(), bb->left.get()) &&
           ast_equal_node(ba->right.get(), bb->right.get());
  }
  case NodeKind::LiteralExpr: {
    auto la = static_cast<const pascal::LiteralExpr *>(a);
    auto lb = static_cast<const pascal::LiteralExpr *>(b);
    return la->value == lb->value;
  }
  case NodeKind::VariableExpr: {
    auto va = static_cast<const pascal::VariableExpr *>(a);
    auto vb = static_cast<const pascal::VariableExpr *>(b);
    if (va->name != vb->name || va->selectors.size() != vb->selectors.size())
      return false;
    return true;
  }
  default:
    return true;
  }
}

inline bool ast_equal(const pascal::AST &a, const pascal::AST &b) {
  if (a.valid != b.valid)
    return false;
  return ast_equal_node(a.root.get(), b.root.get());
}

inline pascal::AST make_empty_ast(bool valid = true) {
  pascal::AST ast{};
  ast.valid = valid;
  return ast;
}

inline void run_full(std::string_view src,
                     const std::vector<Token> &expected_tokens,
                     const pascal::AST &expected_ast,
                     std::string_view expected_asm,
                     std::string_view expected_output) {
  Lexer lex(src);
  auto tokens = lex.scanTokens();
  ASSERT_EQ(tokens.size(), expected_tokens.size());

  for (size_t i = 0; i < expected_tokens.size(); ++i) {
    EXPECT_EQ(tokens[i].type, expected_tokens[i].type);
    EXPECT_EQ(tokens[i].lexeme, expected_tokens[i].lexeme);
  }

  if (TEST_MODE == TestMode::Tokens)
    return;

  Parser parser(tokens);
  AST ast{};
  EXPECT_NO_THROW({ ast = parser.parse(); });
  EXPECT_TRUE(ast_equal(ast, expected_ast));

  if (TEST_MODE == TestMode::TokensAst)
    return;

  ASTValidator validator;
  auto res = validator.validate(ast);
  EXPECT_TRUE(res.success) << res.message;

  CodeGenerator codegen;
  auto asm_code = codegen.generate(ast);
  EXPECT_EQ(asm_code, expected_asm);

  if (TEST_MODE == TestMode::TokensAstAsm)
    return;

  auto output = execute_stub(asm_code);
  EXPECT_EQ(output, expected_output);
}

inline void
run_validation_fail(std::string_view src,
                    const std::vector<Token> &expected_tokens,
                    const pascal::AST &expected_ast,
                    [[maybe_unused]] std::string_view expected_asm,
                    [[maybe_unused]] std::string_view expected_output) {
  Lexer lex(src);
  auto tokens = lex.scanTokens();
  ASSERT_EQ(tokens.size(), expected_tokens.size());

  for (size_t i = 0; i < expected_tokens.size(); ++i) {
    EXPECT_EQ(tokens[i].type, expected_tokens[i].type);
    EXPECT_EQ(tokens[i].lexeme, expected_tokens[i].lexeme);
  }

  Parser parser(tokens);
  AST ast{};
  EXPECT_NO_THROW({ ast = parser.parse(); });
  EXPECT_TRUE(ast_equal(ast, expected_ast));

  ASTValidator validator;
  auto res = validator.validate(ast);
  EXPECT_FALSE(res.success);
}

} // namespace test_utils
