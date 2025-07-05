#include "test_common.hpp"

TEST(StringTests, Str1) {
  std::string input_str = "program test; "
                          "var s: string; "
                          "begin "
                          "end.";

  std::vector<Token> expected_tokens = {{TT::Program, "program"},
                                        {TT::Identifier, "test"},
                                        {TT::Semicolon, ";"},
                                        {TT::Var, "var"},
                                        {TT::Identifier, "s"},
                                        {TT::Colon, ":"},
                                        {TT::Identifier, "string"},
                                        {TT::Semicolon, ";"},
                                        {TT::Begin, "begin"},
                                        {TT::End, "end"},
                                        {TT::Dot, "."},
                                        {TT::EndOfFile, ""}};

  AST expected_ast{};
  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<pascal::VarDecl> varDecls;
  varDecls.emplace_back(std::vector<std::string>{"s"},
                        std::make_unique<pascal::SimpleTypeSpec>(
                            pascal::BasicType::String, "string"));
  decls.emplace_back(std::make_unique<pascal::VarSection>(varDecls));

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
  std::string input_str = "program test; "
                          "var s: string; "
                          "begin "
                          "s:='hi'; "
                          "end.";

  std::vector<Token> expected_tokens = {{TT::Program, "program"},
                                        {TT::Identifier, "test"},
                                        {TT::Semicolon, ";"},
                                        {TT::Var, "var"},
                                        {TT::Identifier, "s"},
                                        {TT::Colon, ":"},
                                        {TT::Identifier, "string"},
                                        {TT::Semicolon, ";"},
                                        {TT::Begin, "begin"},
                                        {TT::Identifier, "s"},
                                        {TT::Assign, ":="},
                                        {TT::Identifier, "'"},
                                        {TT::Identifier, "hi"},
                                        {TT::Identifier, "'"},
                                        {TT::Semicolon, ";"},
                                        {TT::End, "end"},
                                        {TT::Dot, "."},
                                        {TT::EndOfFile, ""}};

  AST expected_ast{};
  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<pascal::VarDecl> varDecls;
  varDecls.emplace_back(std::vector<std::string>{"s"},
                        std::make_unique<pascal::SimpleTypeSpec>(
                            pascal::BasicType::String, "string"));
  decls.emplace_back(std::make_unique<pascal::VarSection>(varDecls));

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
  std::string input_str = "program test; "
                          "var s: string; "
                          "begin "
                          "s:=s+'!'; "
                          "end.";

  std::vector<Token> expected_tokens = {{TT::Program, "program"},
                                        {TT::Identifier, "test"},
                                        {TT::Semicolon, ";"},
                                        {TT::Var, "var"},
                                        {TT::Identifier, "s"},
                                        {TT::Colon, ":"},
                                        {TT::Identifier, "string"},
                                        {TT::Semicolon, ";"},
                                        {TT::Begin, "begin"},
                                        {TT::Identifier, "s"},
                                        {TT::Assign, ":="},
                                        {TT::Identifier, "s"},
                                        {TT::Plus, "+"},
                                        {TT::Identifier, "'"},
                                        {TT::Identifier, "!"},
                                        {TT::Identifier, "'"},
                                        {TT::Semicolon, ";"},
                                        {TT::End, "end"},
                                        {TT::Dot, "."},
                                        {TT::EndOfFile, ""}};

  AST expected_ast{};
  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<pascal::VarDecl> varDecls;
  varDecls.emplace_back(std::vector<std::string>{"s"},
                        std::make_unique<pascal::SimpleTypeSpec>(
                            pascal::BasicType::String, "string"));
  decls.emplace_back(std::make_unique<pascal::VarSection>(varDecls));

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
  std::string input_str = "program test; "
                          "var s: string; "
                          "begin "
                          "writeln(s); "
                          "end.";

  std::vector<Token> expected_tokens = {
      {TT::Program, "program"},   {TT::Identifier, "test"},
      {TT::Semicolon, ";"},       {TT::Var, "var"},
      {TT::Identifier, "s"},      {TT::Colon, ":"},
      {TT::Identifier, "string"}, {TT::Semicolon, ";"},
      {TT::Begin, "begin"},       {TT::Identifier, "writeln"},
      {TT::LeftParen, "("},       {TT::Identifier, "s"},
      {TT::RightParen, ")"},      {TT::Semicolon, ";"},
      {TT::End, "end"},           {TT::Dot, "."},
      {TT::EndOfFile, ""}};

  AST expected_ast{};
  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<pascal::VarDecl> varDecls;
  varDecls.emplace_back(std::vector<std::string>{"s"},
                        std::make_unique<pascal::SimpleTypeSpec>(
                            pascal::BasicType::String, "string"));
  decls.emplace_back(std::make_unique<pascal::VarSection>(varDecls));

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

  std::string expected_asm =
      "section .data\nfmt_int: db \"%d\", 10, 0\n\n"
      "section .bss\ns:    resq    1\n\n"
      "section .text\nextern printf\nglobal main\nmain:\n"
      "    mov    rdi, fmt_int\n"
      "    mov    rax, [s]\n"
      "    mov    rsi, rax\n"
      "    xor    rax, rax\n"
      "    call   printf\n"
      "    ret\n";

  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}

TEST(StringTests, Str5) {
  std::string input_str = "program test; "
                          "var s: string; "
                          "begin "
                          "if s='' then s:='a'; "
                          "end.";

  std::vector<Token> expected_tokens = {{TT::Program, "program"},
                                        {TT::Identifier, "test"},
                                        {TT::Semicolon, ";"},
                                        {TT::Var, "var"},
                                        {TT::Identifier, "s"},
                                        {TT::Colon, ":"},
                                        {TT::Identifier, "string"},
                                        {TT::Semicolon, ";"},
                                        {TT::Begin, "begin"},
                                        {TT::If, "if"},
                                        {TT::Identifier, "s"},
                                        {TT::Equal, "="},
                                        {TT::Identifier, "'"},
                                        {TT::Identifier, "'"},
                                        {TT::Then, "then"},
                                        {TT::Identifier, "s"},
                                        {TT::Assign, ":="},
                                        {TT::Identifier, "'"},
                                        {TT::Identifier, "a"},
                                        {TT::Identifier, "'"},
                                        {TT::Semicolon, ";"},
                                        {TT::End, "end"},
                                        {TT::Dot, "."},
                                        {TT::EndOfFile, ""}};

  AST expected_ast{};
  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<pascal::VarDecl> varDecls;
  varDecls.emplace_back(std::vector<std::string>{"s"},
                        std::make_unique<pascal::SimpleTypeSpec>(
                            pascal::BasicType::String, "string"));
  decls.emplace_back(std::make_unique<pascal::VarSection>(varDecls));

  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  auto thenAssign = std::make_unique<pascal::AssignStmt>(
      std::make_unique<pascal::VariableExpr>("s"),
      std::make_unique<pascal::LiteralExpr>("'a'"));
  auto cmp = std::make_unique<pascal::BinaryExpr>(
      std::make_unique<pascal::VariableExpr>("s"), "=",
      std::make_unique<pascal::LiteralExpr>("''"));
  stmts.emplace_back(
      std::make_unique<pascal::IfStmt>(std::move(cmp), std::move(thenAssign)));

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
