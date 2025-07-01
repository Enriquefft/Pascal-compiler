#include "test_utils.hpp"
#include <gtest/gtest.h>

using pascal::AST;
using pascal::Lexer;
using pascal::Parser;
using pascal::Token;
using std::make_unique;
using std::unique_ptr;
using std::vector;
using test_utils::run_full;
using TT = pascal::TokenType;

// Variable declarations
TEST(VarDeclTests, Decl1) {
  std::string input_str = "var a: integer;";
  vector<Token> expected_tokens = {
      {TT::Var, "var"},     {TT::Identifier, "a"},
      {TT::Colon, ":"},     {TT::Identifier, "integer"},
      {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};

  AST expected_ast{};

  // build declarations vector without initializer_list
  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  decls.emplace_back(std::make_unique<pascal::VarDecl>(
      std::vector<std::string>{"a"},
      std::make_unique<pascal::SimpleTypeSpec>(pascal::BasicType::Integer,
                                               "integer")));

  // empty statements vector
  std::vector<std::unique_ptr<pascal::Statement>> stmts;

  // assemble AST
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
  std::string expected_output = "";
  run_full(input_str, expected_tokens, expected_ast, expected_asm,
           expected_output);
}

// // Variable declarations
// TEST(VarDeclTests, Decl2) {
//   std::string input_str = "var b: real;";
//   std::vector<Token> expected_tokens = {
//       {TT::Var, "var"},         {TT::Identifier, "b"}, {TT::Colon, ":"},
//       {TT::Identifier, "real"}, {TT::Semicolon, ";"},  {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   decls.emplace_back(std::make_unique<pascal::VarDecl>(
//       std::vector<std::string>{"b"},
//       std::make_unique<pascal::SimpleTypeSpec>(
//                                          pascal::BasicType::Real, "real")));
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "b:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    ret\n";
//   std::string expected_output = "";
//
//   run_full(input_str, expected_tokens, expected_ast, expected_asm,
//            expected_output);
// }
//
// TEST(VarDeclTests, Decl3) {
//   std::string input_str = "var c: unsigned;";
//   std::vector<Token> expected_tokens = {
//       {TT::Var, "var"},     {TT::Identifier, "c"},
//       {TT::Colon, ":"},     {TT::Identifier, "unsigned"},
//       {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   decls.emplace_back(std::make_unique<pascal::VarDecl>(
//       std::vector<std::string>{"c"},
//       std::make_unique<pascal::SimpleTypeSpec>(pascal::BasicType::UnsignedInt,
//                                                "unsigned")));
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "c:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    ret\n";
//   std::string expected_output = "";
//
//   run_full(input_str, expected_tokens, expected_ast, expected_asm,
//            expected_output);
// }
//
// // Expressions
// TEST(ExpressionTests, Expr1) {
//   std::string input_str = "begin a := 1; end.";
//   std::vector<Token> expected_tokens = {
//       {TT::Begin, "begin"}, {TT::Identifier, "a"}, {TT::Colon, ":"},
//       {TT::Assign, "="},    {TT::Number, "1"},     {TT::Semicolon, ";"},
//       {TT::End, "end"},     {TT::Dot, "."},        {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//
//   std::vector<std::unique_ptr<pascal::Statement>> inner;
//   inner.emplace_back(std::make_unique<pascal::AssignStmt>(
//       std::make_unique<pascal::VariableExpr>("a"),
//       std::make_unique<pascal::LiteralExpr>("1")));
//   stmts.emplace_back(std::make_unique<pascal::CompoundStmt>(std::move(inner)));
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "a:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    qword [a], 1\n"
//                              "    ret\n";
//   std::string expected_output = "";
//
//   run_full(input_str, expected_tokens, expected_ast, expected_asm,
//            expected_output);
// }
//
// TEST(ExpressionTests, Expr2) {
//   std::string input_str = "begin b := a + 1; end.";
//   std::vector<Token> expected_tokens = {
//       {TT::Begin, "begin"}, {TT::Identifier, "b"}, {TT::Colon, ":"},
//       {TT::Assign, "="},    {TT::Identifier, "a"}, {TT::Plus, "+"},
//       {TT::Number, "1"},    {TT::Semicolon, ";"},  {TT::End, "end"},
//       {TT::Dot, "."},       {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//
//   std::vector<std::unique_ptr<pascal::Statement>> inner;
//   inner.emplace_back(std::make_unique<pascal::AssignStmt>(
//       std::make_unique<pascal::VariableExpr>("b"),
//       std::make_unique<pascal::BinaryExpr>(
//           std::make_unique<pascal::VariableExpr>("a"), "+",
//           std::make_unique<pascal::LiteralExpr>("1"))));
//   stmts.emplace_back(std::make_unique<pascal::CompoundStmt>(std::move(inner)));
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "a:    resq    1\n"
//                              "b:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    rax, [a]\n"
//                              "    add    rax, 1\n"
//                              "    mov    [b], rax\n"
//                              "    ret\n";
//   std::string expected_output = "";
//
//   run_full(input_str, expected_tokens, expected_ast, expected_asm,
//            expected_output);
// }
//
// TEST(ExpressionTests, Expr3) {
//   std::string input_str = "begin c := b * 2; end.";
//   std::vector<Token> expected_tokens = {
//       {TT::Begin, "begin"}, {TT::Identifier, "c"}, {TT::Colon, ":"},
//       {TT::Assign, "="},    {TT::Identifier, "b"}, {TT::Star, "*"},
//       {TT::Number, "2"},    {TT::Semicolon, ";"},  {TT::End, "end"},
//       {TT::Dot, "."},       {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//
//   std::vector<std::unique_ptr<pascal::Statement>> inner;
//   inner.emplace_back(std::make_unique<pascal::AssignStmt>(
//       std::make_unique<pascal::VariableExpr>("c"),
//       std::make_unique<pascal::BinaryExpr>(
//           std::make_unique<pascal::VariableExpr>("b"), "*",
//           std::make_unique<pascal::LiteralExpr>("2"))));
//   stmts.emplace_back(std::make_unique<pascal::CompoundStmt>(std::move(inner)));
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "b:    resq    1\n"
//                              "c:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    rax, [b]\n"
//                              "    imul   rax, 2\n"
//                              "    mov    [c], rax\n"
//                              "    ret\n";
//   std::string expected_output = "";
//
//   run_full(input_str, expected_tokens, expected_ast, expected_asm,
//            expected_output);
// }
//
// // Control statements
// TEST(ControlTests, IfStmt) {
//   std::string input_str = "if a > 0 then b := 1;";
//   std::vector<Token> expected_tokens = {
//       {TT::If, "if"},       {TT::Identifier, "a"}, {TT::Greater, ">"},
//       {TT::Number, "0"},    {TT::Then, "then"},    {TT::Identifier, "b"},
//       {TT::Colon, ":"},     {TT::Assign, "="},     {TT::Number, "1"},
//       {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   stmts.emplace_back(std::make_unique<pascal::IfStmt>(
//       std::make_unique<pascal::BinaryExpr>(
//           std::make_unique<pascal::VariableExpr>("a"), ">",
//           std::make_unique<pascal::LiteralExpr>("0")),
//       std::make_unique<pascal::AssignStmt>(
//           std::make_unique<pascal::VariableExpr>("b"),
//           std::make_unique<pascal::LiteralExpr>("1"))));
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "a:    resq    1\n"
//                              "b:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    rax, [a]\n"
//                              "    cmp    rax, 0\n"
//                              "    jle    L1\n"
//                              "    mov    qword [b], 1\n"
//                              "L1:\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// TEST(ControlTests, IfElse) {
//   std::string input_str = "if a > 0 then b := 1 else b := 2;";
//   std::vector<Token> expected_tokens = {
//       {TT::If, "if"},     {TT::Identifier, "a"}, {TT::Greater, ">"},
//       {TT::Number, "0"},  {TT::Then, "then"},    {TT::Identifier, "b"},
//       {TT::Colon, ":"},   {TT::Assign, "="},     {TT::Number, "1"},
//       {TT::Else, "else"}, {TT::Identifier, "b"}, {TT::Colon, ":"},
//       {TT::Assign, "="},  {TT::Number, "2"},     {TT::Semicolon, ";"},
//       {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   stmts.emplace_back(std::make_unique<pascal::IfStmt>(
//       std::make_unique<pascal::BinaryExpr>(
//           std::make_unique<pascal::VariableExpr>("a"), ">",
//           std::make_unique<pascal::LiteralExpr>("0")),
//       std::make_unique<pascal::AssignStmt>(
//           std::make_unique<pascal::VariableExpr>("b"),
//           std::make_unique<pascal::LiteralExpr>("1")),
//       std::make_unique<pascal::AssignStmt>(
//           std::make_unique<pascal::VariableExpr>("b"),
//           std::make_unique<pascal::LiteralExpr>("2"))));
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "a:    resq    1\n"
//                              "b:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    rax, [a]\n"
//                              "    cmp    rax, 0\n"
//                              "    jle    L1\n"
//                              "    mov    qword [b], 1\n"
//                              "    jmp    L2\n"
//                              "L1:\n"
//                              "    mov    qword [b], 2\n"
//                              "L2:\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// TEST(ControlTests, CaseStmt) {
//   std::string input_str = "case a of 1: b := 1; end;";
//   std::vector<Token> expected_tokens = {
//       {TT::Case, "case"},   {TT::Identifier, "a"}, {TT::Of, "of"},
//       {TT::Number, "1"},    {TT::Colon, ":"},      {TT::Identifier, "b"},
//       {TT::Colon, ":"},     {TT::Assign, "="},     {TT::Number, "1"},
//       {TT::Semicolon, ";"}, {TT::End, "end"},      {TT::Semicolon, ";"},
//       {TT::EndOfFile, ""}};
//
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//
//   // build the constants vector by moving
//   std::vector<std::unique_ptr<pascal::Expression>> constants;
//   constants.emplace_back(std::make_unique<pascal::LiteralExpr>("1"));
//
//   // build the CaseLabel
//   auto label = std::make_unique<pascal::CaseLabel>(
//       std::move(constants), std::make_unique<pascal::AssignStmt>(
//                                 std::make_unique<pascal::VariableExpr>("b"),
//                                 std::make_unique<pascal::LiteralExpr>("1")));
//
//   // collect labels
//   std::vector<std::unique_ptr<pascal::CaseLabel>> labels;
//   labels.emplace_back(std::move(label));
//
//   // now build the CaseStmt
//   stmts.emplace_back(std::make_unique<pascal::CaseStmt>(
//       std::make_unique<pascal::VariableExpr>("a"), std::move(labels)));
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "a:    resq    1\n"
//                              "b:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    rax, [a]\n"
//                              "    cmp    rax, 1\n"
//                              "    jne    L1\n"
//                              "    mov    qword [b], 1\n"
//                              "L1:\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// TEST(ControlTests, WhileStmt) {
//   std::string input_str = "while a > 0 do a := a - 1;";
//   std::vector<Token> expected_tokens = {
//       {TT::While, "while"}, {TT::Identifier, "a"}, {TT::Greater, ">"},
//       {TT::Number, "0"},    {TT::Do, "do"},        {TT::Identifier, "a"},
//       {TT::Colon, ":"},     {TT::Assign, "="},     {TT::Identifier, "a"},
//       {TT::Minus, "-"},     {TT::Number, "1"},     {TT::Semicolon, ";"},
//       {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   stmts.emplace_back(std::make_unique<pascal::WhileStmt>(
//       std::make_unique<pascal::BinaryExpr>(
//           std::make_unique<pascal::VariableExpr>("a"), ">",
//           std::make_unique<pascal::LiteralExpr>("0")),
//       std::make_unique<pascal::AssignStmt>(
//           std::make_unique<pascal::VariableExpr>("a"),
//           std::make_unique<pascal::BinaryExpr>(
//               std::make_unique<pascal::VariableExpr>("a"), "-",
//               std::make_unique<pascal::LiteralExpr>("1")))));
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "a:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "L1:\n"
//                              "    mov    rax, [a]\n"
//                              "    cmp    rax, 0\n"
//                              "    jle    L2\n"
//                              "    mov    rax, [a]\n"
//                              "    sub    rax, 1\n"
//                              "    mov    [a], rax\n"
//                              "    jmp    L1\n"
//                              "L2:\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// TEST(ControlTests, ForStmt) {
//   std::string input_str = "for i:=1 to 10 do a:=i;";
//   std::vector<Token> expected_tokens = {
//       {TT::For, "for"},     {TT::Identifier, "i"}, {TT::Colon, ":"},
//       {TT::Assign, "="},    {TT::Number, "1"},     {TT::To, "to"},
//       {TT::Number, "10"},   {TT::Do, "do"},        {TT::Identifier, "a"},
//       {TT::Colon, ":"},     {TT::Assign, "="},     {TT::Identifier, "i"},
//       {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   stmts.emplace_back(std::make_unique<pascal::ForStmt>(
//       std::make_unique<pascal::AssignStmt>(
//           std::make_unique<pascal::VariableExpr>("i"),
//           std::make_unique<pascal::LiteralExpr>("1")),
//       false, std::make_unique<pascal::LiteralExpr>("10"),
//       std::make_unique<pascal::AssignStmt>(
//           std::make_unique<pascal::VariableExpr>("a"),
//           std::make_unique<pascal::VariableExpr>("i"))));
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "i:    resq    1\n"
//                              "a:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    qword [i], 1\n"
//                              "L1:\n"
//                              "    mov    rax, [i]\n"
//                              "    cmp    rax, 10\n"
//                              "    jg     L2\n"
//                              "    mov    [a], rax\n"
//                              "    add    qword [i], 1\n"
//                              "    jmp    L1\n"
//                              "L2:\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// TEST(ControlTests, RepeatStmt) {
//   std::string input_str = "repeat a:=a-1 until a=0;";
//   std::vector<Token> expected_tokens = {
//       {TT::Repeat, "repeat"}, {TT::Identifier, "a"}, {TT::Colon, ":"},
//       {TT::Assign, "="},      {TT::Identifier, "a"}, {TT::Minus, "-"},
//       {TT::Number, "1"},      {TT::Until, "until"},  {TT::Identifier, "a"},
//       {TT::Equal, "="},       {TT::Number, "0"},     {TT::Semicolon, ";"},
//       {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   std::vector<std::unique_ptr<pascal::Statement>> body;
//   body.emplace_back(std::make_unique<pascal::AssignStmt>(
//       std::make_unique<pascal::VariableExpr>("a"),
//       std::make_unique<pascal::BinaryExpr>(
//           std::make_unique<pascal::VariableExpr>("a"), "-",
//           std::make_unique<pascal::LiteralExpr>("1"))));
//   stmts.emplace_back(std::make_unique<pascal::RepeatStmt>(
//       std::move(body), std::make_unique<pascal::BinaryExpr>(
//                            std::make_unique<pascal::VariableExpr>("a"), "=",
//                            std::make_unique<pascal::LiteralExpr>("0"))));
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "a:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "L1:\n"
//                              "    mov    rax, [a]\n"
//                              "    sub    rax, 1\n"
//                              "    mov    [a], rax\n"
//                              "    cmp    rax, 0\n"
//                              "    jne    L1\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// // Unsigned int
// TEST(UnsignedTests, Uns1) {
//   std::vector<Token> expected_tokens = {
//       {TT::Var, "var"},     {TT::Identifier, "u"},
//       {TT::Colon, ":"},     {TT::Identifier, "unsigned"},
//       {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   decls.emplace_back(std::make_unique<pascal::VarDecl>(
//       std::vector<std::string>{"u"},
//       std::make_unique<pascal::SimpleTypeSpec>(pascal::BasicType::UnsignedInt,
//                                                "unsigned")));
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "u:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    ret\n";
//   std::string expected_output = "";
//   run_full("var u: unsigned;", expected_tokens, expected_ast, expected_asm,
//            expected_output);
// }
//
// TEST(UnsignedTests, Uns2) {
//   std::vector<Token> expected_tokens = {
//       {TT::Identifier, "u"}, {TT::Colon, ":"},     {TT::Assign, "="},
//       {TT::Number, "1"},     {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   stmts.emplace_back(std::make_unique<pascal::AssignStmt>(
//       std::make_unique<pascal::VariableExpr>("u"),
//       std::make_unique<pascal::LiteralExpr>("1")));
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "u:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    qword [u], 1\n"
//                              "    ret\n";
//   std::string expected_output = "";
//   run_full("u:=1;", expected_tokens, expected_ast, expected_asm,
//            expected_output);
// }
//
// TEST(UnsignedTests, Uns3) {
//   std::vector<Token> expected_tokens = {
//       {TT::While, "while"}, {TT::Identifier, "u"}, {TT::Greater, ">"},
//       {TT::Number, "0"},    {TT::Do, "do"},        {TT::Identifier, "u"},
//       {TT::Colon, ":"},     {TT::Assign, "="},     {TT::Identifier, "u"},
//       {TT::Minus, "-"},     {TT::Number, "1"},     {TT::Semicolon, ";"},
//       {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   stmts.emplace_back(std::make_unique<pascal::WhileStmt>(
//       std::make_unique<pascal::BinaryExpr>(
//           std::make_unique<pascal::VariableExpr>("u"), ">",
//           std::make_unique<pascal::LiteralExpr>("0")),
//       std::make_unique<pascal::AssignStmt>(
//           std::make_unique<pascal::VariableExpr>("u"),
//           std::make_unique<pascal::BinaryExpr>(
//               std::make_unique<pascal::VariableExpr>("u"), "-",
//               std::make_unique<pascal::LiteralExpr>("1")))));
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "u:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "L1:\n"
//                              "    mov    rax, [u]\n"
//                              "    cmp    rax, 0\n"
//                              "    jle    L2\n"
//                              "    mov    rax, [u]\n"
//                              "    sub    rax, 1\n"
//                              "    mov    [u], rax\n"
//                              "    jmp    L1\n"
//                              "L2:\n"
//                              "    ret\n";
//   std::string expected_output = "";
//   run_full("while u>0 do u:=u-1;", expected_tokens, expected_ast,
//   expected_asm,
//            expected_output);
// }
//
// TEST(UnsignedTests, Uns4) {
//   std::vector<Token> expected_tokens = {
//       {TT::Function, "function"}, {TT::Identifier, "f"},
//       {TT::Colon, ":"},           {TT::Identifier, "unsigned"},
//       {TT::Semicolon, ";"},       {TT::Begin, "begin"},
//       {TT::Identifier, "f"},      {TT::Colon, ":"},
//       {TT::Assign, "="},          {TT::Number, "0"},
//       {TT::Semicolon, ";"},       {TT::End, "end"},
//       {TT::Semicolon, ";"},       {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::ParamDecl>> params;
//
//   std::vector<std::unique_ptr<pascal::Statement>> fn_stmts;
//   fn_stmts.emplace_back(std::make_unique<pascal::AssignStmt>(
//       std::make_unique<pascal::VariableExpr>("f"),
//       std::make_unique<pascal::LiteralExpr>("0")));
//   auto fn_block = std::make_unique<pascal::Block>(
//       std::vector<std::unique_ptr<pascal::Declaration>>{},
//       std::move(fn_stmts));
//
//   decls.emplace_back(std::make_unique<pascal::FunctionDecl>(
//       "f", std::move(params),
//       std::make_unique<pascal::SimpleTypeSpec>(pascal::BasicType::UnsignedInt,
//                                                "unsigned"),
//       std::move(fn_block)));
//
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .text\n"
//                              "global f\n"
//                              "f:\n"
//                               "    mov    rax, 0\n"
//                               "    ret\n"
//                               "global main\n"
//                              "main:\n"
//                              "    ret\n";
//   std::string expected_output = "";
//   run_full("function f: unsigned; begin f:=0; end;", expected_tokens,
//            expected_ast, expected_asm, expected_output);
// }
//
// TEST(UnsignedTests, Uns5) {
//   std::vector<Token> expected_tokens = {
//       {TT::For, "for"},     {TT::Identifier, "u"}, {TT::Colon, ":"},
//       {TT::Assign, "="},    {TT::Number, "1"},     {TT::To, "to"},
//       {TT::Number, "5"},    {TT::Do, "do"},        {TT::Identifier, "u"},
//       {TT::Colon, ":"},     {TT::Assign, "="},     {TT::Identifier, "u"},
//       {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   stmts.emplace_back(std::make_unique<pascal::ForStmt>(
//       std::make_unique<pascal::AssignStmt>(
//           std::make_unique<pascal::VariableExpr>("u"),
//           std::make_unique<pascal::LiteralExpr>("1")),
//       false, std::make_unique<pascal::LiteralExpr>("5"),
//       std::make_unique<pascal::AssignStmt>(
//           std::make_unique<pascal::VariableExpr>("u"),
//           std::make_unique<pascal::VariableExpr>("u"))));
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "u:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    qword [u], 1\n"
//                              "L1:\n"
//                              "    mov    rax, [u]\n"
//                              "    cmp    rax, 5\n"
//                              "    jg     L2\n"
//                              "    mov    [u], rax\n"
//                              "    add    qword [u], 1\n"
//                              "    jmp    L1\n"
//                              "L2:\n"
//                              "    ret\n";
//   std::string expected_output = "";
//   run_full("for u:=1 to 5 do u:=u;", expected_tokens, expected_ast,
//            expected_asm, expected_output);
// }
//
// // Long int
// TEST(LongIntTests, Long1) {
//   std::vector<Token> expected_tokens = {
//       {TT::Var, "var"},     {TT::Identifier, "l"},
//       {TT::Colon, ":"},     {TT::Identifier, "longint"},
//       {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   decls.emplace_back(std::make_unique<pascal::VarDecl>(
//       std::vector<std::string>{"l"},
//       std::make_unique<pascal::SimpleTypeSpec>(pascal::BasicType::LongInt,
//                                                "longint")));
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "l:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    ret\n";
//   std::string expected_output = "";
//   run_full("var l: longint;", expected_tokens, expected_ast, expected_asm,
//            expected_output);
// }
//
// TEST(LongIntTests, Long2) {
//   std::vector<Token> expected_tokens = {
//       {TT::Identifier, "l"}, {TT::Colon, ":"},     {TT::Assign, "="},
//       {TT::Number, "1"},     {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   stmts.emplace_back(std::make_unique<pascal::AssignStmt>(
//       std::make_unique<pascal::VariableExpr>("l"),
//       std::make_unique<pascal::LiteralExpr>("1")));
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "l:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    qword [l], 1\n"
//                              "    ret\n";
//   std::string expected_output = "";
//   run_full("l:=1;", expected_tokens, expected_ast, expected_asm,
//            expected_output);
// }
//
// TEST(LongIntTests, Long3) {
//   std::vector<Token> expected_tokens = {
//       {TT::While, "while"}, {TT::Identifier, "l"}, {TT::Greater, ">"},
//       {TT::Number, "0"},    {TT::Do, "do"},        {TT::Identifier, "l"},
//       {TT::Colon, ":"},     {TT::Assign, "="},     {TT::Identifier, "l"},
//       {TT::Minus, "-"},     {TT::Number, "1"},     {TT::Semicolon, ";"},
//       {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   stmts.emplace_back(std::make_unique<pascal::WhileStmt>(
//       std::make_unique<pascal::BinaryExpr>(
//           std::make_unique<pascal::VariableExpr>("l"), ">",
//           std::make_unique<pascal::LiteralExpr>("0")),
//       std::make_unique<pascal::AssignStmt>(
//           std::make_unique<pascal::VariableExpr>("l"),
//           std::make_unique<pascal::BinaryExpr>(
//               std::make_unique<pascal::VariableExpr>("l"), "-",
//               std::make_unique<pascal::LiteralExpr>("1")))));
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "l:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "L1:\n"
//                              "    mov    rax, [l]\n"
//                              "    cmp    rax, 0\n"
//                              "    jle    L2\n"
//                              "    mov    rax, [l]\n"
//                              "    sub    rax, 1\n"
//                              "    mov    [l], rax\n"
//                              "    jmp    L1\n"
//                              "L2:\n"
//                              "    ret\n";
//   std::string expected_output = "";
//   run_full("while l>0 do l:=l-1;", expected_tokens, expected_ast,
//   expected_asm,
//            expected_output);
// }
//
// TEST(LongIntTests, Long4) {
//   std::vector<Token> expected_tokens = {
//       {TT::Function, "function"},  {TT::Identifier, "f"}, {TT::Colon, ":"},
//       {TT::Identifier, "longint"}, {TT::Semicolon, ";"},  {TT::Begin,
//       "begin"}, {TT::Identifier, "f"},       {TT::Colon, ":"}, {TT::Assign,
//       "="}, {TT::Number, "0"},           {TT::Semicolon, ";"},  {TT::End,
//       "end"}, {TT::Semicolon, ";"},        {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::ParamDecl>> params;
//
//   std::vector<std::unique_ptr<pascal::Statement>> fn_stmts;
//   fn_stmts.emplace_back(std::make_unique<pascal::AssignStmt>(
//       std::make_unique<pascal::VariableExpr>("f"),
//       std::make_unique<pascal::LiteralExpr>("0")));
//   auto fn_block = std::make_unique<pascal::Block>(
//       std::vector<std::unique_ptr<pascal::Declaration>>{},
//       std::move(fn_stmts));
//
//   decls.emplace_back(std::make_unique<pascal::FunctionDecl>(
//       "f", std::move(params),
//       std::make_unique<pascal::SimpleTypeSpec>(pascal::BasicType::LongInt,
//                                                "longint"),
//       std::move(fn_block)));
//
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .text\n"
//                              "global f\n"
//                              "f:\n"
//                              "    mov    rax, 0\n"
//                              "    ret\n"
//                              "global main\n"
//                              "main:\n"
//                              "    ret\n";
//   std::string expected_output = "";
//   run_full("function f: longint; begin f:=0; end;", expected_tokens,
//            expected_ast, expected_asm, expected_output);
// }
//
// TEST(LongIntTests, Long5) {
//   std::vector<Token> expected_tokens = {
//       {TT::For, "for"},     {TT::Identifier, "l"}, {TT::Colon, ":"},
//       {TT::Assign, "="},    {TT::Number, "1"},     {TT::To, "to"},
//       {TT::Number, "5"},    {TT::Do, "do"},        {TT::Identifier, "l"},
//       {TT::Colon, ":"},     {TT::Assign, "="},     {TT::Identifier, "l"},
//       {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   stmts.emplace_back(std::make_unique<pascal::ForStmt>(
//       std::make_unique<pascal::AssignStmt>(
//           std::make_unique<pascal::VariableExpr>("l"),
//           std::make_unique<pascal::LiteralExpr>("1")),
//       false, std::make_unique<pascal::LiteralExpr>("5"),
//       std::make_unique<pascal::AssignStmt>(
//           std::make_unique<pascal::VariableExpr>("l"),
//           std::make_unique<pascal::VariableExpr>("l"))));
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "l:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    qword [l], 1\n"
//                              "L1:\n"
//                              "    mov    rax, [l]\n"
//                              "    cmp    rax, 5\n"
//                              "    jg     L2\n"
//                              "    mov    [l], rax\n"
//                              "    add    qword [l], 1\n"
//                              "    jmp    L1\n"
//                              "L2:\n"
//                              "    ret\n";
//   std::string expected_output = "";
//   run_full("for l:=1 to 5 do l:=l;", expected_tokens, expected_ast,
//            expected_asm, expected_output);
// }
//
// // Functions
// TEST(FunctionTests, Func1) {
//   std::string input_str = "function f: integer; begin f:=0; end;";
//   std::vector<Token> expected_tokens = {
//       {TT::Function, "function"},  {TT::Identifier, "f"}, {TT::Colon, ":"},
//       {TT::Identifier, "integer"}, {TT::Semicolon, ";"},  {TT::Begin,
//       "begin"}, {TT::Identifier, "f"},       {TT::Colon, ":"}, {TT::Assign,
//       "="}, {TT::Number, "0"},           {TT::Semicolon, ";"},  {TT::End,
//       "end"}, {TT::Semicolon, ";"},        {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::ParamDecl>> params;
//   std::vector<std::unique_ptr<pascal::Statement>> fn_stmts;
//   fn_stmts.emplace_back(std::make_unique<pascal::AssignStmt>(
//       std::make_unique<pascal::VariableExpr>("f"),
//       std::make_unique<pascal::LiteralExpr>("0")));
//   auto fn_block = std::make_unique<pascal::Block>(
//       std::vector<std::unique_ptr<pascal::Declaration>>{},
//       std::move(fn_stmts));
//   decls.emplace_back(std::make_unique<pascal::FunctionDecl>(
//       "f", std::move(params),
//       std::make_unique<pascal::SimpleTypeSpec>(pascal::BasicType::Integer,
//                                                "integer"),
//       std::move(fn_block)));
//
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .text\n"
//                              "global f\n"
//                              "f:\n"
//                               "    mov    rax, 0\n"
//                               "    ret\n"
//                               "global main\n"
//                              "main:\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// TEST(FunctionTests, Func2) {
//   std::string input_str = "procedure p; begin end;";
//   std::vector<Token> expected_tokens = {
//       {TT::Procedure, "procedure"}, {TT::Identifier, "p"}, {TT::Semicolon,
//       ";"}, {TT::Begin, "begin"},         {TT::End, "end"}, {TT::Semicolon,
//       ";"}, {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::ParamDecl>> params;
//   auto proc_block = std::make_unique<pascal::Block>(
//       std::vector<std::unique_ptr<pascal::Declaration>>{},
//       std::vector<std::unique_ptr<pascal::Statement>>{});
//   decls.emplace_back(std::make_unique<pascal::ProcedureDecl>(
//       "p", std::move(params), std::move(proc_block)));
//
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .text\n"
//                              "global p\n"
//                              "p:\n"
//                               "    ret\n"
//                               "global main\n"
//                              "main:\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// TEST(FunctionTests, Func3) {
//   std::string input_str = "function g(x: integer): integer; begin g:=x;
//   end;"; std::vector<Token> expected_tokens = {{TT::Function, "function"},
//                                         {TT::Identifier, "g"},
//                                         {TT::LeftParen, "("},
//                                         {TT::Identifier, "x"},
//                                         {TT::Colon, ":"},
//                                         {TT::Identifier, "integer"},
//                                         {TT::RightParen, ")"},
//                                         {TT::Colon, ":"},
//                                         {TT::Identifier, "integer"},
//                                         {TT::Semicolon, ";"},
//                                         {TT::Begin, "begin"},
//                                         {TT::Identifier, "g"},
//                                         {TT::Colon, ":"},
//                                         {TT::Assign, "="},
//                                         {TT::Identifier, "x"},
//                                         {TT::Semicolon, ";"},
//                                         {TT::End, "end"},
//                                         {TT::Semicolon, ";"},
//                                         {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::ParamDecl>> params;
//   params.emplace_back(std::make_unique<pascal::ParamDecl>(
//       std::vector<std::string>{"x"},
//       std::make_unique<pascal::SimpleTypeSpec>(pascal::BasicType::Integer,
//                                                "integer")));
//   std::vector<std::unique_ptr<pascal::Statement>> fn_stmts;
//   fn_stmts.emplace_back(std::make_unique<pascal::AssignStmt>(
//       std::make_unique<pascal::VariableExpr>("g"),
//       std::make_unique<pascal::VariableExpr>("x")));
//   auto fn_block = std::make_unique<pascal::Block>(
//       std::vector<std::unique_ptr<pascal::Declaration>>{},
//       std::move(fn_stmts));
//   decls.emplace_back(std::make_unique<pascal::FunctionDecl>(
//       "g", std::move(params),
//       std::make_unique<pascal::SimpleTypeSpec>(pascal::BasicType::Integer,
//                                                "integer"),
//       std::move(fn_block)));
//
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .text\n"
//                              "global g\n"
//                              "g:\n"
//                              "    mov    rax, rdi\n"
//                              "    ret\n"
//                              "global main\n"
//                              "main:\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
//
// }
//
// // Float
// TEST(FloatTests, Float1) {
//   std::string input_str = "var x: real; x:=1.0;";
//   std::vector<Token> expected_tokens = {
//       {TT::Var, "var"},         {TT::Identifier, "x"}, {TT::Colon, ":"},
//       {TT::Identifier, "real"}, {TT::Semicolon, ";"},  {TT::Identifier, "x"},
//       {TT::Colon, ":"},         {TT::Assign, "="},     {TT::Number, "1"},
//       {TT::Dot, "."},           {TT::Number, "0"},     {TT::Semicolon, ";"},
//       {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   decls.emplace_back(std::make_unique<pascal::VarDecl>(
//       std::vector<std::string>{"x"},
//       std::make_unique<pascal::SimpleTypeSpec>(
//                                          pascal::BasicType::Real, "real")));
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   stmts.emplace_back(std::make_unique<pascal::AssignStmt>(
//       std::make_unique<pascal::VariableExpr>("x"),
//       std::make_unique<pascal::LiteralExpr>("1.0")));
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "x:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    qword [x], 0x3FF0000000000000\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// TEST(FloatTests, Float2) {
//   std::string input_str = "x:=1.5+2.5;";
//   std::vector<Token> expected_tokens = {
//       {TT::Identifier, "x"}, {TT::Colon, ":"},     {TT::Assign, "="},
//       {TT::Number, "1"},     {TT::Dot, "."},       {TT::Number, "5"},
//       {TT::Plus, "+"},       {TT::Number, "2"},    {TT::Dot, "."},
//       {TT::Number, "5"},     {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   stmts.emplace_back(std::make_unique<pascal::AssignStmt>(
//       std::make_unique<pascal::VariableExpr>("x"),
//       std::make_unique<pascal::BinaryExpr>(
//           std::make_unique<pascal::LiteralExpr>("1.5"), "+",
//           std::make_unique<pascal::LiteralExpr>("2.5"))));
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "x:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    qword [x], 0x4010000000000000\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// TEST(FloatTests, Float3) {
//   std::string input_str = "if 0.0 < 1.0 then x:=0.0;";
//   std::vector<Token> expected_tokens = {
//       {TT::If, "if"},        {TT::Number, "0"},  {TT::Dot, "."},
//       {TT::Number, "0"},     {TT::Less, "<"},    {TT::Number, "1"},
//       {TT::Dot, "."},        {TT::Number, "0"},  {TT::Then, "then"},
//       {TT::Identifier, "x"}, {TT::Colon, ":"},   {TT::Assign, "="},
//       {TT::Number, "0"},     {TT::Dot, "."},     {TT::Number, "0"},
//       {TT::Semicolon, ";"},  {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   stmts.emplace_back(std::make_unique<pascal::IfStmt>(
//       std::make_unique<pascal::BinaryExpr>(
//           std::make_unique<pascal::LiteralExpr>("0.0"), "<",
//           std::make_unique<pascal::LiteralExpr>("1.0")),
//       std::make_unique<pascal::AssignStmt>(
//           std::make_unique<pascal::VariableExpr>("x"),
//           std::make_unique<pascal::LiteralExpr>("0.0"))));
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "x:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    qword [x], 0x0000000000000000\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// TEST(FloatTests, Float4) {
//   std::string input_str = "while x < 1.0 do x:=x+0.1;";
//   std::vector<Token> expected_tokens = {
//       {TT::While, "while"}, {TT::Identifier, "x"}, {TT::Less, "<"},
//       {TT::Number, "1"},    {TT::Dot, "."},        {TT::Number, "0"},
//       {TT::Do, "do"},       {TT::Identifier, "x"}, {TT::Colon, ":"},
//       {TT::Assign, "="},    {TT::Identifier, "x"}, {TT::Plus, "+"},
//       {TT::Number, "0"},    {TT::Dot, "."},        {TT::Number, "1"},
//       {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   stmts.emplace_back(std::make_unique<pascal::WhileStmt>(
//       std::make_unique<pascal::BinaryExpr>(
//           std::make_unique<pascal::VariableExpr>("x"), "<",
//           std::make_unique<pascal::LiteralExpr>("1.0")),
//       std::make_unique<pascal::AssignStmt>(
//           std::make_unique<pascal::VariableExpr>("x"),
//           std::make_unique<pascal::BinaryExpr>(
//               std::make_unique<pascal::VariableExpr>("x"), "+",
//               std::make_unique<pascal::LiteralExpr>("0.1")))));
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "x:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "L1:\n"
//                              "    mov    rax, [x]\n"
//                              "    cmp    rax, 0x3FF0000000000000\n"
//                              "    jge    L2\n"
//                              "    mov    rax, [x]\n"
//                              "    add    rax, 0x3FB999999999999A\n"
//                              "    mov    [x], rax\n"
//                              "    jmp    L1\n"
//                              "L2:\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// TEST(FloatTests, Float5) {
//   std::string input_str = "function f: real; begin f:=0.0; end;";
//   std::vector<Token> expected_tokens = {
//       {TT::Function, "function"}, {TT::Identifier, "f"}, {TT::Colon, ":"},
//       {TT::Identifier, "real"},   {TT::Semicolon, ";"},  {TT::Begin,
//       "begin"}, {TT::Identifier, "f"},      {TT::Colon, ":"}, {TT::Assign,
//       "="}, {TT::Number, "0"},          {TT::Dot, "."},        {TT::Number,
//       "0"}, {TT::Semicolon, ";"},       {TT::End, "end"}, {TT::Semicolon,
//       ";"}, {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::ParamDecl>> params;
//   std::vector<std::unique_ptr<pascal::Statement>> fn_stmts;
//   fn_stmts.emplace_back(std::make_unique<pascal::AssignStmt>(
//       std::make_unique<pascal::VariableExpr>("f"),
//       std::make_unique<pascal::LiteralExpr>("0.0")));
//   auto fn_block = std::make_unique<pascal::Block>(
//       std::vector<std::unique_ptr<pascal::Declaration>>{},
//       std::move(fn_stmts));
//   decls.emplace_back(std::make_unique<pascal::FunctionDecl>(
//       "f", std::move(params),
//       std::make_unique<pascal::SimpleTypeSpec>(pascal::BasicType::Real,
//       "real"), std::move(fn_block)));
//
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .text\n"
//                              "global f\n"
//                              "f:\n"
//                              "    mov    rax, 0x0000000000000000\n"
//                              "    ret\n"
//                              "global main\n"
//                              "main:\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// // Dynamic memory
// TEST(DynamicTests, Dyn1) {
//   std::string input_str = "new(p);";
//   std::vector<Token> expected_tokens = {
//       {TT::New, "new"},      {TT::LeftParen, "("}, {TT::Identifier, "p"},
//       {TT::RightParen, ")"}, {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   {
//     std::vector<std::unique_ptr<pascal::Expression>> args;
//     args.emplace_back(std::make_unique<pascal::VariableExpr>("p"));
//     stmts.emplace_back(
//         std::make_unique<pascal::ProcCall>("new", std::move(args)));
//   }
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "p:    resq    1\n\n"
//                              "section .text\n"
//                              "extern malloc\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    rdi, 8\n"
//                              "    call   malloc\n"
//                              "    mov    qword [p], rax\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// TEST(DynamicTests, Dyn2) {
//   std::string input_str = "dispose(p);";
//   std::vector<Token> expected_tokens = {
//       {TT::Dispose, "dispose"}, {TT::LeftParen, "("}, {TT::Identifier, "p"},
//       {TT::RightParen, ")"},    {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   {
//     std::vector<std::unique_ptr<pascal::Expression>> args;
//     args.emplace_back(std::make_unique<pascal::VariableExpr>("p"));
//     stmts.emplace_back(
//         std::make_unique<pascal::ProcCall>("dispose", std::move(args)));
//   }
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "p:    resq    1\n\n"
//                              "section .text\n"
//                              "extern free\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    rdi, [p]\n"
//                              "    call   free\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// TEST(DynamicTests, Dyn3) {
//   std::string input_str = "var p:^integer;";
//   std::vector<Token> expected_tokens = {
//       {TT::Var, "var"},   {TT::Identifier, "p"},       {TT::Colon, ":"},
//       {TT::Caret, "^"},   {TT::Identifier, "integer"}, {TT::Semicolon, ";"},
//       {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   decls.emplace_back(std::make_unique<pascal::VarDecl>(
//       std::vector<std::string>{"p"},
//       std::make_unique<pascal::PointerTypeSpec>(
//           std::make_unique<pascal::SimpleTypeSpec>(pascal::BasicType::Integer,
//                                                    "integer"))));
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "p:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// TEST(DynamicTests, Dyn4) {
//   std::string input_str = "p^:=1;";
//   std::vector<Token> expected_tokens = {
//       {TT::Identifier, "p"}, {TT::Caret, "^"},  {TT::Colon, ":"},
//       {TT::Assign, "="},     {TT::Number, "1"}, {TT::Semicolon, ";"},
//       {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<pascal::VariableExpr::Selector> sels;
//   sels.emplace_back("", // empty field
//                     pascal::VariableExpr::Selector::Kind::Pointer);
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   stmts.emplace_back(std::make_unique<pascal::AssignStmt>(
//       std::make_unique<pascal::VariableExpr>(
//           "p",
//
//           std::move(sels) // moves the selector vector in
//           ),
//       std::make_unique<pascal::LiteralExpr>("1")));
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "p:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    rax, [p]\n"
//                              "    mov    qword [rax], 1\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// TEST(DynamicTests, Dyn5) {
//   std::string input_str = "if p<>nil then dispose(p);";
//   std::vector<Token> expected_tokens = {
//       {TT::If, "if"},           {TT::Identifier, "p"},   {TT::Less, "<"},
//       {TT::Greater, ">"},       {TT::Identifier, "nil"}, {TT::Then, "then"},
//       {TT::Dispose, "dispose"}, {TT::LeftParen, "("},    {TT::Identifier,
//       "p"}, {TT::RightParen, ")"},    {TT::Semicolon, ";"}, {TT::EndOfFile,
//       ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   {
//     std::vector<std::unique_ptr<pascal::Expression>> condArgs;
//     condArgs.emplace_back(std::make_unique<pascal::VariableExpr>("p"));
//     auto neq = std::make_unique<pascal::BinaryExpr>(
//         std::make_unique<pascal::VariableExpr>("p"), "<>",
//         std::make_unique<pascal::VariableExpr>("nil"));
//     std::vector<std::unique_ptr<pascal::Expression>> disposeArgs;
//     disposeArgs.emplace_back(std::make_unique<pascal::VariableExpr>("p"));
//     stmts.emplace_back(std::make_unique<pascal::IfStmt>(
//         std::move(neq),
//         std::make_unique<pascal::ProcCall>("dispose",
//         std::move(disposeArgs))));
//   }
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "p:    resq    1\n\n"
//                              "section .text\n"
//                              "extern free\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    rax, [p]\n"
//                              "    cmp    rax, 0\n"
//                              "    je     L1\n"
//                              "    mov    rdi, rax\n"
//                              "    call   free\n"
//                              "L1:\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// // Strings
// TEST(StringTests, Str1) {
//   std::string input_str = "var s: string;";
//   std::vector<Token> expected_tokens = {
//       {TT::Var, "var"},           {TT::Identifier, "s"}, {TT::Colon, ":"},
//       {TT::Identifier, "string"}, {TT::Semicolon, ";"},  {TT::EndOfFile,
//       ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   decls.emplace_back(std::make_unique<pascal::VarDecl>(
//       std::vector<std::string>{"s"},
//       std::make_unique<pascal::SimpleTypeSpec>(
//                                          pascal::BasicType::String,
//                                          "string")));
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "s:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// TEST(StringTests, Str2) {
//   std::string input_str = "s:='hi';";
//   std::vector<Token> expected_tokens = {
//       {TT::Identifier, "s"}, {TT::Colon, ":"},       {TT::Assign, "="},
//       {TT::Identifier, "'"}, {TT::Identifier, "hi"}, {TT::Identifier, "'"},
//       {TT::Semicolon, ";"},  {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   stmts.emplace_back(std::make_unique<pascal::AssignStmt>(
//       std::make_unique<pascal::VariableExpr>("s"),
//       std::make_unique<pascal::LiteralExpr>("'hi'")));
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .data\n"
//                              "str0: db \"hi\", 0\n\n"
//                              "section .bss\n"
//                              "s:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    qword [s], str0\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// TEST(StringTests, Str3) {
//   std::string input_str = "s:=s+'!';";
//   std::vector<Token> expected_tokens = {
//       {TT::Identifier, "s"}, {TT::Colon, ":"},      {TT::Assign, "="},
//       {TT::Identifier, "s"}, {TT::Plus, "+"},       {TT::Identifier, "'"},
//       {TT::Identifier, "!"}, {TT::Identifier, "'"}, {TT::Semicolon, ";"},
//       {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   stmts.emplace_back(std::make_unique<pascal::AssignStmt>(
//       std::make_unique<pascal::VariableExpr>("s"),
//       std::make_unique<pascal::BinaryExpr>(
//           std::make_unique<pascal::VariableExpr>("s"), "+",
//           std::make_unique<pascal::LiteralExpr>("'!'"))));
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .data\n"
//                              "str0: db \"!\", 0\n\n"
//                              "section .bss\n"
//                              "s:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    qword [s], str0\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// TEST(StringTests, Str4) {
//   std::string input_str = "writeln(s);";
//   std::vector<Token> expected_tokens = {
//       {TT::Identifier, "writeln"}, {TT::LeftParen, "("}, {TT::Identifier,
//       "s"}, {TT::RightParen, ")"},       {TT::Semicolon, ";"},
//       {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   {
//     std::vector<std::unique_ptr<pascal::Expression>> args;
//     args.emplace_back(std::make_unique<pascal::VariableExpr>("s"));
//     stmts.emplace_back(
//         std::make_unique<pascal::ProcCall>("writeln", std::move(args)));
//   }
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "s:    resq    1\n\n"
//                              "section .text\n"
//                              "extern puts\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    rdi, [s]\n"
//                              "    call   puts\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// TEST(StringTests, Str5) {
//   std::string input_str = "if s='' then s:='a';";
//   std::vector<Token> expected_tokens = {
//       {TT::If, "if"},        {TT::Identifier, "s"}, {TT::Equal, "="},
//       {TT::Identifier, "'"}, {TT::Identifier, "'"}, {TT::Then, "then"},
//       {TT::Identifier, "s"}, {TT::Colon, ":"},      {TT::Assign, "="},
//       {TT::Identifier, "'"}, {TT::Identifier, "a"}, {TT::Identifier, "'"},
//       {TT::Semicolon, ";"},  {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//
//   std::vector<std::unique_ptr<pascal::Statement>> body;
//   body.emplace_back(std::make_unique<pascal::AssignStmt>(
//       std::make_unique<pascal::VariableExpr>("s"),
//       std::make_unique<pascal::LiteralExpr>("'a'")));
//
//   auto cmp = std::make_unique<pascal::BinaryExpr>(
//       std::make_unique<pascal::VariableExpr>("s"), "=",
//       std::make_unique<pascal::LiteralExpr>("''"));
//   stmts.emplace_back(std::make_unique<pascal::IfStmt>(std::move(cmp),
//                                                       std::move(body.front())));
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .data\n"
//                              "str0: db 0\n"
//                              "str1: db \"a\", 0\n\n"
//                              "section .bss\n"
//                              "s:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    rax, [s]\n"
//                              "    cmp    rax, str0\n"
//                              "    jne    L1\n"
//                              "    mov    qword [s], str1\n"
//                              "L1:\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// // Arrays
// TEST(ArrayTests, Arr1) {
//   std::string input_str = "var a: array[1..5] of integer;";
//   std::vector<Token> expected_tokens = {
//       {TT::Var, "var"},       {TT::Identifier, "a"},
//       {TT::Colon, ":"},       {TT::Array, "array"},
//       {TT::LeftBracket, "["}, {TT::Number, "1"},
//       {TT::Dot, "."},         {TT::Dot, "."},
//       {TT::Number, "5"},      {TT::RightBracket, "]"},
//       {TT::Of, "of"},         {TT::Identifier, "integer"},
//       {TT::Semicolon, ";"},   {TT::EndOfFile, ""}};
//
//   std::vector<pascal::Range> ranges = {pascal::Range(1, 5)};
//
//   AST expected_ast{};
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//
//   decls.emplace_back(std::make_unique<pascal::VarDecl>(
//       std::vector<std::string>{"a"},
//       std::make_unique<pascal::ArrayTypeSpec>(
//           std::move(ranges), // move, no copies
//           std::make_unique<pascal::SimpleTypeSpec>(pascal::BasicType::Integer,
//                                                    "integer"))));
//
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//   std::string expected_asm = "section .bss\n"
//                              "a:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// TEST(ArrayTests, Arr2) {
//   std::string input_str = "a[1]:=0;";
//   std::vector<Token> expected_tokens = {
//       {TT::Identifier, "a"},   {TT::LeftBracket, "["}, {TT::Number, "1"},
//       {TT::RightBracket, "]"}, {TT::Colon, ":"},       {TT::Assign, "="},
//       {TT::Number, "0"},       {TT::Semicolon, ";"},   {TT::EndOfFile, ""}};
//   AST expected_ast{};
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   pascal::VariableExpr::Selector idxSel(
//       std::make_unique<pascal::LiteralExpr>("1"));
//   std::vector<pascal::VariableExpr::Selector> sels;
//   sels.emplace_back(std::move(idxSel));
//   stmts.emplace_back(std::make_unique<pascal::AssignStmt>(
//       std::make_unique<pascal::VariableExpr>("a", std::move(sels)),
//       std::make_unique<pascal::LiteralExpr>("0")));
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//   std::string expected_asm = "section .bss\n"
//                              "a:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    qword [a + 0], 0\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// TEST(ArrayTests, Arr3) {
//   std::string input_str = "for i:=1 to 5 do a[i]:=i;";
//   std::vector<Token> expected_tokens = {
//       {TT::For, "for"},       {TT::Identifier, "i"}, {TT::Colon, ":"},
//       {TT::Assign, "="},      {TT::Number, "1"},     {TT::To, "to"},
//       {TT::Number, "5"},      {TT::Do, "do"},        {TT::Identifier, "a"},
//       {TT::LeftBracket, "["}, {TT::Identifier, "i"}, {TT::RightBracket, "]"},
//       {TT::Colon, ":"},       {TT::Assign, "="},     {TT::Identifier, "i"},
//       {TT::Semicolon, ";"},   {TT::EndOfFile, ""}};
//   AST expected_ast{};
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   pascal::VariableExpr::Selector idx1(
//       std::make_unique<pascal::LiteralExpr>("1"));
//   std::vector<pascal::VariableExpr::Selector> sels1;
//   sels1.emplace_back(std::move(idx1));
//   auto init = std::make_unique<pascal::AssignStmt>(
//       std::make_unique<pascal::VariableExpr>("i"),
//       std::make_unique<pascal::LiteralExpr>("1"));
//   pascal::VariableExpr::Selector idx2(
//       std::make_unique<pascal::VariableExpr>("i"));
//   std::vector<pascal::VariableExpr::Selector> sels2;
//   sels2.emplace_back(std::move(idx2));
//   auto body = std::make_unique<pascal::AssignStmt>(
//       std::make_unique<pascal::VariableExpr>("a", std::move(sels2)),
//       std::make_unique<pascal::VariableExpr>("i"));
//   stmts.emplace_back(std::make_unique<pascal::ForStmt>(
//       std::move(init), false, std::make_unique<pascal::LiteralExpr>("5"),
//       std::move(body)));
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//   std::string expected_asm = "section .bss\n"
//                              "i:    resq    1\n"
//                              "a:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    qword [i], 1\n"
//                              "L1:\n"
//                              "    mov    rax, [i]\n"
//                              "    cmp    rax, 5\n"
//                              "    jg     L2\n"
//                              "    mov    [a], rax\n"
//                              "    add    qword [i], 1\n"
//                              "    jmp    L1\n"
//                              "L2:\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// TEST(ArrayTests, Arr4) {
//   std::string input_str = "writeln(a[1]);";
//   std::vector<Token> expected_tokens = {
//       {TT::Identifier, "writeln"}, {TT::LeftParen, "("},
//       {TT::Identifier, "a"},       {TT::LeftBracket, "["},
//       {TT::Number, "1"},           {TT::RightBracket, "]"},
//       {TT::RightParen, ")"},       {TT::Semicolon, ";"},
//       {TT::EndOfFile, ""}};
//   AST expected_ast{};
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   pascal::VariableExpr::Selector idx(
//       std::make_unique<pascal::LiteralExpr>("1"));
//   std::vector<pascal::VariableExpr::Selector> sels;
//   sels.emplace_back(std::move(idx));
//   std::vector<std::unique_ptr<pascal::Expression>> args;
//   args.emplace_back(
//       std::make_unique<pascal::VariableExpr>("a", std::move(sels)));
//   stmts.emplace_back(
//       std::make_unique<pascal::ProcCall>("writeln", std::move(args)));
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//   std::string expected_asm = "section .bss\n"
//                              "a:    resq    1\n\n"
//                              "section .text\n"
//                              "extern puts\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    rdi, [a]\n"
//                              "    call   puts\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// TEST(ArrayTests, Arr5) {
//   std::string input_str = "if a[1]=0 then a[1]:=1;";
//   std::vector<Token> expected_tokens = {
//       {TT::If, "if"},         {TT::Identifier, "a"},   {TT::LeftBracket,
//       "["}, {TT::Number, "1"},      {TT::RightBracket, "]"}, {TT::Equal,
//       "="}, {TT::Number, "0"},      {TT::Then, "then"},      {TT::Identifier,
//       "a"}, {TT::LeftBracket, "["}, {TT::Number, "1"}, {TT::RightBracket,
//       "]"}, {TT::Colon, ":"},       {TT::Assign, "="},       {TT::Number,
//       "1"}, {TT::Semicolon, ";"},   {TT::EndOfFile, ""}};
//   AST expected_ast{};
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   pascal::VariableExpr::Selector
//   s1(std::make_unique<pascal::LiteralExpr>("1"));
//   std::vector<pascal::VariableExpr::Selector> sels1;
//   sels1.emplace_back(std::move(s1));
//   auto lhs1 = std::make_unique<pascal::VariableExpr>("a", std::move(sels1));
//   auto cond = std::make_unique<pascal::BinaryExpr>(
//       std::move(lhs1), "=", std::make_unique<pascal::LiteralExpr>("0"));
//   pascal::VariableExpr::Selector
//   s2(std::make_unique<pascal::LiteralExpr>("1"));
//   std::vector<pascal::VariableExpr::Selector> sels2;
//   sels2.emplace_back(std::move(s2));
//   auto lhs2 = std::make_unique<pascal::VariableExpr>("a", std::move(sels2));
//   auto thenAssign = std::make_unique<pascal::AssignStmt>(
//       std::move(lhs2), std::make_unique<pascal::LiteralExpr>("1"));
//   stmts.emplace_back(
//       std::make_unique<pascal::IfStmt>(std::move(cond),
//       std::move(thenAssign)));
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//   std::string expected_asm = "section .bss\n"
//                              "a:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    rax, [a]\n"
//                              "    cmp    rax, 0\n"
//                              "    jne    L1\n"
//                              "    mov    qword [a + 0], 1\n"
//                              "L1:\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// // Struct (record)
// TEST(StructTests, Rec1) {
//   std::string input_str = "type r=record a:integer; end;";
//   std::vector<Token> expected_tokens = {
//       {TT::Type, "type"},          {TT::Identifier, "r"}, {TT::Equal, "="},
//       {TT::Record, "record"},      {TT::Identifier, "a"}, {TT::Colon, ":"},
//       {TT::Identifier, "integer"}, {TT::Semicolon, ";"},  {TT::End, "end"},
//       {TT::Semicolon, ";"},        {TT::EndOfFile, ""}};
//   AST expected_ast{};
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   {
//     std::vector<std::unique_ptr<pascal::VarDecl>> fields;
//     fields.emplace_back(std::make_unique<pascal::VarDecl>(
//         std::vector<std::string>{"a"},
//         std::make_unique<pascal::SimpleTypeSpec>(pascal::BasicType::Integer,
//                                                  "integer")));
//     decls.emplace_back(std::make_unique<pascal::TypeDecl>(
//         "r", std::make_unique<pascal::RecordTypeSpec>(std::move(fields))));
//   }
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//   std::string expected_asm = "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// TEST(StructTests, Rec2) {
//   std::string input_str = "var v:r;";
//   std::vector<Token> expected_tokens = {
//       {TT::Var, "var"},      {TT::Identifier, "v"}, {TT::Colon, ":"},
//       {TT::Identifier, "r"}, {TT::Semicolon, ";"},  {TT::EndOfFile, ""}};
//   AST expected_ast{};
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   decls.emplace_back(std::make_unique<pascal::VarDecl>(
//       std::vector<std::string>{"v"},
//       std::make_unique<pascal::SimpleTypeSpec>(
//                                          pascal::BasicType::Integer, "r")));
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//   std::string expected_asm = "section .bss\n"
//                              "v:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// TEST(StructTests, Rec3) {
//   std::string input_str = "v.a:=1;";
//   std::vector<Token> expected_tokens = {
//       {TT::Identifier, "v"}, {TT::Dot, "."},     {TT::Identifier, "a"},
//       {TT::Colon, ":"},      {TT::Assign, "="},  {TT::Number, "1"},
//       {TT::Semicolon, ";"},  {TT::EndOfFile, ""}};
//   AST expected_ast{};
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//
//   // build selector vector by emplacing, avoid copying Selector
//   std::vector<pascal::VariableExpr::Selector> sels;
//   sels.emplace_back("a", pascal::VariableExpr::Selector::Kind::Field);
//
//   stmts.emplace_back(std::make_unique<pascal::AssignStmt>(
//       std::make_unique<pascal::VariableExpr>("v", std::move(sels)),
//       std::make_unique<pascal::LiteralExpr>("1")));
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "v:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    qword [v], 1\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// TEST(StructTests, Rec4) {
//   std::string input_str = "with v do a:=2;";
//   std::vector<Token> expected_tokens = {
//       {TT::With, "with"},    {TT::Identifier, "v"}, {TT::Do, "do"},
//       {TT::Identifier, "a"}, {TT::Colon, ":"},      {TT::Assign, "="},
//       {TT::Number, "2"},     {TT::Semicolon, ";"},  {TT::EndOfFile, ""}};
//   AST expected_ast{};
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   stmts.emplace_back(std::make_unique<pascal::WithStmt>(
//       std::make_unique<pascal::VariableExpr>("v"),
//       std::make_unique<pascal::AssignStmt>(
//           std::make_unique<pascal::VariableExpr>("a"),
//           std::make_unique<pascal::LiteralExpr>("2"))));
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//   std::string expected_asm = "section .bss\n"
//                              "v:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    qword [v], 2\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// TEST(StructTests, Rec5) {
//   std::string input_str = "if v.a=0 then v.a:=1;";
//   std::vector<Token> expected_tokens = {
//       {TT::If, "if"},        {TT::Identifier, "v"}, {TT::Dot, "."},
//       {TT::Identifier, "a"}, {TT::Equal, "="},      {TT::Number, "0"},
//       {TT::Then, "then"},    {TT::Identifier, "v"}, {TT::Dot, "."},
//       {TT::Identifier, "a"}, {TT::Colon, ":"},      {TT::Assign, "="},
//       {TT::Number, "1"},     {TT::Semicolon, ";"},  {TT::EndOfFile, ""}};
//   AST expected_ast{};
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//
//   // build lhs1 selector in place
//   std::vector<pascal::VariableExpr::Selector> sels1;
//   sels1.emplace_back("a", pascal::VariableExpr::Selector::Kind::Field);
//   auto lhs1 = std::make_unique<pascal::VariableExpr>("v", std::move(sels1));
//   auto cond = std::make_unique<pascal::BinaryExpr>(
//       std::move(lhs1), "=", std::make_unique<pascal::LiteralExpr>("0"));
//
//   // build thenAssign selector in place
//   std::vector<pascal::VariableExpr::Selector> sels2;
//   sels2.emplace_back("a", pascal::VariableExpr::Selector::Kind::Field);
//   auto lhs2 = std::make_unique<pascal::VariableExpr>("v", std::move(sels2));
//   auto thenAssign = std::make_unique<pascal::AssignStmt>(
//       std::move(lhs2), std::make_unique<pascal::LiteralExpr>("1"));
//
//   stmts.emplace_back(
//       std::make_unique<pascal::IfStmt>(std::move(cond),
//       std::move(thenAssign)));
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "v:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    rax, [v]\n"
//                              "    cmp    rax, 0\n"
//                              "    jne    L1\n"
//                              "    mov    qword [v], 1\n"
//                              "L1:\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// // Pointers
// TEST(PointerTests, Ptr1) {
//   std::string input_str = "var p:^integer;";
//   std::vector<Token> expected_tokens = {
//       {TT::Var, "var"},   {TT::Identifier, "p"},       {TT::Colon, ":"},
//       {TT::Caret, "^"},   {TT::Identifier, "integer"}, {TT::Semicolon, ";"},
//       {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   decls.emplace_back(std::make_unique<pascal::VarDecl>(
//       std::vector<std::string>{"p"},
//       std::make_unique<pascal::PointerTypeSpec>(
//           std::make_unique<pascal::SimpleTypeSpec>(pascal::BasicType::Integer,
//                                                    "integer"))));
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "p:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    ret\n";
//   std::string expected_output = "";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm,
//            expected_output);
// }
//
// TEST(PointerTests, Ptr2) {
//   std::string input_str = "new(p);";
//   std::vector<Token> expected_tokens = {
//       {TT::New, "new"},      {TT::LeftParen, "("}, {TT::Identifier, "p"},
//       {TT::RightParen, ")"}, {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   {
//     std::vector<std::unique_ptr<pascal::Expression>> args;
//     args.emplace_back(std::make_unique<pascal::VariableExpr>("p"));
//     stmts.emplace_back(
//         std::make_unique<pascal::ProcCall>("new", std::move(args)));
//   }
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "p:    resq    1\n\n"
//                              "section .text\n"
//                              "extern malloc\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    rdi, 8\n"
//                              "    call   malloc\n"
//                              "    mov    qword [p], rax\n"
//                              "    ret\n";
//   std::string expected_output = "";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm,
//            expected_output);
// }
//
// // Pointers
// TEST(PointerTests, Ptr3) {
//   std::string input_str = "p^:=1;";
//   std::vector<Token> expected_tokens = {
//       {TT::Identifier, "p"}, {TT::Caret, "^"},  {TT::Colon, ":"},
//       {TT::Assign, "="},     {TT::Number, "1"}, {TT::Semicolon, ";"},
//       {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   // build selector in-place
//   std::vector<pascal::VariableExpr::Selector> sels;
//   sels.emplace_back("", pascal::VariableExpr::Selector::Kind::Pointer);
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   stmts.emplace_back(std::make_unique<pascal::AssignStmt>(
//       // move the selector vector into VariableExpr
//       std::make_unique<pascal::VariableExpr>("p", std::move(sels)),
//       std::make_unique<pascal::LiteralExpr>("1")));
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "p:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    rax, [p]\n"
//                              "    mov    qword [rax], 1\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// TEST(PointerTests, Ptr4) {
//   std::string input_str = "dispose(p);";
//   std::vector<Token> expected_tokens = {
//       {TT::Dispose, "dispose"}, {TT::LeftParen, "("}, {TT::Identifier, "p"},
//       {TT::RightParen, ")"},    {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   {
//     // build call arguments by moving into vector
//     std::vector<std::unique_ptr<pascal::Expression>> args;
//     args.emplace_back(std::make_unique<pascal::VariableExpr>("p"));
//     stmts.emplace_back(
//         std::make_unique<pascal::ProcCall>("dispose", std::move(args)));
//   }
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .bss\n"
//                              "p:    resq    1\n\n"
//                              "section .text\n"
//                              "extern free\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    rdi, [p]\n"
//                              "    call   free\n"
//                              "    ret\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm, "");
// }
//
// // Print output tests
// TEST(PrintTests, PrintString) {
//   std::string input_str = "writeln('hello');";
//   std::vector<Token> expected_tokens = {
//       {TT::Identifier, "writeln"}, {TT::LeftParen, "("},  {TT::Identifier,
//       "'"}, {TT::Identifier, "hello"},   {TT::Identifier, "'"},
//       {TT::RightParen, ")"}, {TT::Semicolon, ";"},        {TT::EndOfFile,
//       ""}};
//   AST expected_ast{};
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   {
//     std::vector<std::unique_ptr<pascal::Expression>> args;
//     args.emplace_back(std::make_unique<pascal::LiteralExpr>("'hello'"));
//     stmts.emplace_back(
//         std::make_unique<pascal::ProcCall>("writeln", std::move(args)));
//   }
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .data\n"
//                              "str0: db \"hello\", 0\n\n"
//                              "section .text\n"
//                              "extern puts\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    rdi, str0\n"
//                              "    ret\n";
//   std::string expected_output = "hello\n";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm,
//            expected_output);
// }
//
// TEST(PrintTests, PrintIntLiteral) {
//   std::string input_str = "writeln(123);";
//   std::vector<Token> expected_tokens = {
//       {TT::Identifier, "writeln"}, {TT::LeftParen, "("}, {TT::Number, "123"},
//       {TT::RightParen, ")"},       {TT::Semicolon, ";"}, {TT::EndOfFile,
//       ""}};
//   AST expected_ast{};
//
//   // build the proc-call arguments by moving into a vector
//   std::vector<std::unique_ptr<pascal::Expression>> args;
//   args.emplace_back(std::make_unique<pascal::LiteralExpr>("123"));
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   stmts.emplace_back(
//       std::make_unique<pascal::ProcCall>("writeln", std::move(args)));
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .data\n"
//                              "fmt_int: db \"%d\", 10, 0\n\n"
//                              "section .text\n"
//                              "extern printf\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    rdi, fmt_int\n"
//                              "    mov    rsi, 123\n"
//                              "    xor    rax, rax\n"
//                              "    call   printf\n"
//                              "    ret\n";
//   std::string expected_output = "123\n";
//
//   run_full(input_str, expected_tokens, expected_ast, expected_asm,
//            expected_output);
// }
//
// TEST(PrintTests, PrintIntExpr) {
//   std::string input_str = "writeln(10+20);";
//   std::vector<Token> expected_tokens = {
//       {TT::Identifier, "writeln"}, {TT::LeftParen, "("},
//       {TT::Number, "10"},          {TT::Plus, "+"},
//       {TT::Number, "20"},          {TT::RightParen, ")"},
//       {TT::Semicolon, ";"},        {TT::EndOfFile, ""}};
//   AST expected_ast{};
//
//   // build the expression argument in a vector
//   std::vector<std::unique_ptr<pascal::Expression>> args;
//   args.emplace_back(std::make_unique<pascal::BinaryExpr>(
//       std::make_unique<pascal::LiteralExpr>("10"), "+",
//       std::make_unique<pascal::LiteralExpr>("20")));
//
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   stmts.emplace_back(
//       std::make_unique<pascal::ProcCall>("writeln", std::move(args)));
//
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   std::string expected_asm = "section .data\n"
//                              "fmt_int: db \"%d\", 10, 0\n\n"
//                              "section .text\n"
//                              "extern printf\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    rdi, fmt_int\n"
//                              "    mov    rsi, 10\n"
//                              "    add    rsi, 20\n"
//                              "    xor    rax, rax\n"
//                              "    call   printf\n"
//                              "    ret\n";
//   std::string expected_output = "30\n";
//
//   run_full(input_str, expected_tokens, expected_ast, expected_asm,
//            expected_output);
// }
//
// // Validation error tests
// TEST(InvalidCodeTests, TypeDeclMissingName) {
//   std::string input_str = "type = integer;";
//   std::vector<Token> expected_tokens = {{TT::Type, "type"},
//                                         {TT::Equal, "="},
//                                         {TT::Identifier, "integer"},
//                                         {TT::Semicolon, ";"},
//                                         {TT::EndOfFile, ""}};
//
//   AST expected_ast{};
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   decls.emplace_back(std::make_unique<pascal::TypeDecl>(
//       "",
//       std::make_unique<pascal::SimpleTypeSpec>(pascal::BasicType::Integer,
//                                                "integer")));
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   test_utils::run_validation_fail(input_str, expected_tokens, expected_ast,
//   "", "",
//                       "TypeDecl missing name");
// }
//
// TEST(InvalidCodeTests, ProcedureMissingName) {
//   std::string input_str = "procedure ; begin end;";
//   std::vector<Token> expected_tokens = {{TT::Procedure, "procedure"},
//                                         {TT::Semicolon, ";"},
//                                         {TT::Begin, "begin"},
//                                         {TT::End, "end"},
//                                         {TT::Semicolon, ";"},
//                                         {TT::EndOfFile, ""}};
//
//   AST expected_ast{};
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::ParamDecl>> params;
//   std::vector<std::unique_ptr<pascal::Statement>> body_stmts;
//   body_stmts.emplace_back(
//       std::make_unique<pascal::CompoundStmt>(
//           std::vector<std::unique_ptr<pascal::Statement>>{}));
//   auto body_block = std::make_unique<pascal::Block>(
//       std::vector<std::unique_ptr<pascal::Declaration>>{},
//       std::move(body_stmts));
//   decls.emplace_back(std::make_unique<pascal::ProcedureDecl>(
//       "", std::move(params), std::move(body_block)));
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   test_utils::run_validation_fail(input_str, expected_tokens, expected_ast,
//   "", "",
//                       "ProcedureDecl missing name");
// }
//
// TEST(InvalidCodeTests, FunctionMissingName) {
//   std::string input_str = "function : integer; begin end;";
//   std::vector<Token> expected_tokens = {{TT::Function, "function"},
//                                         {TT::Colon, ":"},
//                                         {TT::Identifier, "integer"},
//                                         {TT::Semicolon, ";"},
//                                         {TT::Begin, "begin"},
//                                         {TT::End, "end"},
//                                         {TT::Semicolon, ";"},
//                                         {TT::EndOfFile, ""}};
//
//   AST expected_ast{};
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::ParamDecl>> params;
//   std::vector<std::unique_ptr<pascal::Statement>> body_stmts;
//   body_stmts.emplace_back(
//       std::make_unique<pascal::CompoundStmt>(
//           std::vector<std::unique_ptr<pascal::Statement>>{}));
//   auto body_block = std::make_unique<pascal::Block>(
//       std::vector<std::unique_ptr<pascal::Declaration>>{},
//       std::move(body_stmts));
//   decls.emplace_back(std::make_unique<pascal::FunctionDecl>(
//       "", std::move(params),
//       std::make_unique<pascal::SimpleTypeSpec>(pascal::BasicType::Integer,
//                                                "integer"),
//       std::move(body_block)));
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   test_utils::run_validation_fail(input_str, expected_tokens, expected_ast,
//   "", "",
//                       "FunctionDecl missing name");
// }
//
// TEST(InvalidCodeTests, ParamMissingName) {
//   std::string input_str = "procedure p(:integer); begin end;";
//   std::vector<Token> expected_tokens = {{TT::Procedure, "procedure"},
//                                         {TT::Identifier, "p"},
//                                         {TT::LeftParen, "("},
//                                         {TT::Colon, ":"},
//                                         {TT::Identifier, "integer"},
//                                         {TT::RightParen, ")"},
//                                         {TT::Semicolon, ";"},
//                                         {TT::Begin, "begin"},
//                                         {TT::End, "end"},
//                                         {TT::Semicolon, ";"},
//                                         {TT::EndOfFile, ""}};
//
//   AST expected_ast{};
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::ParamDecl>> params;
//   params.emplace_back(std::make_unique<pascal::ParamDecl>(
//       std::vector<std::string>{},
//       std::make_unique<pascal::SimpleTypeSpec>(pascal::BasicType::Integer,
//                                                "integer")));
//   std::vector<std::unique_ptr<pascal::Statement>> body_stmts;
//   body_stmts.emplace_back(
//       std::make_unique<pascal::CompoundStmt>(
//           std::vector<std::unique_ptr<pascal::Statement>>{}));
//   auto body_block = std::make_unique<pascal::Block>(
//       std::vector<std::unique_ptr<pascal::Declaration>>{},
//       std::move(body_stmts));
//   decls.emplace_back(std::make_unique<pascal::ProcedureDecl>(
//       "p", std::move(params), std::move(body_block)));
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   test_utils::run_validation_fail(input_str, expected_tokens, expected_ast,
//   "", "",
//                       "ParamDecl missing names");
// }
//
// TEST(InvalidCodeTests, CaseLabelMissingStmt) {
//   std::string input_str = "case a of 1: ; end;";
//   std::vector<Token> expected_tokens = {{TT::Case, "case"},
//                                         {TT::Identifier, "a"},
//                                         {TT::Of, "of"},
//                                         {TT::Number, "1"},
//                                         {TT::Colon, ":"},
//                                         {TT::Semicolon, ";"},
//                                         {TT::End, "end"},
//                                         {TT::Semicolon, ";"},
//                                         {TT::EndOfFile, ""}};
//
//   AST expected_ast{};
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   {
//     std::vector<std::unique_ptr<pascal::Expression>> consts;
//     consts.emplace_back(std::make_unique<pascal::LiteralExpr>("1"));
//     std::vector<std::unique_ptr<pascal::CaseLabel>> labels;
//     labels.emplace_back(
//         std::make_unique<pascal::CaseLabel>(std::move(consts), nullptr));
//     stmts.emplace_back(std::make_unique<pascal::CaseStmt>(
//         std::make_unique<pascal::VariableExpr>("a"), std::move(labels)));
//   }
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   test_utils::run_validation_fail(input_str, expected_tokens, expected_ast,
//   "", "",
//                       "CaseLabel missing statement");
// }
//
// TEST(InvalidCodeTests, EmptyRecord) {
//   std::string input_str = "type R = record end;";
//   std::vector<Token> expected_tokens = {{TT::Type, "type"},
//                                         {TT::Identifier, "R"},
//                                         {TT::Equal, "="},
//                                         {TT::Record, "record"},
//                                         {TT::End, "end"},
//                                         {TT::Semicolon, ";"},
//                                         {TT::EndOfFile, ""}};
//
//   AST expected_ast{};
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   decls.emplace_back(std::make_unique<pascal::TypeDecl>(
//       "R", std::make_unique<pascal::RecordTypeSpec>(
//                std::vector<std::unique_ptr<pascal::VarDecl>>{})));
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   test_utils::run_validation_fail(input_str, expected_tokens, expected_ast,
//   "", "",
//                       "RecordTypeSpec with no fields");
// }
//
// TEST(InvalidCodeTests, ForMissingBody) {
//   std::string input_str = "for i:=1 to 10 do ;";
//   std::vector<Token> expected_tokens = {{TT::For, "for"},
//                                         {TT::Identifier, "i"},
//                                         {TT::Colon, ":"},
//                                         {TT::Assign, "="},
//                                         {TT::Number, "1"},
//                                         {TT::To, "to"},
//                                         {TT::Number, "10"},
//                                         {TT::Do, "do"},
//                                         {TT::Semicolon, ";"},
//                                         {TT::EndOfFile, ""}};
//
//   AST expected_ast{};
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   stmts.emplace_back(std::make_unique<pascal::ForStmt>(
//       std::make_unique<pascal::AssignStmt>(
//           std::make_unique<pascal::VariableExpr>("i"),
//           std::make_unique<pascal::LiteralExpr>("1")),
//       false, std::make_unique<pascal::LiteralExpr>("10"), nullptr));
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   test_utils::run_validation_fail(input_str, expected_tokens, expected_ast,
//   "", "",
//                       "ForStmt missing body");
// }
//
// TEST(InvalidCodeTests, WhileMissingBody) {
//   std::string input_str = "while a>0 do ;";
//   std::vector<Token> expected_tokens = {{TT::While, "while"},
//                                         {TT::Identifier, "a"},
//                                         {TT::Greater, ">"},
//                                         {TT::Number, "0"},
//                                         {TT::Do, "do"},
//                                         {TT::Semicolon, ";"},
//                                         {TT::EndOfFile, ""}};
//
//   AST expected_ast{};
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   stmts.emplace_back(std::make_unique<pascal::WhileStmt>(
//       std::make_unique<pascal::BinaryExpr>(
//           std::make_unique<pascal::VariableExpr>("a"), ">",
//           std::make_unique<pascal::LiteralExpr>("0")),
//       nullptr));
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   test_utils::run_validation_fail(input_str, expected_tokens, expected_ast,
//   "", "",
//                       "WhileStmt missing body");
// }
//
// TEST(InvalidCodeTests, IfMissingThenBranch) {
//   std::string input_str = "if a>0 then ;";
//   std::vector<Token> expected_tokens = {{TT::If, "if"},
//                                         {TT::Identifier, "a"},
//                                         {TT::Greater, ">"},
//                                         {TT::Number, "0"},
//                                         {TT::Then, "then"},
//                                         {TT::Semicolon, ";"},
//                                         {TT::EndOfFile, ""}};
//
//   AST expected_ast{};
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   stmts.emplace_back(std::make_unique<pascal::IfStmt>(
//       std::make_unique<pascal::BinaryExpr>(
//           std::make_unique<pascal::VariableExpr>("a"), ">",
//           std::make_unique<pascal::LiteralExpr>("0")),
//       nullptr, nullptr));
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   test_utils::run_validation_fail(input_str, expected_tokens, expected_ast,
//   "", "",
//                       "IfStmt missing then branch");
// }
//
// TEST(InvalidCodeTests, WithMissingBody) {
//   std::string input_str = "with a do ;";
//   std::vector<Token> expected_tokens = {{TT::With, "with"},
//                                         {TT::Identifier, "a"},
//                                         {TT::Do, "do"},
//                                         {TT::Semicolon, ";"},
//                                         {TT::EndOfFile, ""}};
//
//   AST expected_ast{};
//   std::vector<std::unique_ptr<pascal::Declaration>> decls;
//   std::vector<std::unique_ptr<pascal::Statement>> stmts;
//   stmts.emplace_back(std::make_unique<pascal::WithStmt>(
//       std::make_unique<pascal::VariableExpr>("a"), nullptr));
//   auto block =
//       std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
//   expected_ast.root =
//       std::make_unique<pascal::Program>("test", std::move(block));
//   expected_ast.valid = true;
//
//   test_utils::run_validation_fail(input_str, expected_tokens, expected_ast,
//   "", "",
//                       "WithStmt missing body");
// }
