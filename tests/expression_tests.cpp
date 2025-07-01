#include "test_common.hpp"

TEST(ExpressionTests, Expr1) {
  std::string input_str = "program test; begin a := 1; end.";
  std::vector<Token> expected_tokens = {
      {TT::Program, "program"}, {TT::Identifier, "test"},
      {TT::Semicolon, ";"},    {TT::Begin, "begin"},
      {TT::Identifier, "a"},   {TT::Colon, ":"},
      {TT::Assign, "="},       {TT::Number, "1"},
      {TT::Semicolon, ";"},    {TT::End, "end"},
      {TT::Dot, "."},          {TT::EndOfFile, ""}};
  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;

  std::vector<std::unique_ptr<pascal::Statement>> inner;
  inner.emplace_back(std::make_unique<pascal::AssignStmt>(
      std::make_unique<pascal::VariableExpr>("a"),
      std::make_unique<pascal::LiteralExpr>("1")));
  stmts.emplace_back(std::make_unique<pascal::CompoundStmt>(std::move(inner)));

  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  std::string expected_asm = "section .bss\n"
                             "a:    resq    1\n\n"
                             "section .text\n"
                             "global main\n"
                             "main:\n"
                             "    mov    qword [a], 1\n"
                             "    ret\n";
  std::string expected_output = "";

  run_full(input_str, expected_tokens, expected_ast, expected_asm,
           expected_output);
}

TEST(ExpressionTests, Expr2) {
  std::string input_str = "program test; begin b := a + 1; end.";
  std::vector<Token> expected_tokens = {
      {TT::Program, "program"}, {TT::Identifier, "test"},
      {TT::Semicolon, ";"},    {TT::Begin, "begin"},
      {TT::Identifier, "b"},   {TT::Colon, ":"},
      {TT::Assign, "="},       {TT::Identifier, "a"},
      {TT::Plus, "+"},         {TT::Number, "1"},
      {TT::Semicolon, ";"},    {TT::End, "end"},
      {TT::Dot, "."},          {TT::EndOfFile, ""}};
  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;

  std::vector<std::unique_ptr<pascal::Statement>> inner;
  inner.emplace_back(std::make_unique<pascal::AssignStmt>(
      std::make_unique<pascal::VariableExpr>("b"),
      std::make_unique<pascal::BinaryExpr>(
          std::make_unique<pascal::VariableExpr>("a"), "+",
          std::make_unique<pascal::LiteralExpr>("1"))));
  stmts.emplace_back(std::make_unique<pascal::CompoundStmt>(std::move(inner)));

  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  std::string expected_asm = "section .bss\n"
                             "a:    resq    1\n"
                             "b:    resq    1\n\n"
                             "section .text\n"
                             "global main\n"
                             "main:\n"
                             "    mov    rax, [a]\n"
                             "    add    rax, 1\n"
                             "    mov    [b], rax\n"
                             "    ret\n";
  std::string expected_output = "";

  run_full(input_str, expected_tokens, expected_ast, expected_asm,
           expected_output);
}

TEST(ExpressionTests, Expr3) {
  std::string input_str = "program test; begin c := b * 2; end.";
  std::vector<Token> expected_tokens = {
      {TT::Program, "program"}, {TT::Identifier, "test"},
      {TT::Semicolon, ";"},    {TT::Begin, "begin"},
      {TT::Identifier, "c"},   {TT::Colon, ":"},
      {TT::Assign, "="},       {TT::Identifier, "b"},
      {TT::Star, "*"},         {TT::Number, "2"},
      {TT::Semicolon, ";"},    {TT::End, "end"},
      {TT::Dot, "."},          {TT::EndOfFile, ""}};
  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;

  std::vector<std::unique_ptr<pascal::Statement>> inner;
  inner.emplace_back(std::make_unique<pascal::AssignStmt>(
      std::make_unique<pascal::VariableExpr>("c"),
      std::make_unique<pascal::BinaryExpr>(
          std::make_unique<pascal::VariableExpr>("b"), "*",
          std::make_unique<pascal::LiteralExpr>("2"))));
  stmts.emplace_back(std::make_unique<pascal::CompoundStmt>(std::move(inner)));

  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  std::string expected_asm = "section .bss\n"
                             "b:    resq    1\n"
                             "c:    resq    1\n\n"
                             "section .text\n"
                             "global main\n"
                             "main:\n"
                             "    mov    rax, [b]\n"
                             "    imul   rax, 2\n"
                             "    mov    [c], rax\n"
                             "    ret\n";
  std::string expected_output = "";

  run_full(input_str, expected_tokens, expected_ast, expected_asm,
           expected_output);
}


