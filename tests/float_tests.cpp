#include "test_common.hpp"

TEST(FloatTests, Float1) {
  std::string input_str = "program test; var x: real; begin x:=1.0; end.";
  std::vector<Token> expected_tokens = {
      {TT::Program, "program"}, {TT::Identifier, "test"}, {TT::Semicolon, ";"},
      {TT::Var, "var"},         {TT::Identifier, "x"},    {TT::Colon, ":"},
      {TT::Identifier, "real"}, {TT::Semicolon, ";"},     {TT::Begin, "begin"},
      {TT::Identifier, "x"},    {TT::Assign, ":="},       {TT::Number, "1"},
      {TT::Dot, "."},           {TT::Number, "0"},        {TT::Semicolon, ";"},
      {TT::End, "end"},         {TT::Dot, "."},           {TT::EndOfFile, ""}};
  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  decls.emplace_back(std::make_unique<pascal::VarDecl>(
      std::vector<std::string>{"x"}, std::make_unique<pascal::SimpleTypeSpec>(
                                         pascal::BasicType::Real, "real")));
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  stmts.emplace_back(std::make_unique<pascal::AssignStmt>(
      std::make_unique<pascal::VariableExpr>("x"),
      std::make_unique<pascal::LiteralExpr>("1.0")));
  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  std::string expected_asm = "section .bss\n"
                             "x:    resq    1\n\n"
                             "section .text\n"
                             "global main\n"
                             "main:\n"
                             "    mov    qword [x], 0x3FF0000000000000\n"
                             "    ret\n";
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}

TEST(FloatTests, Float2) {
  std::string input_str = "program test; begin x:=1.5+2.5; end.";
  std::vector<Token> expected_tokens = {
      {TT::Program, "program"}, {TT::Identifier, "test"}, {TT::Semicolon, ";"},
      {TT::Begin, "begin"},     {TT::Identifier, "x"},    {TT::Assign, ":="},
      {TT::Number, "1"},        {TT::Dot, "."},           {TT::Number, "5"},
      {TT::Plus, "+"},          {TT::Number, "2"},        {TT::Dot, "."},
      {TT::Number, "5"},        {TT::Semicolon, ";"},     {TT::End, "end"},
      {TT::Dot, "."},           {TT::EndOfFile, ""}};
  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  stmts.emplace_back(std::make_unique<pascal::AssignStmt>(
      std::make_unique<pascal::VariableExpr>("x"),
      std::make_unique<pascal::BinaryExpr>(
          std::make_unique<pascal::LiteralExpr>("1.5"), "+",
          std::make_unique<pascal::LiteralExpr>("2.5"))));
  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  std::string expected_asm = "section .bss\n"
                             "x:    resq    1\n\n"
                             "section .text\n"
                             "global main\n"
                             "main:\n"
                             "    mov    qword [x], 0x4010000000000000\n"
                             "    ret\n";
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}

TEST(FloatTests, Float3) {
  std::string input_str = "program test; begin if 0.0 < 1.0 then x:=0.0; end.";
  std::vector<Token> expected_tokens = {
      {TT::Program, "program"}, {TT::Identifier, "test"}, {TT::Semicolon, ";"},
      {TT::Begin, "begin"},     {TT::If, "if"},           {TT::Number, "0"},
      {TT::Dot, "."},           {TT::Number, "0"},        {TT::Less, "<"},
      {TT::Number, "1"},        {TT::Dot, "."},           {TT::Number, "0"},
      {TT::Then, "then"},       {TT::Identifier, "x"},    {TT::Assign, ":="},
      {TT::Number, "0"},        {TT::Dot, "."},           {TT::Number, "0"},
      {TT::Semicolon, ";"},     {TT::End, "end"},         {TT::Dot, "."},
      {TT::EndOfFile, ""}};
  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  stmts.emplace_back(std::make_unique<pascal::IfStmt>(
      std::make_unique<pascal::BinaryExpr>(
          std::make_unique<pascal::LiteralExpr>("0.0"), "<",
          std::make_unique<pascal::LiteralExpr>("1.0")),
      std::make_unique<pascal::AssignStmt>(
          std::make_unique<pascal::VariableExpr>("x"),
          std::make_unique<pascal::LiteralExpr>("0.0"))));
  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  std::string expected_asm = "section .bss\n"
                             "x:    resq    1\n\n"
                             "section .text\n"
                             "global main\n"
                             "main:\n"
                             "    mov    qword [x], 0x0000000000000000\n"
                             "    ret\n";
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}

