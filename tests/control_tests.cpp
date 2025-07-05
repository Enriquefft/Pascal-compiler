#include "test_common.hpp"

TEST(ControlTests, IfStmt) {
  std::string input_str = "program test; begin if a > 0 then b := 1; end.";
  std::vector<Token> expected_tokens = {
      {TT::Program, "program"}, {TT::Identifier, "test"}, {TT::Semicolon, ";"},
      {TT::Begin, "begin"},     {TT::If, "if"},           {TT::Identifier, "a"},
      {TT::Greater, ">"},       {TT::Number, "0"},        {TT::Then, "then"},
      {TT::Identifier, "b"},    {TT::Assign, ":="},       {TT::Number, "1"},
      {TT::Semicolon, ";"},     {TT::End, "end"},         {TT::Dot, "."},
      {TT::EndOfFile, ""}};
  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  stmts.emplace_back(std::make_unique<pascal::IfStmt>(
      std::make_unique<pascal::BinaryExpr>(
          std::make_unique<pascal::VariableExpr>("a"), ">",
          std::make_unique<pascal::LiteralExpr>("0")),
      std::make_unique<pascal::AssignStmt>(
          std::make_unique<pascal::VariableExpr>("b"),
          std::make_unique<pascal::LiteralExpr>("1"))));

  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  std::string expected_asm = "section .bss\n"
                             "a:    resq    1\n"
                             "b:    resq    1\n\n"
                             "section .text\n"
                             "global main\n"
                             "main:\n"
                             "    mov    rax, [a]\n"
                             "    cmp    rax, 0\n"
                             "    jle    L1\n"
                             "    mov    qword [b], 1\n"
                             "L1:\n"
                             "    ret\n";
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}

TEST(ControlTests, IfElse) {
  std::string input_str =
      "program test; begin if a > 0 then b := 1 else b := 2; end.";
  std::vector<Token> expected_tokens = {
      {TT::Program, "program"}, {TT::Identifier, "test"}, {TT::Semicolon, ";"},
      {TT::Begin, "begin"},     {TT::If, "if"},           {TT::Identifier, "a"},
      {TT::Greater, ">"},       {TT::Number, "0"},        {TT::Then, "then"},
      {TT::Identifier, "b"},    {TT::Assign, ":="},       {TT::Number, "1"},
      {TT::Else, "else"},       {TT::Identifier, "b"},    {TT::Assign, ":="},
      {TT::Number, "2"},        {TT::Semicolon, ";"},     {TT::End, "end"},
      {TT::Dot, "."},           {TT::EndOfFile, ""}};
  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  stmts.emplace_back(std::make_unique<pascal::IfStmt>(
      std::make_unique<pascal::BinaryExpr>(
          std::make_unique<pascal::VariableExpr>("a"), ">",
          std::make_unique<pascal::LiteralExpr>("0")),
      std::make_unique<pascal::AssignStmt>(
          std::make_unique<pascal::VariableExpr>("b"),
          std::make_unique<pascal::LiteralExpr>("1")),
      std::make_unique<pascal::AssignStmt>(
          std::make_unique<pascal::VariableExpr>("b"),
          std::make_unique<pascal::LiteralExpr>("2"))));

  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  std::string expected_asm = "section .bss\n"
                             "a:    resq    1\n"
                             "b:    resq    1\n\n"
                             "section .text\n"
                             "global main\n"
                             "main:\n"
                             "    mov    rax, [a]\n"
                             "    cmp    rax, 0\n"
                             "    jle    L1\n"
                             "    mov    qword [b], 1\n"
                             "    jmp    L2\n"
                             "L1:\n"
                             "    mov    qword [b], 2\n"
                             "L2:\n"
                             "    ret\n";
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}

TEST(ControlTests, CaseStmt) {
  std::string input_str = "program test; begin case a of 1: b := 1; end; end.";
  std::vector<Token> expected_tokens = {
      {TT::Program, "program"}, {TT::Identifier, "test"}, {TT::Semicolon, ";"},
      {TT::Begin, "begin"},     {TT::Case, "case"},       {TT::Identifier, "a"},
      {TT::Of, "of"},           {TT::Number, "1"},        {TT::Colon, ":"},
      {TT::Identifier, "b"},    {TT::Assign, ":="},       {TT::Number, "1"},
      {TT::Semicolon, ";"},     {TT::End, "end"},         {TT::Semicolon, ";"},
      {TT::End, "end"},         {TT::Dot, "."},           {TT::EndOfFile, ""}};

  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;

  // build the constants vector by moving
  std::vector<std::unique_ptr<pascal::Expression>> constants;
  constants.emplace_back(std::make_unique<pascal::LiteralExpr>("1"));

  // build the CaseLabel
  auto label = std::make_unique<pascal::CaseLabel>(
      std::move(constants), std::make_unique<pascal::AssignStmt>(
                                std::make_unique<pascal::VariableExpr>("b"),
                                std::make_unique<pascal::LiteralExpr>("1")));

  // collect labels
  std::vector<std::unique_ptr<pascal::CaseLabel>> labels;
  labels.emplace_back(std::move(label));

  // now build the CaseStmt
  stmts.emplace_back(std::make_unique<pascal::CaseStmt>(
      std::make_unique<pascal::VariableExpr>("a"), std::move(labels)));

  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  std::string expected_asm = "section .bss\n"
                             "a:    resq    1\n"
                             "b:    resq    1\n\n"
                             "section .text\n"
                             "global main\n"
                             "main:\n"
                             "    mov    rax, [a]\n"
                             "    cmp    rax, 1\n"
                             "    jne    L1\n"
                             "    mov    qword [b], 1\n"
                             "L1:\n"
                             "    ret\n";
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}

