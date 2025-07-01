#include "test_common.hpp"

TEST(FunctionTests, Func1) {
  std::string input_str = "function f: integer; begin f:=0; end;";
  std::vector<Token> expected_tokens = {
      {TT::Function, "function"},  {TT::Identifier, "f"}, {TT::Colon, ":"},
      {TT::Identifier, "integer"}, {TT::Semicolon, ";"},  {TT::Begin, "begin"},
      {TT::Identifier, "f"},       {TT::Colon, ":"},      {TT::Assign, "="},
      {TT::Number, "0"},           {TT::Semicolon, ";"},  {TT::End, "end"},
      {TT::Semicolon, ";"},        {TT::EndOfFile, ""}};
  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::ParamDecl>> params;
  std::vector<std::unique_ptr<pascal::Statement>> fn_stmts;
  fn_stmts.emplace_back(std::make_unique<pascal::AssignStmt>(
      std::make_unique<pascal::VariableExpr>("f"),
      std::make_unique<pascal::LiteralExpr>("0")));
  auto fn_block = std::make_unique<pascal::Block>(
      std::vector<std::unique_ptr<pascal::Declaration>>{}, std::move(fn_stmts));
  decls.emplace_back(std::make_unique<pascal::FunctionDecl>(
      "f", std::move(params),
      std::make_unique<pascal::SimpleTypeSpec>(pascal::BasicType::Integer,
                                               "integer"),
      std::move(fn_block)));

  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  std::string expected_asm = "section .text\n"
                             "global f\n"
                             "f:\n"
                             "    mov    rax, 0\n"
                             "    ret\n"
                             "global main\n"
                             "main:\n"
                             "    ret\n";
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}

TEST(FunctionTests, Func2) {
  std::string input_str = "procedure p; begin end;";
  std::vector<Token> expected_tokens = {
      {TT::Procedure, "procedure"}, {TT::Identifier, "p"}, {TT::Semicolon, ";"},
      {TT::Begin, "begin"},         {TT::End, "end"},      {TT::Semicolon, ";"},
      {TT::EndOfFile, ""}};
  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::ParamDecl>> params;
  auto proc_block = std::make_unique<pascal::Block>(
      std::vector<std::unique_ptr<pascal::Declaration>>{},
      std::vector<std::unique_ptr<pascal::Statement>>{});
  decls.emplace_back(std::make_unique<pascal::ProcedureDecl>(
      "p", std::move(params), std::move(proc_block)));

  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  std::string expected_asm = "section .text\n"
                             "global p\n"
                             "p:\n"
                             "    ret\n"
                             "global main\n"
                             "main:\n"
                             "    ret\n";
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}

TEST(FunctionTests, Func3) {
  std::string input_str = "function g(x: integer): integer; begin g:=x; end;";
  std::vector<Token> expected_tokens = {{TT::Function, "function"},
                                        {TT::Identifier, "g"},
                                        {TT::LeftParen, "("},
                                        {TT::Identifier, "x"},
                                        {TT::Colon, ":"},
                                        {TT::Identifier, "integer"},
                                        {TT::RightParen, ")"},
                                        {TT::Colon, ":"},
                                        {TT::Identifier, "integer"},
                                        {TT::Semicolon, ";"},
                                        {TT::Begin, "begin"},
                                        {TT::Identifier, "g"},
                                        {TT::Colon, ":"},
                                        {TT::Assign, "="},
                                        {TT::Identifier, "x"},
                                        {TT::Semicolon, ";"},
                                        {TT::End, "end"},
                                        {TT::Semicolon, ";"},
                                        {TT::EndOfFile, ""}};
  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::ParamDecl>> params;
  params.emplace_back(std::make_unique<pascal::ParamDecl>(
      std::vector<std::string>{"x"},
      std::make_unique<pascal::SimpleTypeSpec>(pascal::BasicType::Integer,
                                               "integer")));
  std::vector<std::unique_ptr<pascal::Statement>> fn_stmts;
  fn_stmts.emplace_back(std::make_unique<pascal::AssignStmt>(
      std::make_unique<pascal::VariableExpr>("g"),
      std::make_unique<pascal::VariableExpr>("x")));
  auto fn_block = std::make_unique<pascal::Block>(
      std::vector<std::unique_ptr<pascal::Declaration>>{}, std::move(fn_stmts));
  decls.emplace_back(std::make_unique<pascal::FunctionDecl>(
      "g", std::move(params),
      std::make_unique<pascal::SimpleTypeSpec>(pascal::BasicType::Integer,
                                               "integer"),
      std::move(fn_block)));

  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  std::string expected_asm = "section .text\n"
                             "global g\n"
                             "g:\n"
                             "    mov    rax, rdi\n"
                             "    ret\n"
                             "global main\n"
                             "main:\n"
                             "    ret\n";
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}


