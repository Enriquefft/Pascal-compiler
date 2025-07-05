#include "test_common.hpp"

TEST(ArrayTests, Arr1) {
  std::string input_str = "program test;\n"
                          "var a: array[1..5] of integer;\n"
                          "begin\n"
                          "end.";
  std::vector<Token> expected_tokens = {{TT::Program, "program"},
                                        {TT::Identifier, "test"},
                                        {TT::Semicolon, ";"},
                                        {TT::Var, "var"},
                                        {TT::Identifier, "a"},
                                        {TT::Colon, ":"},
                                        {TT::Array, "array"},
                                        {TT::LeftBracket, "["},
                                        {TT::Number, "1"},
                                        {TT::Dot, "."},
                                        {TT::Dot, "."},
                                        {TT::Number, "5"},
                                        {TT::RightBracket, "]"},
                                        {TT::Of, "of"},
                                        {TT::Identifier, "integer"},
                                        {TT::Semicolon, ";"},
                                        {TT::Begin, "begin"},
                                        {TT::End, "end"},
                                        {TT::Dot, "."},
                                        {TT::EndOfFile, ""}};

  std::vector<pascal::Range> ranges = {pascal::Range(1, 5)};
  AST expected_ast{};
  std::vector<std::unique_ptr<pascal::Declaration>> decls;

  std::vector<pascal::VarDecl> varDecls;
  varDecls.emplace_back(
      std::vector<std::string>{"a"},
      std::make_unique<pascal::ArrayTypeSpec>(
          std::move(ranges), std::make_unique<pascal::SimpleTypeSpec>(
                                 pascal::BasicType::Integer, "integer")));

  decls.emplace_back(std::make_unique<pascal::VarSection>(varDecls

                                                          ));

  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  std::string expected_asm =
      "section .bss\n"
      "a:    resq    5\n\n" // ← was resq 1, now correctly 5
      "section .text\n"
      "global main\n"
      "main:\n"
      "    ret\n";
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}

