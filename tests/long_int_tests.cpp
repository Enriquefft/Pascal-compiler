#include "test_common.hpp"

TEST(LongIntTests, Long1) {
  std::vector<Token> expected_tokens = {{TT::Program, "program"},
                                        {TT::Identifier, "test"},
                                        {TT::Semicolon, ";"},
                                        {TT::Var, "var"},
                                        {TT::Identifier, "l"},
                                        {TT::Colon, ":"},
                                        {TT::Identifier, "longint"},
                                        {TT::Semicolon, ";"},
                                        {TT::Begin, "begin"},
                                        {TT::End, "end"},
                                        {TT::Dot, "."},
                                        {TT::EndOfFile, ""}};
  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<pascal::VarDecl> varDecls;
  varDecls.emplace_back(std::vector<std::string>{"l"},
                        std::make_unique<pascal::SimpleTypeSpec>(
                            pascal::BasicType::LongInt, "longint"));
  decls.emplace_back(std::make_unique<pascal::VarSection>(varDecls));
  std::vector<std::unique_ptr<pascal::Statement>> stmts;

  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  std::string expected_asm = "section .bss\n"
                             "l:    resq    1\n\n"
                             "section .text\n"
                             "global main\n"
                             "main:\n"
                             "    ret\n";
  std::string expected_output = "";
  run_full("program test; var l: longint; begin end.", expected_tokens,
           expected_ast, expected_asm, expected_output);
}

TEST(LongIntTests, Long2) {
  std::vector<Token> expected_tokens = {
      {TT::Program, "program"}, {TT::Identifier, "test"}, {TT::Semicolon, ";"},
      {TT::Begin, "begin"},     {TT::Identifier, "l"},    {TT::Assign, ":="},
      {TT::Number, "1"},        {TT::Semicolon, ";"},     {TT::End, "end"},
      {TT::Dot, "."},           {TT::EndOfFile, ""}};
  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  stmts.emplace_back(std::make_unique<pascal::AssignStmt>(
      std::make_unique<pascal::VariableExpr>("l"),
      std::make_unique<pascal::LiteralExpr>("1")));

  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  std::string expected_asm = "section .bss\n"
                             "l:    resq    1\n\n"
                             "section .text\n"
                             "global main\n"
                             "main:\n"
                             "    mov    qword [l], 1\n"
                             "    ret\n";
  std::string expected_output = "";
  run_full("program test; begin l:=1; end.", expected_tokens, expected_ast,
           expected_asm, expected_output);
}

TEST(LongIntTests, Long3) {
  std::vector<Token> expected_tokens = {
      {TT::Program, "program"}, {TT::Identifier, "test"}, {TT::Semicolon, ";"},
      {TT::Begin, "begin"},     {TT::While, "while"},     {TT::Identifier, "l"},
      {TT::Greater, ">"},       {TT::Number, "0"},        {TT::Do, "do"},
      {TT::Identifier, "l"},    {TT::Assign, ":="},       {TT::Identifier, "l"},
      {TT::Minus, "-"},         {TT::Number, "1"},        {TT::Semicolon, ";"},
      {TT::End, "end"},         {TT::Dot, "."},           {TT::EndOfFile, ""}};
  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  stmts.emplace_back(std::make_unique<pascal::WhileStmt>(
      std::make_unique<pascal::BinaryExpr>(
          std::make_unique<pascal::VariableExpr>("l"), ">",
          std::make_unique<pascal::LiteralExpr>("0")),
      std::make_unique<pascal::AssignStmt>(
          std::make_unique<pascal::VariableExpr>("l"),
          std::make_unique<pascal::BinaryExpr>(
              std::make_unique<pascal::VariableExpr>("l"), "-",
              std::make_unique<pascal::LiteralExpr>("1")))));

  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  std::string expected_asm = "section .bss\n"
                             "l:    resq    1\n\n"
                             "section .text\n"
                             "global main\n"
                             "main:\n"
                             "L1:\n"
                             "    mov    rax, [l]\n"
                             "    cmp    rax, 0\n"
                             "    jle    L2\n"
                             "    mov    rax, [l]\n"
                             "    sub    rax, 1\n"
                             "    mov    [l], rax\n"
                             "    jmp    L1\n"
                             "L2:\n"
                             "    ret\n";
  std::string expected_output = "";
  run_full("program test; begin while l>0 do l:=l-1; end.", expected_tokens,
           expected_ast, expected_asm, expected_output);
}

TEST(LongIntTests, Long4) {
  std::vector<Token> expected_tokens = {{TT::Program, "program"},
                                        {TT::Identifier, "test"},
                                        {TT::Semicolon, ";"},
                                        {TT::Function, "function"},
                                        {TT::Identifier, "f"},
                                        {TT::Colon, ":"},
                                        {TT::Identifier, "longint"},
                                        {TT::Semicolon, ";"},
                                        {TT::Begin, "begin"},
                                        {TT::Identifier, "f"},
                                        {TT::Assign, ":="},
                                        {TT::Number, "0"},
                                        {TT::Semicolon, ";"},
                                        {TT::End, "end"},
                                        {TT::Semicolon, ";"},
                                        {TT::Begin, "begin"},
                                        {TT::End, "end"},
                                        {TT::Dot, "."},
                                        {TT::EndOfFile, ""}};
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
      std::make_unique<pascal::SimpleTypeSpec>(pascal::BasicType::LongInt,
                                               "longint"),
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
  std::string expected_output = "";
  run_full("program test; function f: longint; begin f:=0; end; begin end.",
           expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(LongIntTests, Long5) {
  std::vector<Token> expected_tokens = {
      {TT::Program, "program"}, {TT::Identifier, "test"}, {TT::Semicolon, ";"},
      {TT::Begin, "begin"},     {TT::For, "for"},         {TT::Identifier, "l"},
      {TT::Assign, ":="},       {TT::Number, "1"},        {TT::To, "to"},
      {TT::Number, "5"},        {TT::Do, "do"},           {TT::Identifier, "l"},
      {TT::Assign, ":="},       {TT::Identifier, "l"},    {TT::Semicolon, ";"},
      {TT::End, "end"},         {TT::Dot, "."},           {TT::EndOfFile, ""}};
  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  stmts.emplace_back(std::make_unique<pascal::ForStmt>(
      std::make_unique<pascal::AssignStmt>(
          std::make_unique<pascal::VariableExpr>("l"),
          std::make_unique<pascal::LiteralExpr>("1")),
      false, std::make_unique<pascal::LiteralExpr>("5"),
      std::make_unique<pascal::AssignStmt>(
          std::make_unique<pascal::VariableExpr>("l"),
          std::make_unique<pascal::VariableExpr>("l"))));

  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  std::string expected_asm = "section .bss\n"
                             "l:    resq    1\n\n"
                             "section .text\n"
                             "global main\n"
                             "main:\n"
                             "    mov    qword [l], 1\n"
                             "L1:\n"
                             "    mov    rax, [l]\n"
                             "    cmp    rax, 5\n"
                             "    jg     L2\n"
                             "    mov    [l], rax\n"
                             "    add    qword [l], 1\n"
                             "    jmp    L1\n"
                             "L2:\n"
                             "    ret\n";
  std::string expected_output = "";
  run_full("program test; begin for l:=1 to 5 do l:=l; end.", expected_tokens,
           expected_ast, expected_asm, expected_output);
}
