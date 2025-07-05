#include "test_common.hpp"

TEST(StructTests, Rec1) {
  std::string input_str = "program test; type r=record a:integer; end; begin end.";
  std::vector<Token> expected_tokens = {
      {TT::Program, "program"},  {TT::Identifier, "test"},
      {TT::Semicolon, ";"},      {TT::Type, "type"},
      {TT::Identifier, "r"},      {TT::Equal, "="},
      {TT::Record, "record"},    {TT::Identifier, "a"},
      {TT::Colon, ":"},           {TT::Identifier, "integer"},
      {TT::Semicolon, ";"},       {TT::End, "end"},
      {TT::Semicolon, ";"},       {TT::Begin, "begin"},
      {TT::End, "end"},           {TT::Dot, "."},
      {TT::EndOfFile, ""}};
  AST expected_ast{};
  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  {
    std::vector<std::unique_ptr<pascal::VarDecl>> fields;
    fields.emplace_back(std::make_unique<pascal::VarDecl>(
        std::vector<std::string>{"a"},
        std::make_unique<pascal::SimpleTypeSpec>(pascal::BasicType::Integer,
                                                 "integer")));
    std::vector<pascal::TypeDefinition> defs;
    auto rec_spec = std::make_unique<pascal::RecordTypeSpec>(std::move(fields));
    std::unique_ptr<pascal::TypeSpec> spec = std::move(rec_spec);
    defs.emplace_back("r", spec);
    decls.emplace_back(std::make_unique<pascal::TypeDecl>(defs));
  }
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;
  std::string expected_asm = "section .text\n"
                             "global main\n"
                             "main:\n"
                             "    ret\n";
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}

TEST(StructTests, Rec2) {
  std::string input_str = "program test; var v:r; begin end.";
  std::vector<Token> expected_tokens = {
      {TT::Program, "program"}, {TT::Identifier, "test"},
      {TT::Semicolon, ";"},    {TT::Var, "var"},
      {TT::Identifier, "v"},   {TT::Colon, ":"},
      {TT::Identifier, "r"},   {TT::Semicolon, ";"},
      {TT::Begin, "begin"},   {TT::End, "end"},
      {TT::Dot, "."},         {TT::EndOfFile, ""}};
  AST expected_ast{};
  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<pascal::VarDecl> varDecls;
  varDecls.emplace_back(std::vector<std::string>{"v"},
                        std::make_unique<pascal::SimpleTypeSpec>(
                            pascal::BasicType::Integer, "r"));
  decls.emplace_back(std::make_unique<pascal::VarSection>(varDecls));
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;
  std::string expected_asm = "section .bss\n"
                             "v:    resq    1\n\n"
                             "section .text\n"
                             "global main\n"
                             "main:\n"
                             "    ret\n";
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}

TEST(StructTests, Rec3) {
  std::string input_str = "program test; begin v.a:=1; end.";
  std::vector<Token> expected_tokens = {
      {TT::Program, "program"}, {TT::Identifier, "test"},
      {TT::Semicolon, ";"},    {TT::Begin, "begin"},
      {TT::Identifier, "v"},   {TT::Dot, "."},
      {TT::Identifier, "a"},   {TT::Assign, ":="},
      {TT::Number, "1"},       {TT::Semicolon, ";"},
      {TT::End, "end"},        {TT::Dot, "."},
      {TT::EndOfFile, ""}};
  AST expected_ast{};
  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;

  // build selector vector by emplacing, avoid copying Selector
  std::vector<pascal::VariableExpr::Selector> sels;
  sels.emplace_back("a", pascal::VariableExpr::Selector::Kind::Field);

  stmts.emplace_back(std::make_unique<pascal::AssignStmt>(
      std::make_unique<pascal::VariableExpr>("v", std::move(sels)),
      std::make_unique<pascal::LiteralExpr>("1")));

  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  std::string expected_asm = "section .bss\n"
                             "v:    resq    1\n\n"
                             "section .text\n"
                             "global main\n"
                             "main:\n"
                             "    lea    rax, [v]\n"
                             "    lea    rax, [rax + 0]\n"
                             "    mov    rbx, rax\n"
                             "    mov    rax, 1\n"
                             "    mov    [rbx], rax\n"
                             "    ret\n";
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}