TEST(ControlTests, WhileStmt) {
  std::string input_str = "program test; begin while a > 0 do a := a - 1; end.";
  std::vector<Token> expected_tokens = {
      {TT::Program, "program"}, {TT::Identifier, "test"}, {TT::Semicolon, ";"},
      {TT::Begin, "begin"},     {TT::While, "while"},     {TT::Identifier, "a"},
      {TT::Greater, ">"},       {TT::Number, "0"},        {TT::Do, "do"},
      {TT::Identifier, "a"},    {TT::Assign, ":="},       {TT::Identifier, "a"},
      {TT::Minus, "-"},         {TT::Number, "1"},        {TT::Semicolon, ";"},
      {TT::End, "end"},         {TT::Dot, "."},           {TT::EndOfFile, ""}};
  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  stmts.emplace_back(std::make_unique<pascal::WhileStmt>(
      std::make_unique<pascal::BinaryExpr>(
          std::make_unique<pascal::VariableExpr>("a"), ">",
          std::make_unique<pascal::LiteralExpr>("0")),
      std::make_unique<pascal::AssignStmt>(
          std::make_unique<pascal::VariableExpr>("a"),
          std::make_unique<pascal::BinaryExpr>(
              std::make_unique<pascal::VariableExpr>("a"), "-",
              std::make_unique<pascal::LiteralExpr>("1")))));

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
                             "L1:\n"
                             "    mov    rax, [a]\n"
                             "    cmp    rax, 0\n"
                             "    jle    L2\n"
                             "    mov    rax, [a]\n"
                             "    sub    rax, 1\n"
                             "    mov    [a], rax\n"
                             "    jmp    L1\n"
                             "L2:\n"
                             "    ret\n";
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}

TEST(ControlTests, ForStmt) {
  std::string input_str = "program test; begin for i:=1 to 10 do a:=i; end.";
  std::vector<Token> expected_tokens = {
      {TT::Program, "program"}, {TT::Identifier, "test"}, {TT::Semicolon, ";"},
      {TT::Begin, "begin"},     {TT::For, "for"},         {TT::Identifier, "i"},
      {TT::Assign, ":="},       {TT::Number, "1"},        {TT::To, "to"},
      {TT::Number, "10"},       {TT::Do, "do"},           {TT::Identifier, "a"},
      {TT::Assign, ":="},       {TT::Identifier, "i"},    {TT::Semicolon, ";"},
      {TT::End, "end"},         {TT::Dot, "."},           {TT::EndOfFile, ""}};
  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  stmts.emplace_back(std::make_unique<pascal::ForStmt>(
      std::make_unique<pascal::AssignStmt>(
          std::make_unique<pascal::VariableExpr>("i"),
          std::make_unique<pascal::LiteralExpr>("1")),
      false, std::make_unique<pascal::LiteralExpr>("10"),
      std::make_unique<pascal::AssignStmt>(
          std::make_unique<pascal::VariableExpr>("a"),
          std::make_unique<pascal::VariableExpr>("i"))));

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
                             "    cmp    rax, 10\n"
                             "    jg     L2\n"
                             "    mov    [a], rax\n"
                             "    add    qword [i], 1\n"
                             "    jmp    L1\n"
                             "L2:\n"
                             "    ret\n";
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}

TEST(ControlTests, RepeatStmt) {
  std::string input_str = "program test; begin repeat a:=a-1 until a=0; end.";
  std::vector<Token> expected_tokens = {
      {TT::Program, "program"}, {TT::Identifier, "test"}, {TT::Semicolon, ";"},
      {TT::Begin, "begin"},     {TT::Repeat, "repeat"},   {TT::Identifier, "a"},
      {TT::Assign, ":="},       {TT::Identifier, "a"},    {TT::Minus, "-"},
      {TT::Number, "1"},        {TT::Until, "until"},     {TT::Identifier, "a"},
      {TT::Equal, "="},         {TT::Number, "0"},        {TT::Semicolon, ";"},
      {TT::End, "end"},         {TT::Dot, "."},           {TT::EndOfFile, ""}};
  AST expected_ast{};

  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  std::vector<std::unique_ptr<pascal::Statement>> body;
  body.emplace_back(std::make_unique<pascal::AssignStmt>(
      std::make_unique<pascal::VariableExpr>("a"),
      std::make_unique<pascal::BinaryExpr>(
          std::make_unique<pascal::VariableExpr>("a"), "-",
          std::make_unique<pascal::LiteralExpr>("1"))));
  stmts.emplace_back(std::make_unique<pascal::RepeatStmt>(
      std::move(body), std::make_unique<pascal::BinaryExpr>(
                           std::make_unique<pascal::VariableExpr>("a"), "=",
                           std::make_unique<pascal::LiteralExpr>("0"))));

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
                             "L1:\n"
                             "    mov    rax, [a]\n"
                             "    sub    rax, 1\n"
                             "    mov    [a], rax\n"
                             "    cmp    rax, 0\n"
                             "    jne    L1\n"
                             "    ret\n";
  run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
}