TEST(ArrayTests, Arr2) {

  std::string input_str = "program test;\n"
                          "var a: array[1..1] of integer;\n"
                          "begin\n"
                          "  a[1] := 0;\n"
                          "end.";

  std::vector<Token> expected_tokens = {
      {TT::Program, "program"}, {TT::Identifier, "test"},
      {TT::Semicolon, ";"},

      {TT::Var, "var"},         {TT::Identifier, "a"},
      {TT::Colon, ":"},         {TT::Array, "array"},
      {TT::LeftBracket, "["},   {TT::Number, "1"},
      {TT::Dot, "."},           {TT::Dot, "."},
      {TT::Number, "1"},        {TT::RightBracket, "]"},
      {TT::Of, "of"},           {TT::Identifier, "integer"},
      {TT::Semicolon, ";"},

      {TT::Begin, "begin"},     {TT::Identifier, "a"},
      {TT::LeftBracket, "["},   {TT::Number, "1"},
      {TT::RightBracket, "]"},  {TT::Assign, ":="},
      {TT::Number, "0"},        {TT::Semicolon, ";"},
      {TT::End, "end"},         {TT::Dot, "."},
      {TT::EndOfFile, ""}};

  // AST building is unchanged...
  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;

  std::vector<pascal::Range> ranges = {pascal::Range(1, 1)};
  std::vector<pascal::VarDecl> varDecls;
  varDecls.emplace_back(
      std::vector<std::string>{"a"},
      std::make_unique<pascal::ArrayTypeSpec>(
          std::move(ranges), std::make_unique<pascal::SimpleTypeSpec>(
                                 pascal::BasicType::Integer, "integer")));

  decls.emplace_back(std::make_unique<pascal::VarSection>(varDecls

                                                          ));

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
  std::string input_str = "program test;\n"
                          "var a: array[1..1] of integer;\n"
                          "    i: integer;\n"
                          "begin\n"
                          "  for i := 1 to 5 do a[i] := i;\n"
                          "end.";

  std::vector<Token> expected_tokens = {{TT::Program, "program"},
                                        {TT::Identifier, "test"},
                                        {TT::Semicolon, ";"},
                                        {TT::Var, "var"},
                                        {TT::Identifier, "a"},
                                        {TT::Colon, ":"},
                                        {TT::Array, "array"},
                                        {TT::LeftBracket, "["},
                                        {TT::Number, "1"},
                                        {TT::Dot, "."},
                                        {TT::Dot, "."},
                                        {TT::Number, "1"},
                                        {TT::RightBracket, "]"},
                                        {TT::Of, "of"},
                                        {TT::Identifier, "integer"},
                                        {TT::Semicolon, ";"},
                                        {TT::Identifier, "i"},
                                        {TT::Colon, ":"},
                                        {TT::Identifier, "integer"},
                                        {TT::Semicolon, ";"},
                                        {TT::Begin, "begin"},
                                        {TT::For, "for"},
                                        {TT::Identifier, "i"},
                                        {TT::Assign, ":="},
                                        {TT::Number, "1"},
                                        {TT::To, "to"},
                                        {TT::Number, "5"},
                                        {TT::Do, "do"},
                                        {TT::Identifier, "a"},
                                        {TT::LeftBracket, "["},
                                        {TT::Identifier, "i"},
                                        {TT::RightBracket, "]"},
                                        {TT::Assign, ":="},
                                        {TT::Identifier, "i"},
                                        {TT::Semicolon, ";"},
                                        {TT::End, "end"},
                                        {TT::Dot, "."},
                                        {TT::EndOfFile, ""}};

  // AST:
  AST expected_ast{};
  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  // var a: array[1..1] of integer; var i: integer;
  std::vector<pascal::Range> ranges = {pascal::Range(1, 1)};
  std::vector<pascal::VarDecl> varDecls;
  varDecls.emplace_back(
      std::vector<std::string>{"a"},
      std::make_unique<pascal::ArrayTypeSpec>(
          std::move(ranges), std::make_unique<pascal::SimpleTypeSpec>(
                                 pascal::BasicType::Integer, "integer")));
  varDecls.emplace_back(std::vector<std::string>{"i"},
                        std::make_unique<pascal::SimpleTypeSpec>(
                            pascal::BasicType::Integer, "integer"));
  decls.emplace_back(std::make_unique<pascal::VarSection>(varDecls));

  // for i := 1 to 5 do a[i] := i;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
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

  std::string expected_asm =
      "section .bss\n"
      "a:    resq    1\n"
      "i:    resq    1\n\n"
      "section .text\n"
      "global main\n"
      "main:\n"
      "    mov    qword [i], 1\n"
      "L1:\n"
      "    mov    rax, [i]\n"
      "    mov    rbx, 5\n"
      "    cmp    rax, rbx\n"
      "    jg     L2\n"

      "    mov    rax, [i]\n    mov    rcx, [i]\n    sub    rcx, 1\n    imul"
      "rcx, 8\n    mov    qword [a + rcx], rax\n    add    qword [i], 1\n "
      "jmp    L1\nL2:\n    ret\n";

  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}