TEST(StructTests, Rec4) {
  std::string input_str = "program test; begin with v do a:=2; end.";
  std::vector<Token> expected_tokens = {
      {TT::Program, "program"}, {TT::Identifier, "test"},
      {TT::Semicolon, ";"},    {TT::Begin, "begin"},
      {TT::With, "with"},      {TT::Identifier, "v"},
      {TT::Do, "do"},          {TT::Identifier, "a"},
      {TT::Assign, ":="},      {TT::Number, "2"},
      {TT::Semicolon, ";"},    {TT::End, "end"},
      {TT::Dot, "."},          {TT::EndOfFile, ""}};
  AST expected_ast{};
  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  stmts.emplace_back(std::make_unique<pascal::WithStmt>(
      std::make_unique<pascal::VariableExpr>("v"),
      std::make_unique<pascal::AssignStmt>(
          std::make_unique<pascal::VariableExpr>("a"),
          std::make_unique<pascal::LiteralExpr>("2"))));
  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;
  std::string expected_asm = "section .bss\n"
                             "v:    resq    1\n\n"
                             "section .text\n"
                             "global main\n"
                             "main:\n"
                             "    mov    qword [v], 2\n"
                             "    ret\n";
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}

TEST(StructTests, Rec5) {
  std::string input_str = "program test; begin if v.a=0 then v.a:=1; end.";
  std::vector<Token> expected_tokens = {
      {TT::Program, "program"}, {TT::Identifier, "test"},
      {TT::Semicolon, ";"},    {TT::Begin, "begin"},
      {TT::If, "if"},          {TT::Identifier, "v"},
      {TT::Dot, "."},          {TT::Identifier, "a"},
      {TT::Equal, "="},        {TT::Number, "0"},
      {TT::Then, "then"},      {TT::Identifier, "v"},
      {TT::Dot, "."},          {TT::Identifier, "a"},
      {TT::Assign, ":="},      {TT::Number, "1"},
      {TT::Semicolon, ";"},    {TT::End, "end"},
      {TT::Dot, "."},          {TT::EndOfFile, ""}};
  AST expected_ast{};
  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;

  // build lhs1 selector in place
  std::vector<pascal::VariableExpr::Selector> sels1;
  sels1.emplace_back("a", pascal::VariableExpr::Selector::Kind::Field);
  auto lhs1 = std::make_unique<pascal::VariableExpr>("v", std::move(sels1));
  auto cond = std::make_unique<pascal::BinaryExpr>(
      std::move(lhs1), "=", std::make_unique<pascal::LiteralExpr>("0"));

  // build thenAssign selector in place
  std::vector<pascal::VariableExpr::Selector> sels2;
  sels2.emplace_back("a", pascal::VariableExpr::Selector::Kind::Field);
  auto lhs2 = std::make_unique<pascal::VariableExpr>("v", std::move(sels2));
  auto thenAssign = std::make_unique<pascal::AssignStmt>(
      std::move(lhs2), std::make_unique<pascal::LiteralExpr>("1"));

  stmts.emplace_back(
      std::make_unique<pascal::IfStmt>(std::move(cond), std::move(thenAssign)));

  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  std::string expected_asm = "section .bss\n"
                             "v:    resq    1\n\n"
                             "section .text\n"
                             "global main\n"
                             "main:\n"
                             "    mov    rax, [v]\n"
                             "    cmp    rax, 0\n"
                             "    jne    L1\n"
                             "    lea    rax, [v]\n"
                             "    lea    rax, [rax + 0]\n"
                             "    mov    rbx, rax\n"
                             "    mov    rax, 1\n"
                             "    mov    [rbx], rax\n"
                             "L1:\n"
                             "    ret\n";
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}
