#include "test_common.hpp"

TEST(ArrayTests, Arr1) {
  std::string input_str = "var a: array[1..5] of integer;";
  std::vector<Token> expected_tokens = {
      {TT::Var, "var"},       {TT::Identifier, "a"},
      {TT::Colon, ":"},       {TT::Array, "array"},
      {TT::LeftBracket, "["}, {TT::Number, "1"},
      {TT::Dot, "."},         {TT::Dot, "."},
      {TT::Number, "5"},      {TT::RightBracket, "]"},
      {TT::Of, "of"},         {TT::Identifier, "integer"},
      {TT::Semicolon, ";"},   {TT::EndOfFile, ""}};

  std::vector<pascal::Range> ranges = {pascal::Range(1, 5)};

  AST expected_ast{};
  std::vector<std::unique_ptr<pascal::Declaration>> decls;

  decls.emplace_back(std::make_unique<pascal::VarDecl>(
      std::vector<std::string>{"a"},
      std::make_unique<pascal::ArrayTypeSpec>(
          std::move(ranges), // move, no copies
          std::make_unique<pascal::SimpleTypeSpec>(pascal::BasicType::Integer,
                                                   "integer"))));

  std::vector<std::unique_ptr<pascal::Statement>> stmts;
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
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}

TEST(ArrayTests, Arr2) {
  std::string input_str = "a[1]:=0;";
  std::vector<Token> expected_tokens = {
      {TT::Identifier, "a"},   {TT::LeftBracket, "["}, {TT::Number, "1"},
      {TT::RightBracket, "]"}, {TT::Colon, ":"},       {TT::Assign, "="},
      {TT::Number, "0"},       {TT::Semicolon, ";"},   {TT::EndOfFile, ""}};
  AST expected_ast{};
  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  pascal::VariableExpr::Selector idxSel(
      std::make_unique<pascal::LiteralExpr>("1"));
  std::vector<pascal::VariableExpr::Selector> sels;
  sels.emplace_back(std::move(idxSel));
  stmts.emplace_back(std::make_unique<pascal::AssignStmt>(
      std::make_unique<pascal::VariableExpr>("a", std::move(sels)),
      std::make_unique<pascal::LiteralExpr>("0")));
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
                             "    mov    qword [a + 0], 0\n"
                             "    ret\n";
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}

TEST(ArrayTests, Arr3) {
  std::string input_str = "for i:=1 to 5 do a[i]:=i;";
  std::vector<Token> expected_tokens = {
      {TT::For, "for"},       {TT::Identifier, "i"}, {TT::Colon, ":"},
      {TT::Assign, "="},      {TT::Number, "1"},     {TT::To, "to"},
      {TT::Number, "5"},      {TT::Do, "do"},        {TT::Identifier, "a"},
      {TT::LeftBracket, "["}, {TT::Identifier, "i"}, {TT::RightBracket, "]"},
      {TT::Colon, ":"},       {TT::Assign, "="},     {TT::Identifier, "i"},
      {TT::Semicolon, ";"},   {TT::EndOfFile, ""}};
  AST expected_ast{};
  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  pascal::VariableExpr::Selector idx1(
      std::make_unique<pascal::LiteralExpr>("1"));
  std::vector<pascal::VariableExpr::Selector> sels1;
  sels1.emplace_back(std::move(idx1));
  auto init = std::make_unique<pascal::AssignStmt>(
      std::make_unique<pascal::VariableExpr>("i"),
      std::make_unique<pascal::LiteralExpr>("1"));
  pascal::VariableExpr::Selector idx2(
      std::make_unique<pascal::VariableExpr>("i"));
  std::vector<pascal::VariableExpr::Selector> sels2;
  sels2.emplace_back(std::move(idx2));
  auto body = std::make_unique<pascal::AssignStmt>(
      std::make_unique<pascal::VariableExpr>("a", std::move(sels2)),
      std::make_unique<pascal::VariableExpr>("i"));
  stmts.emplace_back(std::make_unique<pascal::ForStmt>(
      std::move(init), false, std::make_unique<pascal::LiteralExpr>("5"),
      std::move(body)));
  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;
  std::string expected_asm = "section .bss\n"
                             "i:    resq    1\n"
                             "a:    resq    1\n\n"
                             "section .text\n"
                             "global main\n"
                             "main:\n"
                             "    mov    qword [i], 1\n"
                             "L1:\n"
                             "    mov    rax, [i]\n"
                             "    cmp    rax, 5\n"
                             "    jg     L2\n"
                             "    mov    [a], rax\n"
                             "    add    qword [i], 1\n"
                             "    jmp    L1\n"
                             "L2:\n"
                             "    ret\n";
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}