TEST(ArrayTests, Arr4) {
  std::string input_str = "program test;\n"
                          "var a: array[1..1] of integer;\n"
                          "begin\n"
                          "  writeln(a[1]);\n"
                          "end.";

  std::vector<Token> expected_tokens = {{TT::Program, "program"},
                                        {TT::Identifier, "test"},
                                        {TT::Semicolon, ";"},
                                        {TT::Var, "var"},
                                        {TT::Identifier, "a"},
                                        {TT::Colon, ":"},
                                        {TT::Array, "array"},
                                        {TT::LeftBracket, "["},
                                        {TT::Number, "1"},
                                        {TT::Dot, "."},
                                        {TT::Dot, "."},
                                        {TT::Number, "1"},
                                        {TT::RightBracket, "]"},
                                        {TT::Of, "of"},
                                        {TT::Identifier, "integer"},
                                        {TT::Semicolon, ";"},
                                        {TT::Begin, "begin"},
                                        {TT::Identifier, "writeln"},
                                        {TT::LeftParen, "("},
                                        {TT::Identifier, "a"},
                                        {TT::LeftBracket, "["},
                                        {TT::Number, "1"},
                                        {TT::RightBracket, "]"},
                                        {TT::RightParen, ")"},
                                        {TT::Semicolon, ";"},
                                        {TT::End, "end"},
                                        {TT::Dot, "."},
                                        {TT::EndOfFile, ""}};

  // AST:
  AST expected_ast{};
  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<pascal::Range> ranges = {pascal::Range(1, 1)};
  std::vector<pascal::VarDecl> varDecls;
  varDecls.emplace_back(
      std::vector<std::string>{"a"},
      std::make_unique<pascal::ArrayTypeSpec>(
          std::move(ranges), std::make_unique<pascal::SimpleTypeSpec>(
                                 pascal::BasicType::Integer, "integer")));
  decls.emplace_back(std::make_unique<pascal::VarSection>(varDecls));

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

  std::string expected_asm =
      "section .data\nfmt_int: db \"%d\", 10, 0\n\nsection .bss\na:    resq "
      "1\n\nsection .text\nextern printf\nglobal main\nmain:\n    mov    rdi,"
      " fmt_int\n mov rax, [a + 0]\n mov rsi, rax\n xor rax,"
      " rax\n call printf\n ret\n ";
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "0");
}

TEST(ArrayTests, Arr5) {
  std::string input_str = "program test;\n"
                          "var a: array[1..1] of integer;\n"
                          "begin\n"
                          "  if a[1] = 0 then a[1] := 1;\n"
                          "end.";

  std::vector<Token> expected_tokens = {{TT::Program, "program"},
                                        {TT::Identifier, "test"},
                                        {TT::Semicolon, ";"},
                                        {TT::Var, "var"},
                                        {TT::Identifier, "a"},
                                        {TT::Colon, ":"},
                                        {TT::Array, "array"},
                                        {TT::LeftBracket, "["},
                                        {TT::Number, "1"},
                                        {TT::Dot, "."},
                                        {TT::Dot, "."},
                                        {TT::Number, "1"},
                                        {TT::RightBracket, "]"},
                                        {TT::Of, "of"},
                                        {TT::Identifier, "integer"},
                                        {TT::Semicolon, ";"},
                                        {TT::Begin, "begin"},
                                        {TT::If, "if"},
                                        {TT::Identifier, "a"},
                                        {TT::LeftBracket, "["},
                                        {TT::Number, "1"},
                                        {TT::RightBracket, "]"},
                                        {TT::Equal, "="},
                                        {TT::Number, "0"},
                                        {TT::Then, "then"},
                                        {TT::Identifier, "a"},
                                        {TT::LeftBracket, "["},
                                        {TT::Number, "1"},
                                        {TT::RightBracket, "]"},
                                        {TT::Assign, ":="},
                                        {TT::Number, "1"},
                                        {TT::Semicolon, ";"},
                                        {TT::End, "end"},
                                        {TT::Dot, "."},
                                        {TT::EndOfFile, ""}};

  // AST:
  AST expected_ast{};
  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<pascal::Range> ranges = {pascal::Range(1, 1)};
  std::vector<pascal::VarDecl> varDecls;
  varDecls.emplace_back(
      std::vector<std::string>{"a"},
      std::make_unique<pascal::ArrayTypeSpec>(
          std::move(ranges), std::make_unique<pascal::SimpleTypeSpec>(
                                 pascal::BasicType::Integer, "integer")));
  decls.emplace_back(std::make_unique<pascal::VarSection>(varDecls));

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
