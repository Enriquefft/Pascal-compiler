#include "test_common.hpp"

TEST(StringTests, Str1) {
  std::string input_str = "var s: string;";
  std::vector<Token> expected_tokens = {
      {TT::Var, "var"},           {TT::Identifier, "s"}, {TT::Colon, ":"},
      {TT::Identifier, "string"}, {TT::Semicolon, ";"},  {TT::EndOfFile, ""}};
  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  decls.emplace_back(std::make_unique<pascal::VarDecl>(
      std::vector<std::string>{"s"}, std::make_unique<pascal::SimpleTypeSpec>(
                                         pascal::BasicType::String, "string")));
  std::vector<std::unique_ptr<pascal::Statement>> stmts;

  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  std::string expected_asm = "section .bss\n"
                             "s:    resq    1\n\n"
                             "section .text\n"
                             "global main\n"
                             "main:\n"
                             "    ret\n";
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}

TEST(StringTests, Str2) {
  std::string input_str = "s:='hi';";
  std::vector<Token> expected_tokens = {
      {TT::Identifier, "s"}, {TT::Colon, ":"},       {TT::Assign, "="},
      {TT::Identifier, "'"}, {TT::Identifier, "hi"}, {TT::Identifier, "'"},
      {TT::Semicolon, ";"},  {TT::EndOfFile, ""}};
  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  stmts.emplace_back(std::make_unique<pascal::AssignStmt>(
      std::make_unique<pascal::VariableExpr>("s"),
      std::make_unique<pascal::LiteralExpr>("'hi'")));

  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  std::string expected_asm = "section .data\n"
                             "str0: db \"hi\", 0\n\n"
                             "section .bss\n"
                             "s:    resq    1\n\n"
                             "section .text\n"
                             "global main\n"
                             "main:\n"
                             "    mov    qword [s], str0\n"
                             "    ret\n";
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}

TEST(StringTests, Str3) {
  std::string input_str = "s:=s+'!';";
  std::vector<Token> expected_tokens = {
      {TT::Identifier, "s"}, {TT::Colon, ":"},      {TT::Assign, "="},
      {TT::Identifier, "s"}, {TT::Plus, "+"},       {TT::Identifier, "'"},
      {TT::Identifier, "!"}, {TT::Identifier, "'"}, {TT::Semicolon, ";"},
      {TT::EndOfFile, ""}};
  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  stmts.emplace_back(std::make_unique<pascal::AssignStmt>(
      std::make_unique<pascal::VariableExpr>("s"),
      std::make_unique<pascal::BinaryExpr>(
          std::make_unique<pascal::VariableExpr>("s"), "+",
          std::make_unique<pascal::LiteralExpr>("'!'"))));

  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  std::string expected_asm = "section .data\n"
                             "str0: db \"!\", 0\n\n"
                             "section .bss\n"
                             "s:    resq    1\n\n"
                             "section .text\n"
                             "global main\n"
                             "main:\n"
                             "    mov    qword [s], str0\n"
                             "    ret\n";
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}

TEST(StringTests, Str4) {
  std::string input_str = "writeln(s);";
  std::vector<Token> expected_tokens = {
      {TT::Identifier, "writeln"}, {TT::LeftParen, "("}, {TT::Identifier, "s"},
      {TT::RightParen, ")"},       {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  {
    std::vector<std::unique_ptr<pascal::Expression>> args;
    args.emplace_back(std::make_unique<pascal::VariableExpr>("s"));
    stmts.emplace_back(
        std::make_unique<pascal::ProcCall>("writeln", std::move(args)));
  }

  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  std::string expected_asm = "section .bss\n"
                             "s:    resq    1\n\n"
                             "section .text\n"
                             "extern puts\n"
                             "global main\n"
                             "main:\n"
                             "    mov    rdi, [s]\n"
                             "    call   puts\n"
                             "    ret\n";
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}

TEST(StringTests, Str5) {
  std::string input_str = "if s='' then s:='a';";
  std::vector<Token> expected_tokens = {
      {TT::If, "if"},        {TT::Identifier, "s"}, {TT::Equal, "="},
      {TT::Identifier, "'"}, {TT::Identifier, "'"}, {TT::Then, "then"},
      {TT::Identifier, "s"}, {TT::Colon, ":"},      {TT::Assign, "="},
      {TT::Identifier, "'"}, {TT::Identifier, "a"}, {TT::Identifier, "'"},
      {TT::Semicolon, ";"},  {TT::EndOfFile, ""}};
  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;

  std::vector<std::unique_ptr<pascal::Statement>> body;
  body.emplace_back(std::make_unique<pascal::AssignStmt>(
      std::make_unique<pascal::VariableExpr>("s"),
      std::make_unique<pascal::LiteralExpr>("'a'")));

  auto cmp = std::make_unique<pascal::BinaryExpr>(
      std::make_unique<pascal::VariableExpr>("s"), "=",
      std::make_unique<pascal::LiteralExpr>("''"));
  stmts.emplace_back(std::make_unique<pascal::IfStmt>(std::move(cmp),
                                                      std::move(body.front())));

  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  std::string expected_asm = "section .data\n"
                             "str0: db 0\n"
                             "str1: db \"a\", 0\n\n"
                             "section .bss\n"
                             "s:    resq    1\n\n"
                             "section .text\n"
                             "global main\n"
                             "main:\n"
                             "    mov    rax, [s]\n"
                             "    cmp    rax, str0\n"
                             "    jne    L1\n"
                             "    mov    qword [s], str1\n"
                             "L1:\n"
                             "    ret\n";
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}