TEST(ArrayTests, Arr4) {
  std::string input_str = "writeln(a[1]);";
  std::vector<Token> expected_tokens = {
      {TT::Identifier, "writeln"}, {TT::LeftParen, "("},
      {TT::Identifier, "a"},       {TT::LeftBracket, "["},
      {TT::Number, "1"},           {TT::RightBracket, "]"},
      {TT::RightParen, ")"},       {TT::Semicolon, ";"},
      {TT::EndOfFile, ""}};
  AST expected_ast{};
  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  pascal::VariableExpr::Selector idx(
      std::make_unique<pascal::LiteralExpr>("1"));
  std::vector<pascal::VariableExpr::Selector> sels;
  sels.emplace_back(std::move(idx));
  std::vector<std::unique_ptr<pascal::Expression>> args;
  args.emplace_back(
      std::make_unique<pascal::VariableExpr>("a", std::move(sels)));
  stmts.emplace_back(
      std::make_unique<pascal::ProcCall>("writeln", std::move(args)));
  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;
  std::string expected_asm = "section .bss\n"
                             "a:    resq    1\n\n"
                             "section .text\n"
                             "extern puts\n"
                             "global main\n"
                             "main:\n"
                             "    mov    rdi, [a]\n"
                             "    call   puts\n"
                             "    ret\n";
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}

TEST(ArrayTests, Arr5) {
  std::string input_str = "if a[1]=0 then a[1]:=1;";
  std::vector<Token> expected_tokens = {
      {TT::If, "if"},         {TT::Identifier, "a"},   {TT::LeftBracket, "["},
      {TT::Number, "1"},      {TT::RightBracket, "]"}, {TT::Equal, "="},
      {TT::Number, "0"},      {TT::Then, "then"},      {TT::Identifier, "a"},
      {TT::LeftBracket, "["}, {TT::Number, "1"},       {TT::RightBracket, "]"},
      {TT::Colon, ":"},       {TT::Assign, "="},       {TT::Number, "1"},
      {TT::Semicolon, ";"},   {TT::EndOfFile, ""}};
  AST expected_ast{};
  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  pascal::VariableExpr::Selector s1(std::make_unique<pascal::LiteralExpr>("1"));
  std::vector<pascal::VariableExpr::Selector> sels1;
  sels1.emplace_back(std::move(s1));
  auto lhs1 = std::make_unique<pascal::VariableExpr>("a", std::move(sels1));
  auto cond = std::make_unique<pascal::BinaryExpr>(
      std::move(lhs1), "=", std::make_unique<pascal::LiteralExpr>("0"));
  pascal::VariableExpr::Selector s2(std::make_unique<pascal::LiteralExpr>("1"));
  std::vector<pascal::VariableExpr::Selector> sels2;
  sels2.emplace_back(std::move(s2));
  auto lhs2 = std::make_unique<pascal::VariableExpr>("a", std::move(sels2));
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
                             "a:    resq    1\n\n"
                             "section .text\n"
                             "global main\n"
                             "main:\n"
                             "    mov    rax, [a]\n"
                             "    cmp    rax, 0\n"
                             "    jne    L1\n"
                             "    mov    qword [a + 0], 1\n"
                             "L1:\n"
                             "    ret\n";
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}


