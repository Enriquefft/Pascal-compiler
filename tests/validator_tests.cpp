#include "test_utils.hpp"
#include <gtest/gtest.h>

using pascal::AST;
using pascal::Lexer;
using pascal::Parser;
using pascal::Token;
using std::make_unique;
using std::unique_ptr;
using std::vector;
using test_utils::run_validation_fail;
using TT = pascal::TokenType;

TEST(ASTValidatorTests, VarDeclMissingName) {
  std::string input_str = "var : integer;";
  vector<Token> expected_tokens = {
      {TT::Var, "var"},    {TT::Colon, ":"},
      {TT::Identifier, "integer"}, {TT::Semicolon, ";"},
      {TT::EndOfFile, ""}};
  AST expected_ast{};
  vector<unique_ptr<pascal::Declaration>> decls;
  decls.emplace_back(make_unique<pascal::VarDecl>(vector<std::string>{},
              make_unique<pascal::SimpleTypeSpec>(pascal::BasicType::Integer,
                                                  "integer")));
  vector<unique_ptr<pascal::Statement>> stmts;
  auto block = make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root = make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;
  run_validation_fail(input_str, expected_tokens, expected_ast, "", "");
}

TEST(ASTValidatorTests, InvalidArrayRange) {
  std::string input_str = "type T = array[2..1] of integer;";
  vector<Token> expected_tokens = {
      {TT::Type, "type"},      {TT::Identifier, "T"},
      {TT::Equal, "="},        {TT::Array, "array"},
      {TT::LeftBracket, "["},  {TT::Number, "2"},
      {TT::Dot, "."},          {TT::Dot, "."},
      {TT::Number, "1"},       {TT::RightBracket, "]"},
      {TT::Of, "of"},          {TT::Identifier, "integer"},
      {TT::Semicolon, ";"},    {TT::EndOfFile, ""}};
  AST expected_ast{};
  vector<unique_ptr<pascal::Declaration>> decls;
  vector<pascal::Range> ranges;
  ranges.emplace_back(2,1);
  decls.emplace_back(make_unique<pascal::TypeDecl>(
      "T", make_unique<pascal::ArrayTypeSpec>(std::move(ranges),
          make_unique<pascal::SimpleTypeSpec>(pascal::BasicType::Integer,
                                              "integer"))));
  vector<unique_ptr<pascal::Statement>> stmts;
  auto block = make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root = make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;
  run_validation_fail(input_str, expected_tokens, expected_ast, "", "");
}
