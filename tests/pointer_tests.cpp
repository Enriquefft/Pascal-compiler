#include "test_common.hpp"

TEST(PointerTests, Ptr1) {
  std::string input_str = "var p:^integer;";
  std::vector<Token> expected_tokens = {
      {TT::Var, "var"},   {TT::Identifier, "p"},       {TT::Colon, ":"},
      {TT::Caret, "^"},   {TT::Identifier, "integer"}, {TT::Semicolon, ";"},
      {TT::EndOfFile, ""}};
  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  decls.emplace_back(std::make_unique<pascal::VarDecl>(
      std::vector<std::string>{"p"},
      std::make_unique<pascal::PointerTypeSpec>(
          std::make_unique<pascal::SimpleTypeSpec>(pascal::BasicType::Integer,
                                                   "integer"))));
  std::vector<std::unique_ptr<pascal::Statement>> stmts;

  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  std::string expected_asm = "section .bss\n"
                             "p:    resq    1\n\n"
                             "section .text\n"
                             "global main\n"
                             "main:\n"
                             "    ret\n";
  std::string expected_output = "";
  run_full(input_str, expected_tokens, expected_ast, expected_asm,
           expected_output);
}

TEST(PointerTests, Ptr2) {
  std::string input_str = "new(p);";
  std::vector<Token> expected_tokens = {
      {TT::New, "new"},      {TT::LeftParen, "("}, {TT::Identifier, "p"},
      {TT::RightParen, ")"}, {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  {
    std::vector<std::unique_ptr<pascal::Expression>> args;
    args.emplace_back(std::make_unique<pascal::VariableExpr>("p"));
    stmts.emplace_back(
        std::make_unique<pascal::ProcCall>("new", std::move(args)));
  }

  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  std::string expected_asm = "section .bss\n"
                             "p:    resq    1\n\n"
                             "section .text\n"
                             "extern malloc\n"
                             "global main\n"
                             "main:\n"
                             "    mov    rdi, 8\n"
                             "    call   malloc\n"
                             "    mov    qword [p], rax\n"
                             "    ret\n";
  std::string expected_output = "";
  run_full(input_str, expected_tokens, expected_ast, expected_asm,
           expected_output);
}

// Pointers
TEST(PointerTests, Ptr3) {
  std::string input_str = "p^:=1;";
  std::vector<Token> expected_tokens = {
      {TT::Identifier, "p"}, {TT::Caret, "^"},  {TT::Colon, ":"},
      {TT::Assign, "="},     {TT::Number, "1"}, {TT::Semicolon, ";"},
      {TT::EndOfFile, ""}};
  AST expected_ast{};

  // build selector in-place
  std::vector<pascal::VariableExpr::Selector> sels;
  sels.emplace_back("", pascal::VariableExpr::Selector::Kind::Pointer);

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  stmts.emplace_back(std::make_unique<pascal::AssignStmt>(
      // move the selector vector into VariableExpr
      std::make_unique<pascal::VariableExpr>("p", std::move(sels)),
      std::make_unique<pascal::LiteralExpr>("1")));

  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  std::string expected_asm = "section .bss\n"
                             "p:    resq    1\n\n"
                             "section .text\n"
                             "global main\n"
                             "main:\n"
                             "    mov    rax, [p]\n"
                             "    mov    qword [rax], 1\n"
                             "    ret\n";
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}

TEST(PointerTests, Ptr4) {
  std::string input_str = "dispose(p);";
  std::vector<Token> expected_tokens = {
      {TT::Dispose, "dispose"}, {TT::LeftParen, "("}, {TT::Identifier, "p"},
      {TT::RightParen, ")"},    {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  {
    // build call arguments by moving into vector
    std::vector<std::unique_ptr<pascal::Expression>> args;
    args.emplace_back(std::make_unique<pascal::VariableExpr>("p"));
    stmts.emplace_back(
        std::make_unique<pascal::ProcCall>("dispose", std::move(args)));
  }

  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  std::string expected_asm = "section .bss\n"
                             "p:    resq    1\n\n"
                             "section .text\n"
                             "extern free\n"
                             "global main\n"
                             "main:\n"
                             "    mov    rdi, [p]\n"
                             "    call   free\n"
                             "    ret\n";
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}