TEST(FloatTests, Float4) {
  std::string input_str = "program test; begin while x < 1.0 do x:=x+0.1; end.";
  std::vector<Token> expected_tokens = {
      {TT::Program, "program"}, {TT::Identifier, "test"}, {TT::Semicolon, ";"},
      {TT::Begin, "begin"},     {TT::While, "while"},     {TT::Identifier, "x"},
      {TT::Less, "<"},          {TT::Number, "1"},        {TT::Dot, "."},
      {TT::Number, "0"},        {TT::Do, "do"},           {TT::Identifier, "x"},
      {TT::Assign, ":="},       {TT::Identifier, "x"},    {TT::Plus, "+"},
      {TT::Number, "0"},        {TT::Dot, "."},           {TT::Number, "1"},
      {TT::Semicolon, ";"},     {TT::End, "end"},         {TT::Dot, "."},
      {TT::EndOfFile, ""}};
  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  stmts.emplace_back(std::make_unique<pascal::WhileStmt>(
      std::make_unique<pascal::BinaryExpr>(
          std::make_unique<pascal::VariableExpr>("x"), "<",
          std::make_unique<pascal::LiteralExpr>("1.0")),
      std::make_unique<pascal::AssignStmt>(
          std::make_unique<pascal::VariableExpr>("x"),
          std::make_unique<pascal::BinaryExpr>(
              std::make_unique<pascal::VariableExpr>("x"), "+",
              std::make_unique<pascal::LiteralExpr>("0.1")))));
  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  std::string expected_asm = "section .bss\n"
                             "x:    resq    1\n\n"
                             "section .text\n"
                             "global main\n"
                             "main:\n"
                             "L1:\n"
                             "    mov    rax, [x]\n"
                             "    cmp    rax, 0x3FF0000000000000\n"
                             "    jge    L2\n"
                             "    mov    rax, [x]\n"
                             "    add    rax, 0x3FB999999999999A\n"
                             "    mov    [x], rax\n"
                             "    jmp    L1\n"
                             "L2:\n"
                             "    ret\n";
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}

TEST(FloatTests, Float5) {
  std::string input_str =
      "program test; function f: real; begin f:=0.0; end; begin end.";
  std::vector<Token> expected_tokens = {
      {TT::Program, "program"}, {TT::Identifier, "test"},
      {TT::Semicolon, ";"},     {TT::Function, "function"},
      {TT::Identifier, "f"},    {TT::Colon, ":"},
      {TT::Identifier, "real"}, {TT::Semicolon, ";"},
      {TT::Begin, "begin"},     {TT::Identifier, "f"},
      {TT::Assign, ":="},       {TT::Number, "0"},
      {TT::Dot, "."},           {TT::Number, "0"},
      {TT::Semicolon, ";"},     {TT::End, "end"},
      {TT::Semicolon, ";"},     {TT::Begin, "begin"},
      {TT::End, "end"},         {TT::Dot, "."},
      {TT::EndOfFile, ""}};
  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::ParamDecl>> params;
  std::vector<std::unique_ptr<pascal::Statement>> fn_stmts;
  fn_stmts.emplace_back(std::make_unique<pascal::AssignStmt>(
      std::make_unique<pascal::VariableExpr>("f"),
      std::make_unique<pascal::LiteralExpr>("0.0")));
  auto fn_block = std::make_unique<pascal::Block>(
      std::vector<std::unique_ptr<pascal::Declaration>>{}, std::move(fn_stmts));
  decls.emplace_back(std::make_unique<pascal::FunctionDecl>(
      "f", std::move(params),
      std::make_unique<pascal::SimpleTypeSpec>(pascal::BasicType::Real, "real"),
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
                             "    mov    rax, 0x0000000000000000\n"
                             "    ret\n"
                             "global main\n"
                             "main:\n"
                             "    ret\n";
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}
