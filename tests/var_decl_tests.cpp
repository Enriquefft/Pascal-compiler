#include "test_common.hpp"

TEST(VarDeclTests, Decl1) {
  std::string input_str = "program test; "
                          "var a: integer; "
                          "begin "
                          "end.";
  vector<Token> expected_tokens = {
      {TT::Program, "program"}, {TT::Identifier, "test"},
      {TT::Semicolon, ";"},     {TT::Var, "var"},
      {TT::Identifier, "a"},    {TT::Colon, ":"},
      {TT::Identifier, "integer"}, {TT::Semicolon, ";"},
      {TT::Begin, "begin"},     {TT::End, "end"},
      {TT::Dot, "."},           {TT::EndOfFile, ""}};

  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<pascal::VarDecl> varDecls;
  varDecls.emplace_back(
      std::vector<std::string>{"a"},
      std::make_unique<pascal::SimpleTypeSpec>(pascal::BasicType::Integer,
                                               "integer"));
  decls.emplace_back(std::make_unique<pascal::VarSection>(varDecls));

  // empty statements vector
  std::vector<std::unique_ptr<pascal::Statement>> stmts;

  // assemble AST
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
                             "    ret\n";
  std::string expected_output = "";
  run_full(input_str, expected_tokens, expected_ast, expected_asm,
           expected_output);
}

// Variable declarations
TEST(VarDeclTests, Decl2) {
  std::string input_str = "program test; "
                          "var b: real; "
                          "begin "
                          "end.";
  std::vector<Token> expected_tokens = {
      {TT::Program, "program"}, {TT::Identifier, "test"},
      {TT::Semicolon, ";"},     {TT::Var, "var"},
      {TT::Identifier, "b"},    {TT::Colon, ":"},
      {TT::Identifier, "real"}, {TT::Semicolon, ";"},
      {TT::Begin, "begin"},     {TT::End, "end"},
      {TT::Dot, "."},           {TT::EndOfFile, ""}};
  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<pascal::VarDecl> varDecls;
  varDecls.emplace_back(std::vector<std::string>{"b"},
                        std::make_unique<pascal::SimpleTypeSpec>(
                            pascal::BasicType::Real, "real"));
  decls.emplace_back(std::make_unique<pascal::VarSection>(varDecls));

  std::vector<std::unique_ptr<pascal::Statement>> stmts;

  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  std::string expected_asm = "section .bss\n"
                             "b:    resq    1\n\n"
                             "section .text\n"
                             "global main\n"
                             "main:\n"
                             "    ret\n";
  std::string expected_output = "";

  run_full(input_str, expected_tokens, expected_ast, expected_asm,
           expected_output);
}

TEST(VarDeclTests, Decl3) {
  std::string input_str = "program test; "
                          "var c: unsigned; "
                          "begin "
                          "end.";
  std::vector<Token> expected_tokens = {
      {TT::Program, "program"}, {TT::Identifier, "test"},
      {TT::Semicolon, ";"},     {TT::Var, "var"},
      {TT::Identifier, "c"},    {TT::Colon, ":"},
      {TT::Identifier, "unsigned"}, {TT::Semicolon, ";"},
      {TT::Begin, "begin"},     {TT::End, "end"},
      {TT::Dot, "."},           {TT::EndOfFile, ""}};
  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<pascal::VarDecl> varDecls;
  varDecls.emplace_back(
      std::vector<std::string>{"c"},
      std::make_unique<pascal::SimpleTypeSpec>(pascal::BasicType::UnsignedInt,
                                               "unsigned"));
  decls.emplace_back(std::make_unique<pascal::VarSection>(varDecls));

  std::vector<std::unique_ptr<pascal::Statement>> stmts;

  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  std::string expected_asm = "section .bss\n"
                             "c:    resq    1\n\n"
                             "section .text\n"
                             "global main\n"
                             "main:\n"
                             "    ret\n";
  std::string expected_output = "";

  run_full(input_str, expected_tokens, expected_ast, expected_asm,
           expected_output);
}


