#include "test_common.hpp"

TEST(DynamicTests, Dyn1) {
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
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}

TEST(DynamicTests, Dyn2) {
  std::string input_str = "dispose(p);";
  std::vector<Token> expected_tokens = {
      {TT::Dispose, "dispose"}, {TT::LeftParen, "("}, {TT::Identifier, "p"},
      {TT::RightParen, ")"},    {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  {
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

TEST(DynamicTests, Dyn3) {
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
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}

TEST(DynamicTests, Dyn4) {
  std::string input_str = "p^:=1;";
  std::vector<Token> expected_tokens = {
      {TT::Identifier, "p"}, {TT::Caret, "^"},  {TT::Colon, ":"},
      {TT::Assign, "="},     {TT::Number, "1"}, {TT::Semicolon, ";"},
      {TT::EndOfFile, ""}};
  AST expected_ast{};

  std::vector<pascal::VariableExpr::Selector> sels;
  sels.emplace_back("", // empty field
                    pascal::VariableExpr::Selector::Kind::Pointer);

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  stmts.emplace_back(std::make_unique<pascal::AssignStmt>(
      std::make_unique<pascal::VariableExpr>(
          "p",

          std::move(sels) // moves the selector vector in
          ),
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

TEST(DynamicTests, Dyn5) {
  std::string input_str = "if p<>nil then dispose(p);";
  std::vector<Token> expected_tokens = {
      {TT::If, "if"},           {TT::Identifier, "p"},   {TT::Less, "<"},
      {TT::Greater, ">"},       {TT::Identifier, "nil"}, {TT::Then, "then"},
      {TT::Dispose, "dispose"}, {TT::LeftParen, "("},    {TT::Identifier, "p"},
      {TT::RightParen, ")"},    {TT::Semicolon, ";"},    {TT::EndOfFile, ""}};
  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  {
    auto neq = std::make_unique<pascal::BinaryExpr>(
        std::make_unique<pascal::VariableExpr>("p"), "<>",
        std::make_unique<pascal::VariableExpr>("nil"));
    std::vector<std::unique_ptr<pascal::Expression>> disposeArgs;
    disposeArgs.emplace_back(std::make_unique<pascal::VariableExpr>("p"));
    stmts.emplace_back(std::make_unique<pascal::IfStmt>(
        std::move(neq),
        std::make_unique<pascal::ProcCall>("dispose", std::move(disposeArgs))));
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
                             "    mov    rax, [p]\n"
                             "    cmp    rax, 0\n"
                             "    je     L1\n"
                             "    mov    rdi, rax\n"
                             "    call   free\n"
                             "L1:\n"
                             "    ret\n";
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}


