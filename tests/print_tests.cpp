#include "test_common.hpp"

TEST(PrintTests, PrintString) {
  std::string input_str = "program test; "
                          "begin "
                          "writeln('hello'); "
                          "end.";
  std::vector<Token> expected_tokens = {
      {TT::Program, "program"},  {TT::Identifier, "test"},
      {TT::Semicolon, ";"},      {TT::Begin, "begin"},
      {TT::Identifier, "writeln"}, {TT::LeftParen, "("},
      {TT::Identifier, "'"},      {TT::Identifier, "hello"},
      {TT::Identifier, "'"},      {TT::RightParen, ")"},
      {TT::Semicolon, ";"},      {TT::End, "end"},
      {TT::Dot, "."},            {TT::EndOfFile, ""}};
  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  {
    std::vector<std::unique_ptr<pascal::Expression>> args;
    args.emplace_back(std::make_unique<pascal::LiteralExpr>("'hello'"));
    stmts.emplace_back(
        std::make_unique<pascal::ProcCall>("writeln", std::move(args)));
  }

  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  std::string expected_asm = "section .data\n"
                             "str0: db \"hello\", 0\n\n"
                             "section .text\n"
                             "extern puts\n"
                             "global main\n"
                             "main:\n"
                             "    mov    rdi, str0\n"
                             "    call   puts\n"
                             "    ret\n";
  std::string expected_output = "hello\n";
  run_full(input_str, expected_tokens, expected_ast, expected_asm,
           expected_output);
}

TEST(PrintTests, PrintIntLiteral) {
  std::string input_str = "program test; "
                          "begin "
                          "writeln(123); "
                          "end.";
  std::vector<Token> expected_tokens = {
      {TT::Program, "program"},  {TT::Identifier, "test"},
      {TT::Semicolon, ";"},      {TT::Begin, "begin"},
      {TT::Identifier, "writeln"}, {TT::LeftParen, "("},
      {TT::Number, "123"},       {TT::RightParen, ")"},
      {TT::Semicolon, ";"},      {TT::End, "end"},
      {TT::Dot, "."},            {TT::EndOfFile, ""}};
  AST expected_ast{};

  // build the proc-call arguments by moving into a vector
  std::vector<std::unique_ptr<pascal::Expression>> args;
  args.emplace_back(std::make_unique<pascal::LiteralExpr>("123"));

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  stmts.emplace_back(
      std::make_unique<pascal::ProcCall>("writeln", std::move(args)));

  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  std::string expected_asm = "section .data\n"
                             "fmt_int: db \"%d\", 10, 0\n\n"
                             "section .text\n"
                             "extern printf\n"
                             "global main\n"
                             "main:\n"
                             "    mov    rdi, fmt_int\n"
                             "    mov    rsi, 123\n"
                             "    xor    rax, rax\n"
                             "    call   printf\n"
                             "    ret\n";
  std::string expected_output = "123\n";

  run_full(input_str, expected_tokens, expected_ast, expected_asm,
           expected_output);
}

TEST(PrintTests, PrintIntExpr) {
  std::string input_str = "program test; "
                          "begin "
                          "writeln(10+20); "
                          "end.";
  std::vector<Token> expected_tokens = {
      {TT::Program, "program"},  {TT::Identifier, "test"},
      {TT::Semicolon, ";"},      {TT::Begin, "begin"},
      {TT::Identifier, "writeln"}, {TT::LeftParen, "("},
      {TT::Number, "10"},         {TT::Plus, "+"},
      {TT::Number, "20"},         {TT::RightParen, ")"},
      {TT::Semicolon, ";"},      {TT::End, "end"},
      {TT::Dot, "."},            {TT::EndOfFile, ""}};
  AST expected_ast{};

  // build the expression argument in a vector
  std::vector<std::unique_ptr<pascal::Expression>> args;
  args.emplace_back(std::make_unique<pascal::BinaryExpr>(
      std::make_unique<pascal::LiteralExpr>("10"), "+",
      std::make_unique<pascal::LiteralExpr>("20")));

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  stmts.emplace_back(
      std::make_unique<pascal::ProcCall>("writeln", std::move(args)));

  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  std::string expected_asm = "section .data\n"
                             "fmt_int: db \"%d\", 10, 0\n\n"
                             "section .text\n"
                             "extern printf\n"
                             "global main\n"
                             "main:\n"
                             "    mov    rdi, fmt_int\n"
                             "    mov    rsi, 10\n"
                             "    add    rsi, 20\n"
                             "    xor    rax, rax\n"
                             "    call   printf\n"
                             "    ret\n";
  std::string expected_output = "30\n";

  run_full(input_str, expected_tokens, expected_ast, expected_asm,
           expected_output);
}


