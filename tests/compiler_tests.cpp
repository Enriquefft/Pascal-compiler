#include "test_utils.hpp"
// Diagnostics:
// 1. In included file: call to deleted constructor of
// 'unique_ptr<pascal::Declaration>' [ovl_deleted_init]
// 2. Too many errors emitted, stopping now [fatal_too_many_errors]
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

// TEST(VarDeclTests, Decl2) {
//   std::string input_str = "var b: real;";
//   vector<Token> expected_tokens = {
//       {TT::Var, "var"},         {TT::Identifier, "b"}, {TT::Colon, ":"},
//       {TT::Identifier, "real"}, {TT::Semicolon, ";"},  {TT::EndOfFile, ""}};
//   AST expected_ast{};
//   expected_ast.root = make_unique<pascal::Program>(
//       "test", make_unique<pascal::Block>(
//                   vector<unique_ptr<pascal::Declaration>>{
//                       make_unique<pascal::VarDecl>(
//                           vector<std::string>{"b"},
//                           make_unique<pascal::SimpleTypeSpec>(
//                               pascal::BasicType::Real, "real"))},
//                   vector<unique_ptr<pascal::Statement>>{}));
//   expected_ast.valid = true;
//   std::string expected_asm = "section .bss\n"
//                              "b:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    ret\n";
//   std::string expected_output = "";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm,
//            expected_output);
// }
//
// TEST(VarDeclTests, Decl3) {
//   std::string input_str = "var c: unsigned;";
//   vector<Token> expected_tokens = {
//       {TT::Var, "var"},     {TT::Identifier, "c"},
//       {TT::Colon, ":"},     {TT::Identifier, "unsigned"},
//       {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
//   AST expected_ast{};
//   expected_ast.root = make_unique<pascal::Program>(
//       "test", make_unique<pascal::Block>(
//                   vector<unique_ptr<pascal::Declaration>>{
//                       make_unique<pascal::VarDecl>(
//                           vector<std::string>{"c"},
//                           make_unique<pascal::SimpleTypeSpec>(
//                               pascal::BasicType::UnsignedInt, "unsigned"))},
//                   vector<unique_ptr<pascal::Statement>>{}));
//   expected_ast.valid = true;
//   std::string expected_asm = "section .bss\n"
//                              "c:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    ret\n";
//   std::string expected_output = "";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm,
//            expected_output);
// }
//
// // Expressions
// TEST(ExpressionTests, Expr1) {
//   std::string input_str = "begin a := 1; end.";
//   vector<Token> expected_tokens = {
//       {TT::Begin, "begin"}, {TT::Identifier, "a"}, {TT::Colon, ":"},
//       {TT::Assign, "="},    {TT::Number, "1"},     {TT::Semicolon, ";"},
//       {TT::End, "end"},     {TT::Dot, "."},        {TT::EndOfFile, ""}};
//   AST expected_ast{};
//   expected_ast.root = make_unique<pascal::Program>(
//       "test",
//       make_unique<pascal::Block>(
//           vector<unique_ptr<pascal::Declaration>>{},
//           vector<unique_ptr<pascal::Statement>>{
//               make_unique<pascal::CompoundStmt>(
//                   vector<unique_ptr<pascal::Statement>>{
//                       make_unique<pascal::AssignStmt>(
//                           make_unique<pascal::VariableExpr>("a"),
//                           make_unique<pascal::LiteralExpr>("1"))})}));
//   expected_ast.valid = true;
//   std::string expected_asm = "section .bss\n"
//                              "a:    resq    1\n\n"
//                              "section .text\n"
//                              "global main\n"
//                              "main:\n"
//                              "    mov    qword [a], 1\n"
//                              "    ret\n";
//   std::string expected_output = "";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm,
//            expected_output);
// }
//
// TEST(ExpressionTests, Expr2) {
//   std::string input_str = "begin b := a + 1; end.";
//   vector<Token> expected_tokens = {
//       {TT::Begin, "begin"}, {TT::Identifier, "b"}, {TT::Colon, ":"},
//       {TT::Assign, "="},    {TT::Identifier, "a"}, {TT::Plus, "+"},
//       {TT::Number, "1"},    {TT::Semicolon, ";"},  {TT::End, "end"},
//       {TT::Dot, "."},       {TT::EndOfFile, ""}};
//   AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test",
//         make_unique<pascal::Block>(
//             vector<unique_ptr<pascal::Declaration>>{},
//             vector<unique_ptr<pascal::Statement>>{
//                 make_unique<pascal::CompoundStmt>(
//                     vector<unique_ptr<pascal::Statement>>{
//                         make_unique<pascal::AssignStmt>(
//                             make_unique<pascal::VariableExpr>("b"),
//                             make_unique<pascal::BinaryExpr>(
//                                 make_unique<pascal::VariableExpr>("a"),
//                                 "+",
//                                 make_unique<pascal::LiteralExpr>("1"))))}))});
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "a:    resq    1\n"
//         "b:    resq    1\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    mov    rax, [a]\n"
//         "    add    rax, 1\n"
//         "    mov    [b], rax\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(ExpressionTests, Expr3) {
//   std::string input_str = "begin c := b * 2; end.";
//   vector<Token> expected_tokens = {
//       {TT::Begin, "begin"}, {TT::Identifier, "c"}, {TT::Colon, ":"},
//       {TT::Assign, "="},    {TT::Identifier, "b"}, {TT::Star, "*"},
//       {TT::Number, "2"},    {TT::Semicolon, ";"},  {TT::End, "end"},
//       {TT::Dot, "."},       {TT::EndOfFile, ""}};
//   AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test",
//         make_unique<pascal::Block>(
//             vector<unique_ptr<pascal::Declaration>>{},
//             vector<unique_ptr<pascal::Statement>>{
//                 make_unique<pascal::CompoundStmt>(
//                     vector<unique_ptr<pascal::Statement>>{
//                         make_unique<pascal::AssignStmt>(
//                             make_unique<pascal::VariableExpr>("c"),
//                             make_unique<pascal::BinaryExpr>(
//                                 make_unique<pascal::VariableExpr>("b"),
//                                 "*",
//                                 make_unique<pascal::LiteralExpr>("2"))))}))});
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "b:    resq    1\n"
//         "c:    resq    1\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    mov    rax, [b]\n"
//         "    imul   rax, 2\n"
//         "    mov    [c], rax\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// // Control statements
// TEST(ControlTests, IfStmt) {
//   std::string input_str = "if a > 0 then b := 1;";
//   vector<Token> expected_tokens = {
//       {TT::If, "if"},       {TT::Identifier, "a"}, {TT::Greater, ">"},
//       {TT::Number, "0"},    {TT::Then, "then"},    {TT::Identifier, "b"},
//       {TT::Colon, ":"},     {TT::Assign, "="},     {TT::Number, "1"},
//       {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
//   AST expected_ast{};
//   expected_ast.root = make_unique<pascal::Program>(
//       "test", make_unique<pascal::Block>(
//                   vector<unique_ptr<pascal::Declaration>>{},
//                   vector<unique_ptr<pascal::Statement>>{
//                       make_unique<pascal::IfStmt>(
//                           make_unique<pascal::BinaryExpr>(
//                               make_unique<pascal::VariableExpr>("a"),
//                               ">",
//                               make_unique<pascal::LiteralExpr>("0")),
//                           make_unique<pascal::AssignStmt>(
//                               make_unique<pascal::VariableExpr>("b"),
//                               make_unique<pascal::LiteralExpr>("1")))}));
//   expected_ast.valid = true;
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
//   std::string expected_output = "";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm,
//            expected_output);
// }
//
// TEST(ControlTests, IfElse) {
//   std::string input_str = "if a > 0 then b := 1 else b := 2;";
//   vector<Token> expected_tokens = {
//       {TT::If, "if"},     {TT::Identifier, "a"}, {TT::Greater, ">"},
//       {TT::Number, "0"},  {TT::Then, "then"},    {TT::Identifier, "b"},
//       {TT::Colon, ":"},   {TT::Assign, "="},     {TT::Number, "1"},
//       {TT::Else, "else"}, {TT::Identifier, "b"}, {TT::Colon, ":"},
//       {TT::Assign, "="},  {TT::Number, "2"},     {TT::Semicolon, ";"},
//       {TT::EndOfFile, ""}};
//   AST expected_ast{};
//   expected_ast.root = make_unique<pascal::Program>(
//       "test", make_unique<pascal::Block>(
//                   vector<unique_ptr<pascal::Declaration>>{},
//                   vector<unique_ptr<pascal::Statement>>{
//                       make_unique<pascal::IfStmt>(
//                           make_unique<pascal::BinaryExpr>(
//                               make_unique<pascal::VariableExpr>("a"),
//                               ">",
//                               make_unique<pascal::LiteralExpr>("0")),
//                           make_unique<pascal::AssignStmt>(
//                               make_unique<pascal::VariableExpr>("b"),
//                               make_unique<pascal::LiteralExpr>("1")),
//                           make_unique<pascal::AssignStmt>(
//                               make_unique<pascal::VariableExpr>("b"),
//                               make_unique<pascal::LiteralExpr>("2")))}));
//   expected_ast.valid = true;
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
//   std::string expected_output = "";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm,
//            expected_output);
// }
//
// TEST(ControlTests, CaseStmt) {
//   std::string input_str = "case a of 1: b := 1; end;";
//   vector<Token> expected_tokens = {
//       {TT::Case, "case"},   {TT::Identifier, "a"}, {TT::Of, "of"},
//       {TT::Number, "1"},    {TT::Colon, ":"},      {TT::Identifier, "b"},
//       {TT::Colon, ":"},     {TT::Assign, "="},     {TT::Number, "1"},
//       {TT::Semicolon, ";"}, {TT::End, "end"},      {TT::Semicolon, ";"},
//       {TT::EndOfFile, ""}};
//   AST expected_ast{};
//   expected_ast.root = make_unique<pascal::Program>(
//       "test",
//       make_unique<pascal::Block>(
//           vector<unique_ptr<pascal::Declaration>>{},
//           vector<unique_ptr<pascal::Statement>>{
//               make_unique<pascal::CaseStmt>(
//                   make_unique<pascal::VariableExpr>("a"),
//                   vector<unique_ptr<pascal::CaseLabel>>{
//                       make_unique<pascal::CaseLabel>(
//                           vector<unique_ptr<pascal::Expression>>{
//                               make_unique<pascal::LiteralExpr>("1")},
//                           make_unique<pascal::AssignStmt>(
//                               make_unique<pascal::VariableExpr>("b"),
//                               make_unique<pascal::LiteralExpr>("1")))})}));
//   expected_ast.valid = true;
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
//   std::string expected_output = "";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm,
//            expected_output);
// }
//
// TEST(ControlTests, WhileStmt) {
//   std::string input_str = "while a > 0 do a := a - 1;";
//   vector<Token> expected_tokens = {
//       {TT::While, "while"}, {TT::Identifier, "a"}, {TT::Greater, ">"},
//       {TT::Number, "0"},    {TT::Do, "do"},        {TT::Identifier, "a"},
//       {TT::Colon, ":"},     {TT::Assign, "="},     {TT::Identifier, "a"},
//       {TT::Minus, "-"},     {TT::Number, "1"},     {TT::Semicolon, ";"},
//       {TT::EndOfFile, ""}};
//   AST expected_ast{};
//   expected_ast.root = make_unique<pascal::Program>(
//       "test",
//       make_unique<pascal::Block>(
//           vector<unique_ptr<pascal::Declaration>>{},
//           vector<unique_ptr<pascal::Statement>>{
//               make_unique<pascal::WhileStmt>(
//                   make_unique<pascal::BinaryExpr>(
//                       make_unique<pascal::VariableExpr>("a"), ">",
//                       make_unique<pascal::LiteralExpr>("0")),
//                   make_unique<pascal::AssignStmt>(
//                       make_unique<pascal::VariableExpr>("a"),
//                       make_unique<pascal::BinaryExpr>(
//                           make_unique<pascal::VariableExpr>("a"), "-",
//                           make_unique<pascal::LiteralExpr>("1"))))}));
//   expected_ast.valid = true;
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
//   std::string expected_output = "";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm,
//            expected_output);
// }
//
// TEST(ControlTests, ForStmt) {
//   std::string input_str = "for i:=1 to 10 do a:=i;";
//   vector<Token> expected_tokens = {
//       {TT::For, "for"},     {TT::Identifier, "i"}, {TT::Colon, ":"},
//       {TT::Assign, "="},    {TT::Number, "1"},     {TT::To, "to"},
//       {TT::Number, "10"},   {TT::Do, "do"},        {TT::Identifier, "a"},
//       {TT::Colon, ":"},     {TT::Assign, "="},     {TT::Identifier, "i"},
//       {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
//   AST expected_ast{};
//   expected_ast.root = make_unique<pascal::Program>(
//       "test", make_unique<pascal::Block>(
//                   vector<unique_ptr<pascal::Declaration>>{},
//                   vector<unique_ptr<pascal::Statement>>{
//                       make_unique<pascal::ForStmt>(
//                           make_unique<pascal::AssignStmt>(
//                               make_unique<pascal::VariableExpr>("i"),
//                               make_unique<pascal::LiteralExpr>("1")),
//                           false, make_unique<pascal::LiteralExpr>("10"),
//                           make_unique<pascal::AssignStmt>(
//                               make_unique<pascal::VariableExpr>("a"),
//                               make_unique<pascal::VariableExpr>("i")))}));
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
//                              "    cmp    rax, 10\n"
//                              "    jg     L2\n"
//                              "    mov    [a], rax\n"
//                              "    add    qword [i], 1\n"
//                              "    jmp    L1\n"
//                              "L2:\n"
//                              "    ret\n";
//   std::string expected_output = "";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm,
//            expected_output);
// }
//
// TEST(ControlTests, RepeatStmt) {
//   std::string input_str = "repeat a:=a-1 until a=0;";
//   vector<Token> expected_tokens = {
//       {TT::Repeat, "repeat"}, {TT::Identifier, "a"}, {TT::Colon, ":"},
//       {TT::Assign, "="},      {TT::Identifier, "a"}, {TT::Minus, "-"},
//       {TT::Number, "1"},      {TT::Until, "until"},  {TT::Identifier, "a"},
//       {TT::Equal, "="},       {TT::Number, "0"},     {TT::Semicolon, ";"},
//       {TT::EndOfFile, ""}};
//   AST expected_ast{};
//   expected_ast.root = make_unique<pascal::Program>(
//       "test",
//       make_unique<pascal::Block>(
//           vector<unique_ptr<pascal::Declaration>>{},
//           vector<unique_ptr<pascal::Statement>>{
//               make_unique<pascal::RepeatStmt>(
//                   vector<unique_ptr<pascal::Statement>>{
//                       make_unique<pascal::AssignStmt>(
//                           make_unique<pascal::VariableExpr>("a"),
//                           make_unique<pascal::BinaryExpr>(
//                               make_unique<pascal::VariableExpr>("a"),
//                               "-",
//                               make_unique<pascal::LiteralExpr>("1")))},
//                   make_unique<pascal::BinaryExpr>(
//                       make_unique<pascal::VariableExpr>("a"), "=",
//                       make_unique<pascal::LiteralExpr>("0")))}));
//   expected_ast.valid = true;
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
//   std::string expected_output = "";
//   run_full(input_str, expected_tokens, expected_ast, expected_asm,
//            expected_output);
// }
//
// // Unsigned int
// TEST(UnsignedTests, Uns1) {
//   vector<Token> expected_tokens = {
//       {TT::Var, "var"},     {TT::Identifier, "u"},
//       {TT::Colon, ":"},     {TT::Identifier, "unsigned"},
//       {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
//   AST expected_ast{};
//   expected_ast.root = make_unique<pascal::Program>(
//       "test", make_unique<pascal::Block>(
//                   vector<unique_ptr<pascal::Declaration>>{
//                       make_unique<pascal::VarDecl>(
//                           vector<std::string>{"u"},
//                           make_unique<pascal::SimpleTypeSpec>(
//                               pascal::BasicType::UnsignedInt, "unsigned"))},
//                   vector<unique_ptr<pascal::Statement>>{}));
//   expected_ast.valid = true;
//   std::string expected_asm = "section .text";
//   std::string expected_output = "";
//   run_full("var u: unsigned;", expected_tokens, expected_ast, expected_asm,
//            expected_output);
// }
//
// TEST(UnsignedTests, Uns2) {
//   vector<Token> expected_tokens = {
//       {TT::Identifier, "u"}, {TT::Colon, ":"},     {TT::Assign, "="},
//       {TT::Number, "1"},     {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
//   AST expected_ast{};
//   expected_ast.root = make_unique<pascal::Program>(
//       "test", make_unique<pascal::Block>(
//                   vector<unique_ptr<pascal::Declaration>>{},
//                   vector<unique_ptr<pascal::Statement>>{
//                       make_unique<pascal::AssignStmt>(
//                           make_unique<pascal::VariableExpr>("u"),
//                           make_unique<pascal::LiteralExpr>("1"))}));
//   expected_ast.valid = true;
//   std::string expected_asm = "section .text";
//   std::string expected_output = "";
//   run_full("u:=1;", expected_tokens, expected_ast, expected_asm,
//            expected_output);
// }
//
// TEST(UnsignedTests, Uns3) {
//   vector<Token> expected_tokens = {
//       {TT::While, "while"}, {TT::Identifier, "u"}, {TT::Greater, ">"},
//       {TT::Number, "0"},    {TT::Do, "do"},        {TT::Identifier, "u"},
//       {TT::Colon, ":"},     {TT::Assign, "="},     {TT::Identifier, "u"},
//       {TT::Minus, "-"},     {TT::Number, "1"},     {TT::Semicolon, ";"},
//       {TT::EndOfFile, ""}};
//   AST expected_ast{};
//   expected_ast.root = make_unique<pascal::Program>(
//       "test",
//       make_unique<pascal::Block>(
//           vector<unique_ptr<pascal::Declaration>>{},
//           vector<unique_ptr<pascal::Statement>>{
//               make_unique<pascal::WhileStmt>(
//                   make_unique<pascal::BinaryExpr>(
//                       make_unique<pascal::VariableExpr>("u"), ">",
//                       make_unique<pascal::LiteralExpr>("0")),
//                   make_unique<pascal::AssignStmt>(
//                       make_unique<pascal::VariableExpr>("u"),
//                       make_unique<pascal::BinaryExpr>(
//                           make_unique<pascal::VariableExpr>("u"), "-",
//                           make_unique<pascal::LiteralExpr>("1"))))}));
//   expected_ast.valid = true;
//   std::string expected_asm = "section .text";
//   std::string expected_output = "";
//   run_full("while u>0 do u:=u-1;", expected_tokens, expected_ast,
//   expected_asm,
//            expected_output);
// }
//
// TEST(UnsignedTests, Uns4) {
//   vector<Token> expected_tokens = {
//       {TT::Function, "function"}, {TT::Identifier, "f"},
//       {TT::Colon, ":"},           {TT::Identifier, "unsigned"},
//       {TT::Semicolon, ";"},       {TT::Begin, "begin"},
//       {TT::Identifier, "f"},      {TT::Colon, ":"},
//       {TT::Assign, "="},          {TT::Number, "0"},
//       {TT::Semicolon, ";"},       {TT::End, "end"},
//       {TT::Semicolon, ";"},       {TT::EndOfFile, ""}};
//   AST expected_ast{};
//   expected_ast.root = make_unique<pascal::Program>(
//       "test",
//       make_unique<pascal::Block>(
//           vector<unique_ptr<pascal::Declaration>>{
//               make_unique<pascal::FunctionDecl>(
//                   "f", vector<unique_ptr<pascal::ParamDecl>>{},
//                   make_unique<pascal::SimpleTypeSpec>(
//                       pascal::BasicType::UnsignedInt, "unsigned"),
//                   make_unique<pascal::Block>(
//                       vector<unique_ptr<pascal::Declaration>>{},
//                       vector<unique_ptr<pascal::Statement>>{
//                           make_unique<pascal::AssignStmt>(
//                               make_unique<pascal::VariableExpr>("f"),
//                               make_unique<pascal::LiteralExpr>("0"))}))},
//           vector<unique_ptr<pascal::Statement>>{}));
//   expected_ast.valid = true;
//   std::string expected_asm = "section .text";
//   std::string expected_output = "";
//   run_full("function f: unsigned; begin f:=0; end;", expected_tokens,
//            expected_ast, expected_asm, expected_output);
// }
//
// TEST(UnsignedTests, Uns5) {
//   vector<Token> expected_tokens = {
//       {TT::For, "for"},     {TT::Identifier, "u"}, {TT::Colon, ":"},
//       {TT::Assign, "="},    {TT::Number, "1"},     {TT::To, "to"},
//       {TT::Number, "5"},    {TT::Do, "do"},        {TT::Identifier, "u"},
//       {TT::Colon, ":"},     {TT::Assign, "="},     {TT::Identifier, "u"},
//       {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
//   AST expected_ast{};
//   expected_ast.root = make_unique<pascal::Program>(
//       "test", make_unique<pascal::Block>(
//                   vector<unique_ptr<pascal::Declaration>>{},
//                   vector<unique_ptr<pascal::Statement>>{
//                       make_unique<pascal::ForStmt>(
//                           make_unique<pascal::AssignStmt>(
//                               make_unique<pascal::VariableExpr>("u"),
//                               make_unique<pascal::LiteralExpr>("1")),
//                           false, make_unique<pascal::LiteralExpr>("5"),
//                           make_unique<pascal::AssignStmt>(
//                               make_unique<pascal::VariableExpr>("u"),
//                               make_unique<pascal::VariableExpr>("u")))}));
//   expected_ast.valid = true;
//   std::string expected_asm = "section .text";
//   std::string expected_output = "";
//   run_full("for u:=1 to 5 do u:=u;", expected_tokens, expected_ast,
//            expected_asm, expected_output);
// }
//
// // Long int
// TEST(LongIntTests, Long1) {
//   vector<Token> expected_tokens = {
//       {TT::Var, "var"},     {TT::Identifier, "l"},
//       {TT::Colon, ":"},     {TT::Identifier, "longint"},
//       {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
//   AST expected_ast{};
//   expected_ast.root = make_unique<pascal::Program>(
//       "test", make_unique<pascal::Block>(
//                   vector<unique_ptr<pascal::Declaration>>{
//                       make_unique<pascal::VarDecl>(
//                           vector<std::string>{"l"},
//                           make_unique<pascal::SimpleTypeSpec>(
//                               pascal::BasicType::LongInt, "longint"))},
//                   vector<unique_ptr<pascal::Statement>>{}));
//   expected_ast.valid = true;
//   std::string expected_asm = "section .text";
//   std::string expected_output = "";
//   run_full("var l: longint;", expected_tokens, expected_ast, expected_asm,
//            expected_output);
// }
//
// TEST(LongIntTests, Long2) {
//   vector<Token> expected_tokens = {
//       {TT::Identifier, "l"}, {TT::Colon, ":"},     {TT::Assign, "="},
//       {TT::Number, "1"},     {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
//   AST expected_ast{};
//   expected_ast.root = make_unique<pascal::Program>(
//       "test", make_unique<pascal::Block>(
//                   vector<unique_ptr<pascal::Declaration>>{},
//                   vector<unique_ptr<pascal::Statement>>{
//                       make_unique<pascal::AssignStmt>(
//                           make_unique<pascal::VariableExpr>("l"),
//                           make_unique<pascal::LiteralExpr>("1"))}));
//   expected_ast.valid = true;
//   std::string expected_asm = "section .text";
//   std::string expected_output = "";
//   run_full("l:=1;", expected_tokens, expected_ast, expected_asm,
//            expected_output);
// }
//
// TEST(LongIntTests, Long3) {
//   vector<Token> expected_tokens = {
//       {TT::While, "while"}, {TT::Identifier, "l"}, {TT::Greater, ">"},
//       {TT::Number, "0"},    {TT::Do, "do"},        {TT::Identifier, "l"},
//       {TT::Colon, ":"},     {TT::Assign, "="},     {TT::Identifier, "l"},
//       {TT::Minus, "-"},     {TT::Number, "1"},     {TT::Semicolon, ";"},
//       {TT::EndOfFile, ""}};
//   AST expected_ast{};
//   expected_ast.root = make_unique<pascal::Program>(
//       "test",
//       make_unique<pascal::Block>(
//           vector<unique_ptr<pascal::Declaration>>{},
//           vector<unique_ptr<pascal::Statement>>{
//               make_unique<pascal::WhileStmt>(
//                   make_unique<pascal::BinaryExpr>(
//                       make_unique<pascal::VariableExpr>("l"), ">",
//                       make_unique<pascal::LiteralExpr>("0")),
//                   make_unique<pascal::AssignStmt>(
//                       make_unique<pascal::VariableExpr>("l"),
//                       make_unique<pascal::BinaryExpr>(
//                           make_unique<pascal::VariableExpr>("l"), "-",
//                           make_unique<pascal::LiteralExpr>("1"))))}));
//   expected_ast.valid = true;
//   std::string expected_asm = "section .text";
//   std::string expected_output = "";
//   run_full("while l>0 do l:=l-1;", expected_tokens, expected_ast,
//   expected_asm,
//            expected_output);
// }
//
// TEST(LongIntTests, Long4) {
//   vector<Token> expected_tokens = {
//       {TT::Function, "function"},  {TT::Identifier, "f"}, {TT::Colon, ":"},
//       {TT::Identifier, "longint"}, {TT::Semicolon, ";"},  {TT::Begin,
//       "begin"}, {TT::Identifier, "f"},       {TT::Colon, ":"}, {TT::Assign,
//       "="}, {TT::Number, "0"},           {TT::Semicolon, ";"},  {TT::End,
//       "end"}, {TT::Semicolon, ";"},        {TT::EndOfFile, ""}};
//   AST expected_ast{};
//   expected_ast.root = make_unique<pascal::Program>(
//       "test",
//       make_unique<pascal::Block>(
//           vector<unique_ptr<pascal::Declaration>>{
//               make_unique<pascal::FunctionDecl>(
//                   "f", vector<unique_ptr<pascal::ParamDecl>>{},
//                   make_unique<pascal::SimpleTypeSpec>(
//                       pascal::BasicType::LongInt, "longint"),
//                   make_unique<pascal::Block>(
//                       vector<unique_ptr<pascal::Declaration>>{},
//                       vector<unique_ptr<pascal::Statement>>{
//                           make_unique<pascal::AssignStmt>(
//                               make_unique<pascal::VariableExpr>("f"),
//                               make_unique<pascal::LiteralExpr>("0"))}))},
//           vector<unique_ptr<pascal::Statement>>{}));
//   expected_ast.valid = true;
//   std::string expected_asm = "section .text";
//   std::string expected_output = "";
//   run_full("function f: longint; begin f:=0; end;", expected_tokens,
//            expected_ast, expected_asm, expected_output);
// }
//
// TEST(LongIntTests, Long5) {
//   vector<Token> expected_tokens = {
//       {TT::For, "for"},     {TT::Identifier, "l"}, {TT::Colon, ":"},
//       {TT::Assign, "="},    {TT::Number, "1"},     {TT::To, "to"},
//       {TT::Number, "5"},    {TT::Do, "do"},        {TT::Identifier, "l"},
//       {TT::Colon, ":"},     {TT::Assign, "="},     {TT::Identifier, "l"},
//       {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
//   AST expected_ast{};
//   expected_ast.root = make_unique<pascal::Program>(
//       "test", make_unique<pascal::Block>(
//                   vector<unique_ptr<pascal::Declaration>>{},
//                   vector<unique_ptr<pascal::Statement>>{
//                       make_unique<pascal::ForStmt>(
//                           make_unique<pascal::AssignStmt>(
//                               make_unique<pascal::VariableExpr>("l"),
//                               make_unique<pascal::LiteralExpr>("1")),
//                           false, make_unique<pascal::LiteralExpr>("5"),
//                           make_unique<pascal::AssignStmt>(
//                               make_unique<pascal::VariableExpr>("l"),
//                               make_unique<pascal::VariableExpr>("l")))}));
//   expected_ast.valid = true;
//   std::string expected_asm = "section .text";
//   std::string expected_output = "";
//   run_full("for l:=1 to 5 do l:=l;", expected_tokens, expected_ast,
//            expected_asm, expected_output);
// }
//
// // Functions
// TEST(FunctionTests, Func1) {
//     std::string input_str = "function f: integer; begin f:=0; end;";
//     vector<Token> expected_tokens = {
//         {TT::Function, "function"},  {TT::Identifier, "f"}, {TT::Colon, ":"},
//         {TT::Identifier, "integer"}, {TT::Semicolon, ";"},  {TT::Begin,
//         "begin"}, {TT::Identifier, "f"},       {TT::Colon, ":"}, {TT::Assign,
//         "="}, {TT::Number, "0"},           {TT::Semicolon, ";"},  {TT::End,
//         "end"}, {TT::Semicolon, ";"},        {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test",
//         make_unique<pascal::Block>(
//             vector<unique_ptr<pascal::Declaration>>{
//                 make_unique<pascal::FunctionDecl>(
//                     "f", vector<unique_ptr<pascal::ParamDecl>>{},
//                     make_unique<pascal::SimpleTypeSpec>(
//                         pascal::BasicType::Integer, "integer"),
//                     make_unique<pascal::Block>(
//                         vector<unique_ptr<pascal::Declaration>>{},
//                         vector<unique_ptr<pascal::Statement>>{
//                             make_unique<pascal::AssignStmt>(
//                                 make_unique<pascal::VariableExpr>("f"),
//                                 make_unique<pascal::LiteralExpr>("0"))}))},
//             vector<unique_ptr<pascal::Statement>>{}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .text\n"
//         "global f\n"
//         "global main\n"
//         "f:\n"
//         "    mov    rax, 0\n"
//         "    ret\n"
//         "main:\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(FunctionTests, Func2) {
//     std::string input_str = "procedure p; begin end;";
//     vector<Token> expected_tokens = {
//         {TT::Procedure, "procedure"}, {TT::Identifier, "p"}, {TT::Semicolon,
//         ";"}, {TT::Begin, "begin"},         {TT::End, "end"}, {TT::Semicolon,
//         ";"}, {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test",
//         make_unique<pascal::Block>(
//             vector<unique_ptr<pascal::Declaration>>{
//                 make_unique<pascal::ProcedureDecl>(
//                     "p", vector<unique_ptr<pascal::ParamDecl>>{},
//                     make_unique<pascal::Block>(
//                         vector<unique_ptr<pascal::Declaration>>{},
//                         vector<unique_ptr<pascal::Statement>>{}))},
//             vector<unique_ptr<pascal::Statement>>{}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .text\n"
//         "global p\n"
//         "global main\n"
//         "p:\n"
//         "    ret\n"
//         "main:\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(FunctionTests, Func3) {
//     std::string input_str = "function g(x: integer): integer; begin g:=x;
//     end;"; vector<Token> expected_tokens = {
//         {TT::Function, "function"}, {TT::Identifier, "g"},
//         {TT::LeftParen, "("},       {TT::Identifier, "x"},
//         {TT::Colon, ":"},           {TT::Identifier, "integer"},
//         {TT::RightParen, ")"},      {TT::Colon, ":"},
//         {TT::Identifier, "integer"},{TT::Semicolon, ";"},
//         {TT::Begin, "begin"},{TT::Identifier, "g"},{TT::Colon, ":"},
//         {TT::Assign, "="},{TT::Identifier, "x"},{TT::Semicolon, ";"},
//         {TT::End, "end"},{TT::Semicolon, ";"},{TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test",
//         make_unique<pascal::Block>(
//             vector<unique_ptr<pascal::Declaration>>{
//                 make_unique<pascal::FunctionDecl>(
//                     "g",
//                     vector<unique_ptr<pascal::ParamDecl>>{
//                         make_unique<pascal::ParamDecl>(
//                             vector<std::string>{"x"},
//                             make_unique<pascal::SimpleTypeSpec>(
//                                 pascal::BasicType::Integer, "integer"))},
//                     make_unique<pascal::SimpleTypeSpec>(
//                         pascal::BasicType::Integer, "integer"),
//                     make_unique<pascal::Block>(
//                         vector<unique_ptr<pascal::Declaration>>{},
//                         vector<unique_ptr<pascal::Statement>>{
//                             make_unique<pascal::AssignStmt>(
//                                 make_unique<pascal::VariableExpr>("g"),
//                                 make_unique<pascal::VariableExpr>("x"))}))},
//             vector<unique_ptr<pascal::Statement>>{}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .text\n"
//         "global g\n"
//         "global main\n"
//         "g:\n"
//         "    mov    rax, rdi\n"
//         "    ret\n"
//         "main:\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// // Float
// TEST(FloatTests, Float1) {
//     std::string input_str = "var x: real; x:=1.0;";
//     vector<Token> expected_tokens = {
//         {TT::Var, "var"},         {TT::Identifier, "x"}, {TT::Colon, ":"},
//         {TT::Identifier, "real"}, {TT::Semicolon, ";"},  {TT::Identifier,
//         "x"}, {TT::Colon, ":"},         {TT::Assign, "="},     {TT::Number,
//         "1"}, {TT::Dot, "."},           {TT::Number, "0"}, {TT::Semicolon,
//         ";"}, {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test", make_unique<pascal::Block>(
//                     vector<unique_ptr<pascal::Declaration>>{
//                         make_unique<pascal::VarDecl>(
//                             vector<std::string>{"x"},
//                             make_unique<pascal::SimpleTypeSpec>(
//                                 pascal::BasicType::Real, "real"))},
//                     vector<unique_ptr<pascal::Statement>>{
//                         make_unique<pascal::AssignStmt>(
//                             make_unique<pascal::VariableExpr>("x"),
//                             make_unique<pascal::LiteralExpr>("1.0"))}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "x:    resq    1\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    mov    qword [x], 0x3FF0000000000000\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(FloatTests, Float2) {
//     std::string input_str = "x:=1.5+2.5;";
//     vector<Token> expected_tokens = {
//         {TT::Identifier, "x"}, {TT::Colon, ":"},     {TT::Assign, "="},
//         {TT::Number, "1"},     {TT::Dot, "."},       {TT::Number, "5"},
//         {TT::Plus, "+"},       {TT::Number, "2"},    {TT::Dot, "."},
//         {TT::Number, "5"},     {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test", make_unique<pascal::Block>(
//                     vector<unique_ptr<pascal::Declaration>>{},
//                     vector<unique_ptr<pascal::Statement>>{
//                         make_unique<pascal::AssignStmt>(
//                             make_unique<pascal::VariableExpr>("x"),
//                             make_unique<pascal::BinaryExpr>(
//                                 make_unique<pascal::LiteralExpr>("1.5"),
//                                 "+",
//                                 make_unique<pascal::LiteralExpr>("2.5")))}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "x:    resq    1\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    mov    qword [x], 0x4010000000000000\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(FloatTests, Float3) {
//     std::string input_str = "if 0.0 < 1.0 then x:=0.0;";
//     vector<Token> expected_tokens = {
//         {TT::If, "if"},        {TT::Number, "0"},  {TT::Dot, "."},
//         {TT::Number, "0"},     {TT::Less, "<"},    {TT::Number, "1"},
//         {TT::Dot, "."},        {TT::Number, "0"},  {TT::Then, "then"},
//         {TT::Identifier, "x"}, {TT::Colon, ":"},   {TT::Assign, "="},
//         {TT::Number, "0"},     {TT::Dot, "."},     {TT::Number, "0"},
//         {TT::Semicolon, ";"},  {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test", make_unique<pascal::Block>(
//                     vector<unique_ptr<pascal::Declaration>>{},
//                     vector<unique_ptr<pascal::Statement>>{
//                         make_unique<pascal::IfStmt>(
//                             make_unique<pascal::BinaryExpr>(
//                                 make_unique<pascal::LiteralExpr>("0.0"),
//                                 "<",
//                                 make_unique<pascal::LiteralExpr>("1.0")),
//                             make_unique<pascal::AssignStmt>(
//                                 make_unique<pascal::VariableExpr>("x"),
//                                 make_unique<pascal::LiteralExpr>("0.0")))}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "x:    resq    1\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    mov    qword [x], 0x0000000000000000\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(FloatTests, Float4) {
//     std::string input_str = "while x < 1.0 do x:=x+0.1;";
//     vector<Token> expected_tokens = {
//         {TT::While, "while"}, {TT::Identifier, "x"}, {TT::Less, "<"},
//         {TT::Number, "1"},    {TT::Dot, "."},        {TT::Number, "0"},
//         {TT::Do, "do"},       {TT::Identifier, "x"}, {TT::Colon, ":"},
//         {TT::Assign, "="},    {TT::Identifier, "x"}, {TT::Plus, "+"},
//         {TT::Number, "0"},    {TT::Dot, "."},        {TT::Number, "1"},
//         {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test",
//         make_unique<pascal::Block>(
//             vector<unique_ptr<pascal::Declaration>>{},
//             vector<unique_ptr<pascal::Statement>>{
//                 make_unique<pascal::WhileStmt>(
//                     make_unique<pascal::BinaryExpr>(
//                         make_unique<pascal::VariableExpr>("x"), "<",
//                         make_unique<pascal::LiteralExpr>("1.0")),
//                     make_unique<pascal::AssignStmt>(
//                         make_unique<pascal::VariableExpr>("x"),
//                         make_unique<pascal::BinaryExpr>(
//                             make_unique<pascal::VariableExpr>("x"), "+",
//                             make_unique<pascal::LiteralExpr>("0.1"))))}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "x:    resq    1\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "L1:\n"
//         "    mov    rax, [x]\n"
//         "    cmp    rax, 0x3FF0000000000000\n"
//         "    jge    L2\n"
//         "    mov    rax, [x]\n"
//         "    add    rax, 0x3FB999999999999A\n"
//         "    mov    [x], rax\n"
//         "    jmp    L1\n"
//         "L2:\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(FloatTests, Float5) {
//     std::string input_str = "function f: real; begin f:=0.0; end;";
//     vector<Token> expected_tokens = {
//         {TT::Function, "function"}, {TT::Identifier, "f"}, {TT::Colon, ":"},
//         {TT::Identifier, "real"},   {TT::Semicolon, ";"},  {TT::Begin,
//         "begin"}, {TT::Identifier, "f"},      {TT::Colon, ":"}, {TT::Assign,
//         "="}, {TT::Number, "0"},          {TT::Dot, "."},        {TT::Number,
//         "0"}, {TT::Semicolon, ";"},       {TT::End, "end"}, {TT::Semicolon,
//         ";"}, {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test",
//         make_unique<pascal::Block>(
//             vector<unique_ptr<pascal::Declaration>>{
//                 make_unique<pascal::FunctionDecl>(
//                     "f", vector<unique_ptr<pascal::ParamDecl>>{},
//                     make_unique<pascal::SimpleTypeSpec>(
//                         pascal::BasicType::Real, "real"),
//                     make_unique<pascal::Block>(
//                         vector<unique_ptr<pascal::Declaration>>{},
//                         vector<unique_ptr<pascal::Statement>>{
//                             make_unique<pascal::AssignStmt>(
//                                 make_unique<pascal::VariableExpr>("f"),
//                                 make_unique<pascal::LiteralExpr>("0.0"))}))},
//             vector<unique_ptr<pascal::Statement>>{}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .text\n"
//         "global f\n"
//         "global main\n"
//         "f:\n"
//         "    mov    rax, 0x0000000000000000\n"
//         "    ret\n"
//         "main:\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// // Dynamic memory
// TEST(DynamicTests, Dyn1) {
//     std::string input_str = "new(p);";
//     vector<Token> expected_tokens = {
//         {TT::New, "new"},      {TT::LeftParen, "("}, {TT::Identifier, "p"},
//         {TT::RightParen, ")"}, {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test",
//         make_unique<pascal::Block>(
//             vector<unique_ptr<pascal::Declaration>>{},
//             vector<unique_ptr<pascal::Statement>>{
//                 make_unique<pascal::ProcCall>(
//                     "new", vector<unique_ptr<pascal::Expression>>{
//                                make_unique<pascal::VariableExpr>("p")})}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "p:    resq    1\n\n"
//         "section .text\n"
//         "extern malloc\n"
//         "global main\n"
//         "main:\n"
//         "    mov    rdi, 8\n"
//         "    call   malloc\n"
//         "    mov    qword [p], rax\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(DynamicTests, Dyn2) {
//     std::string input_str = "dispose(p);";
//     vector<Token> expected_tokens = {
//         {TT::Dispose, "dispose"}, {TT::LeftParen, "("}, {TT::Identifier,
//         "p"}, {TT::RightParen, ")"},    {TT::Semicolon, ";"}, {TT::EndOfFile,
//         ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test",
//         make_unique<pascal::Block>(
//             vector<unique_ptr<pascal::Declaration>>{},
//             vector<unique_ptr<pascal::Statement>>{
//                 make_unique<pascal::ProcCall>(
//                     "dispose",
//                     vector<unique_ptr<pascal::Expression>>{
//                                    make_unique<pascal::VariableExpr>("p")})}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "p:    resq    1\n\n"
//         "section .text\n"
//         "extern free\n"
//         "global main\n"
//         "main:\n"
//         "    mov    rdi, [p]\n"
//         "    call   free\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(DynamicTests, Dyn3) {
//     std::string input_str = "var p:^integer;";
//     vector<Token> expected_tokens = {
//         {TT::Var, "var"},   {TT::Identifier, "p"},       {TT::Colon, ":"},
//         {TT::Caret, "^"},   {TT::Identifier, "integer"}, {TT::Semicolon,
//         ";"}, {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test", make_unique<pascal::Block>(
//                     vector<unique_ptr<pascal::Declaration>>{
//                         make_unique<pascal::VarDecl>(
//                             vector<std::string>{"p"},
//                             make_unique<pascal::PointerTypeSpec>(
//                                 make_unique<pascal::SimpleTypeSpec>(
//                                     pascal::BasicType::Integer,
//                                     "integer")))},
//                     vector<unique_ptr<pascal::Statement>>{}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "p:    resq    1\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(DynamicTests, Dyn4) {
//     std::string input_str = "p^:=1;";
//     vector<Token> expected_tokens = {
//         {TT::Identifier, "p"}, {TT::Caret, "^"},  {TT::Colon, ":"},
//         {TT::Assign, "="},     {TT::Number, "1"}, {TT::Semicolon, ";"},
//         {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     pascal::VariableExpr::Selector ptrSel{
//         "", pascal::VariableExpr::Selector::Kind::Pointer};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test", make_unique<pascal::Block>(
//                     vector<unique_ptr<pascal::Declaration>>{},
//                     vector<unique_ptr<pascal::Statement>>{
//                         make_unique<pascal::AssignStmt>(
//                             make_unique<pascal::VariableExpr>(
//                                 "p", vector{ptrSel}),
//                             make_unique<pascal::LiteralExpr>("1"))}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "p:    resq    1\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    mov    rax, [p]\n"
//         "    mov    qword [rax], 1\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(DynamicTests, Dyn5) {
//     std::string input_str = "if p<>nil then dispose(p);";
//     vector<Token> expected_tokens = {
//         {TT::If, "if"},           {TT::Identifier, "p"},   {TT::Less, "<"},
//         {TT::Greater, ">"},       {TT::Identifier, "nil"}, {TT::Then,
//         "then"}, {TT::Dispose, "dispose"},{TT::LeftParen, "("},
//         {TT::Identifier, "p"}, {TT::RightParen, ")"},    {TT::Semicolon,
//         ";"},    {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test",
//         make_unique<pascal::Block>(
//             vector<unique_ptr<pascal::Declaration>>{},
//             vector<unique_ptr<pascal::Statement>>{
//                 make_unique<pascal::IfStmt>(
//                     make_unique<pascal::BinaryExpr>(
//                         make_unique<pascal::VariableExpr>("p"), "<>",
//                         make_unique<pascal::VariableExpr>("nil")),
//                     make_unique<pascal::ProcCall>(
//                         "dispose",
//                         vector<unique_ptr<pascal::Expression>>{
//                             make_unique<pascal::VariableExpr>("p")}))}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "p:    resq    1\n\n"
//         "section .text\n"
//         "extern free\n"
//         "global main\n"
//         "main:\n"
//         "    mov    rax, [p]\n"
//         "    cmp    rax, 0\n"
//         "    je     L1\n"
//         "    mov    rdi, rax\n"
//         "    call   free\n"
//         "L1:\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// // Strings
// TEST(StringTests, Str1) {
//     std::string input_str = "var s: string;";
//     vector<Token> expected_tokens = {
//         {TT::Var, "var"},           {TT::Identifier, "s"}, {TT::Colon, ":"},
//         {TT::Identifier, "string"},{TT::Semicolon, ";"},  {TT::EndOfFile,
//         ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test", make_unique<pascal::Block>(
//                     vector<unique_ptr<pascal::Declaration>>{
//                         make_unique<pascal::VarDecl>(
//                             vector<std::string>{"s"},
//                             make_unique<pascal::SimpleTypeSpec>(
//                                 pascal::BasicType::String, "string"))},
//                     vector<unique_ptr<pascal::Statement>>{}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "s:    resq    1\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(StringTests, Str2) {
//     std::string input_str = "s:='hi';";
//     vector<Token> expected_tokens = {
//         {TT::Identifier, "s"}, {TT::Colon, ":"},       {TT::Assign, "="},
//         {TT::Identifier, "'"}, {TT::Identifier, "hi"},{TT::Identifier, "'"},
//         {TT::Semicolon, ";"},  {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test", make_unique<pascal::Block>(
//                     vector<unique_ptr<pascal::Declaration>>{},
//                     vector<unique_ptr<pascal::Statement>>{
//                         make_unique<pascal::AssignStmt>(
//                             make_unique<pascal::VariableExpr>("s"),
//                             make_unique<pascal::LiteralExpr>("'hi'"))}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .data\n"
//         "str0: db \"hi\", 0\n\n"
//         "section .bss\n"
//         "s:    resq    1\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    mov    qword [s], str0\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(StringTests, Str3) {
//     std::string input_str = "s:=s+'!';";
//     vector<Token> expected_tokens = {
//         {TT::Identifier, "s"}, {TT::Colon, ":"},     {TT::Assign, "="},
//         {TT::Identifier, "s"},{TT::Plus, "+"},       {TT::Identifier, "'"},
//         {TT::Identifier, "!"},{TT::Identifier, "'"}, {TT::Semicolon, ";"},
//         {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test", make_unique<pascal::Block>(
//                     vector<unique_ptr<pascal::Declaration>>{},
//                     vector<unique_ptr<pascal::Statement>>{
//                         make_unique<pascal::AssignStmt>(
//                             make_unique<pascal::VariableExpr>("s"),
//                             make_unique<pascal::BinaryExpr>(
//                                 make_unique<pascal::VariableExpr>("s"),
//                                 "+",
//                                 make_unique<pascal::LiteralExpr>("'!'")))}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .data\n"
//         "str1: db \"!\", 0\n\n"
//         "section .bss\n"
//         "s:    resq    1\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    mov    qword [s], str1\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(StringTests, Str4) {
//     std::string input_str = "writeln(s);";
//     vector<Token> expected_tokens = {
//         {TT::Identifier, "writeln"},{TT::LeftParen, "("}, {TT::Identifier,
//         "s"}, {TT::RightParen, ")"},      {TT::Semicolon, ";"},
//         {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test", make_unique<pascal::Block>(
//                     vector<unique_ptr<pascal::Declaration>>{},
//                     vector<unique_ptr<pascal::Statement>>{
//                         make_unique<pascal::ProcCall>(
//                             "writeln",
//                             vector<unique_ptr<pascal::Expression>>{
//                                 make_unique<pascal::VariableExpr>("s")})}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "s:    resq    1\n\n"
//         "section .text\n"
//         "extern puts\n"
//         "global main\n"
//         "main:\n"
//         "    mov    rdi, [s]\n"
//         "    call   puts\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(StringTests, Str5) {
//     std::string input_str = "if s='' then s:='a';";
//     vector<Token> expected_tokens = {
//         {TT::If, "if"},        {TT::Identifier, "s"}, {TT::Equal, "="},
//         {TT::Identifier, "'"}, {TT::Identifier, "'"}, {TT::Then, "then"},
//         {TT::Identifier, "s"},{TT::Colon, ":"},      {TT::Assign, "="},
//         {TT::Identifier, "'"}, {TT::Identifier, "a"},{TT::Identifier, "'"},
//         {TT::Semicolon, ";"},{TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test", make_unique<pascal::Block>(
//                     vector<unique_ptr<pascal::Declaration>>{},
//                     vector<unique_ptr<pascal::Statement>>{
//                         make_unique<pascal::IfStmt>(
//                             make_unique<pascal::BinaryExpr>(
//                                 make_unique<pascal::VariableExpr>("s"),
//                                 "=",
//                                 make_unique<pascal::LiteralExpr>("''")),
//                             make_unique<pascal::AssignStmt>(
//                                 make_unique<pascal::VariableExpr>("s"),
//                                 make_unique<pascal::LiteralExpr>("'a'")))}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .data\n"
//         "str2: db 0\n"
//         "str3: db \"a\", 0\n\n"
//         "section .bss\n"
//         "s:    resq    1\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    mov    rax, [s]\n"
//         "    cmp    rax, str2\n"
//         "    jne    L1\n"
//         "    mov    qword [s], str3\n"
//         "L1:\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
//
// // Arrays
// TEST(ArrayTests, Arr1) {
//     std::string input_str = "var a: array[1..5] of integer;";
//     vector<Token> expected_tokens = {
//         {TT::Var, "var"},       {TT::Identifier, "a"},
//         {TT::Colon, ":"},       {TT::Array, "array"},
//         {TT::LeftBracket, "["}, {TT::Number, "1"},
//         {TT::Dot, "."},         {TT::Dot, "."},
//         {TT::Number, "5"},      {TT::RightBracket, "]"},
//         {TT::Of, "of"},         {TT::Identifier, "integer"},
//         {TT::Semicolon, ";"},   {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test", make_unique<pascal::Block>(
//                     vector<unique_ptr<pascal::Declaration>>{
//                         make_unique<pascal::VarDecl>(
//                             vector<std::string>{"a"},
//                             make_unique<pascal::ArrayTypeSpec>(
//                                 vector<pascal::Range>{pascal::Range(1,
//                                 5)},
//                                 make_unique<pascal::SimpleTypeSpec>(
//                                     pascal::BasicType::Integer,
//                                     "integer")))},
//                     vector<unique_ptr<pascal::Statement>>{}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "a:    resq    5\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(ArrayTests, Arr2) {
//     std::string input_str = "a[1]:=0;";
//     vector<Token> expected_tokens = {
//         {TT::Identifier, "a"},   {TT::LeftBracket, "["}, {TT::Number, "1"},
//         {TT::RightBracket, "]"}, {TT::Colon, ":"},       {TT::Assign, "="},
//         {TT::Number, "0"},       {TT::Semicolon, ";"},   {TT::EndOfFile,
//         ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test",
//         make_unique<pascal::Block>(
//             vector<unique_ptr<pascal::Declaration>>{},
//             vector<unique_ptr<pascal::Statement>>{
//                 make_unique<pascal::AssignStmt>(
//                     make_unique<pascal::VariableExpr>(
//                         "a",
//                         vector<pascal::VariableExpr::Selector>{
//                             pascal::VariableExpr::Selector(
//                                 make_unique<pascal::LiteralExpr>("1"))}),
//                     make_unique<pascal::LiteralExpr>("0"))}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "a:    resq    5\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    mov    qword [a], 0\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(ArrayTests, Arr3) {
//     std::string input_str = "for i:=1 to 5 do a[i]:=i;";
//     vector<Token> expected_tokens = {
//         {TT::For, "for"},       {TT::Identifier, "i"}, {TT::Colon, ":"},
//         {TT::Assign, "="},      {TT::Number, "1"},     {TT::To, "to"},
//         {TT::Number, "5"},      {TT::Do, "do"},        {TT::Identifier, "a"},
//         {TT::LeftBracket, "["}, {TT::Identifier, "i"}, {TT::RightBracket,
//         "]"}, {TT::Colon, ":"},       {TT::Assign, "="},     {TT::Identifier,
//         "i"}, {TT::Semicolon, ";"},   {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test",
//         make_unique<pascal::Block>(
//             vector<unique_ptr<pascal::Declaration>>{},
//             vector<unique_ptr<pascal::Statement>>{
//                 make_unique<pascal::ForStmt>(
//                     make_unique<pascal::AssignStmt>(
//                         make_unique<pascal::VariableExpr>("i"),
//                         make_unique<pascal::LiteralExpr>("1")),
//                     false, make_unique<pascal::LiteralExpr>("5"),
//                     make_unique<pascal::AssignStmt>(
//                         make_unique<pascal::VariableExpr>(
//                             "a",
//                             vector<pascal::VariableExpr::Selector>{
//                                 pascal::VariableExpr::Selector(
//                                     make_unique<pascal::VariableExpr>(
//                                         "i"))}),
//                         make_unique<pascal::VariableExpr>("i")))}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "i:    resq    1\n"
//         "a:    resq    5\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    mov    qword [i], 1\n"
//         "L1:\n"
//         "    mov    rax, [i]\n"
//         "    cmp    rax, 5\n"
//         "    jg     L2\n"
//         "    mov    rcx, rax\n"
//         "    sub    rcx, 1\n"
//         "    imul   rcx, 8\n"
//         "    lea    rcx, [a + rcx]\n"
//         "    mov    [rcx], rax\n"
//         "    add    qword [i], 1\n"
//         "    jmp    L1\n"
//         "L2:\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(ArrayTests, Arr4) {
//     std::string input_str = "writeln(a[1]);";
//     vector<Token> expected_tokens = {
//         {TT::Identifier, "writeln"}, {TT::LeftParen, "("},
//         {TT::Identifier, "a"},       {TT::LeftBracket, "["},
//         {TT::Number, "1"},           {TT::RightBracket, "]"},
//         {TT::RightParen, ")"},       {TT::Semicolon, ";"},
//         {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test",
//         make_unique<pascal::Block>(
//             vector<unique_ptr<pascal::Declaration>>{},
//             vector<unique_ptr<pascal::Statement>>{
//                 make_unique<pascal::ProcCall>(
//                     "writeln",
//                     vector<unique_ptr<pascal::Expression>>{
//                         make_unique<pascal::VariableExpr>(
//                             "a", vector<pascal::VariableExpr::Selector>{
//                                      pascal::VariableExpr::Selector(
//                                          make_unique<
//                                              pascal::LiteralExpr>("1"))})})}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "a:    resq    5\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(ArrayTests, Arr5) {
//     std::string input_str = "if a[1]=0 then a[1]:=1;";
//     vector<Token> expected_tokens = {
//         {TT::If, "if"},         {TT::Identifier, "a"},   {TT::LeftBracket,
//         "["}, {TT::Number, "1"},      {TT::RightBracket, "]"}, {TT::Equal,
//         "="}, {TT::Number, "0"},      {TT::Then, "then"}, {TT::Identifier,
//         "a"}, {TT::LeftBracket, "["}, {TT::Number, "1"}, {TT::RightBracket,
//         "]"}, {TT::Colon, ":"},       {TT::Assign, "="},       {TT::Number,
//         "1"}, {TT::Semicolon, ";"},   {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test",
//         make_unique<pascal::Block>(
//             vector<unique_ptr<pascal::Declaration>>{},
//             vector<unique_ptr<pascal::Statement>>{
//                 make_unique<pascal::IfStmt>(
//                     make_unique<pascal::BinaryExpr>(
//                         make_unique<pascal::VariableExpr>(
//                             "a",
//                             vector<pascal::VariableExpr::Selector>{
//                                 pascal::VariableExpr::Selector(
//                                     make_unique<pascal::LiteralExpr>("1"))}),
//                         "=", make_unique<pascal::LiteralExpr>("0")),
//                     make_unique<pascal::AssignStmt>(
//                         make_unique<pascal::VariableExpr>(
//                             "a",
//                             vector<pascal::VariableExpr::Selector>{
//                                 pascal::VariableExpr::Selector(
//                                     make_unique<pascal::LiteralExpr>("1"))}),
//                         make_unique<pascal::LiteralExpr>("1")))}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "a:    resq    5\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    mov    rax, [a]\n"
//         "    cmp    rax, 0\n"
//         "    jne    L1\n"
//         "    mov    qword [a], 1\n"
//         "L1:\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// // Struct (record)
// TEST(StructTests, Rec1) {
//     std::string input_str = "type r=record a:integer; end;";
//     vector<Token> expected_tokens = {
//         {TT::Type, "type"},          {TT::Identifier, "r"}, {TT::Equal, "="},
//         {TT::Record, "record"},      {TT::Identifier, "a"}, {TT::Colon, ":"},
//         {TT::Identifier, "integer"}, {TT::Semicolon, ";"},  {TT::End, "end"},
//         {TT::Semicolon, ";"},        {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test",
//         make_unique<pascal::Block>(
//             vector<unique_ptr<pascal::Declaration>>{
//                 make_unique<pascal::TypeDecl>(
//                     "r",
//                     make_unique<pascal::RecordTypeSpec>(
//                         vector<unique_ptr<pascal::VarDecl>>{
//                             make_unique<pascal::VarDecl>(
//                                 vector<std::string>{"a"},
//                                 make_unique<pascal::SimpleTypeSpec>(
//                                     pascal::BasicType::Integer,
//                                     "integer"))}))},
//             vector<unique_ptr<pascal::Statement>>{}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(StructTests, Rec2) {
//     std::string input_str = "var v:r;";
//     vector<Token> expected_tokens = {
//         {TT::Var, "var"},      {TT::Identifier, "v"}, {TT::Colon, ":"},
//         {TT::Identifier, "r"}, {TT::Semicolon, ";"},  {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test", make_unique<pascal::Block>(
//                     vector<unique_ptr<pascal::Declaration>>{
//                         make_unique<pascal::VarDecl>(
//                             vector<std::string>{"v"},
//                             make_unique<pascal::SimpleTypeSpec>(
//                                 pascal::BasicType::Integer, "r"))},
//                     vector<unique_ptr<pascal::Statement>>{}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "v:    resq    1\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(StructTests, Rec3) {
//     std::string input_str = "v.a:=1;";
//     vector<Token> expected_tokens = {
//         {TT::Identifier, "v"}, {TT::Dot, "."},     {TT::Identifier, "a"},
//         {TT::Colon, ":"},      {TT::Assign, "="},  {TT::Number, "1"},
//         {TT::Semicolon, ";"},  {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     pascal::VariableExpr::Selector fieldSel(
//         "a", pascal::VariableExpr::Selector::Kind::Field);
//     expected_ast.root = make_unique<pascal::Program>(
//         "test", make_unique<pascal::Block>(
//                     vector<unique_ptr<pascal::Declaration>>{},
//                     vector<unique_ptr<pascal::Statement>>{
//                         make_unique<pascal::AssignStmt>(
//                             make_unique<pascal::VariableExpr>(
//                                 "v", vector{fieldSel}),
//                             make_unique<pascal::LiteralExpr>("1"))}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "v:    resq    1\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    mov    qword [v], 1\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(StructTests, Rec4) {
//     std::string input_str = "with v do a:=2;";
//     vector<Token> expected_tokens = {
//         {TT::With, "with"},    {TT::Identifier, "v"}, {TT::Do, "do"},
//         {TT::Identifier, "a"}, {TT::Colon, ":"},      {TT::Assign, "="},
//         {TT::Number, "2"},     {TT::Semicolon, ";"},  {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test", make_unique<pascal::Block>(
//                     vector<unique_ptr<pascal::Declaration>>{},
//                     vector<unique_ptr<pascal::Statement>>{
//                         make_unique<pascal::WithStmt>(
//                             make_unique<pascal::VariableExpr>("v"),
//                             make_unique<pascal::AssignStmt>(
//                                 make_unique<pascal::VariableExpr>("a"),
//                                 make_unique<pascal::LiteralExpr>("2")))}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "v:    resq    1\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    mov    qword [v], 2\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(StructTests, Rec5) {
//     std::string input_str = "if v.a=0 then v.a:=1;";
//     vector<Token> expected_tokens = {
//         {TT::If, "if"},        {TT::Identifier, "v"}, {TT::Dot, "."},
//         {TT::Identifier, "a"}, {TT::Equal, "="},      {TT::Number, "0"},
//         {TT::Then, "then"},    {TT::Identifier, "v"}, {TT::Dot, "."},
//         {TT::Identifier, "a"}, {TT::Colon, ":"},      {TT::Assign, "="},
//         {TT::Number, "1"},     {TT::Semicolon, ";"},  {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     pascal::VariableExpr::Selector fieldSel(
//         "a", pascal::VariableExpr::Selector::Kind::Field);
//     expected_ast.root = make_unique<pascal::Program>(
//         "test", make_unique<pascal::Block>(
//                     vector<unique_ptr<pascal::Declaration>>{},
//                     vector<unique_ptr<pascal::Statement>>{
//                         make_unique<pascal::IfStmt>(
//                             make_unique<pascal::BinaryExpr>(
//                                 make_unique<pascal::VariableExpr>(
//                                     "v", vector{fieldSel}),
//                                 "=",
//                                 make_unique<pascal::LiteralExpr>("0")),
//                             make_unique<pascal::AssignStmt>(
//                                 make_unique<pascal::VariableExpr>(
//                                     "v", vector{fieldSel}),
//                                 make_unique<pascal::LiteralExpr>("1")))}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "v:    resq    1\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    mov    rax, [v]\n"
//         "    cmp    rax, 0\n"
//         "    jne    L1\n"
//         "    mov    qword [v], 1\n"
//         "L1:\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// // Pointers
// TEST(PointerTests, Ptr1) {
//     std::string input_str = "var p:^integer;";
//     vector<Token> expected_tokens = {
//         {TT::Var, "var"},   {TT::Identifier, "p"},       {TT::Colon, ":"},
//         {TT::Caret, "^"},   {TT::Identifier, "integer"}, {TT::Semicolon,
//         ";"}, {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test", make_unique<pascal::Block>(
//                     vector<unique_ptr<pascal::Declaration>>{
//                         make_unique<pascal::VarDecl>(
//                             vector<std::string>{"p"},
//                             make_unique<pascal::PointerTypeSpec>(
//                                 make_unique<pascal::SimpleTypeSpec>(
//                                     pascal::BasicType::Integer,
//                                     "integer")))},
//                     vector<unique_ptr<pascal::Statement>>{}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "p:    resq    1\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(PointerTests, Ptr2) {
//     std::string input_str = "new(p);";
//     vector<Token> expected_tokens = {
//         {TT::New, "new"},      {TT::LeftParen, "("}, {TT::Identifier, "p"},
//         {TT::RightParen, ")"}, {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test",
//         make_unique<pascal::Block>(
//             vector<unique_ptr<pascal::Declaration>>{},
//             vector<unique_ptr<pascal::Statement>>{
//                 make_unique<pascal::ProcCall>(
//                     "new", vector<unique_ptr<pascal::Expression>>{
//                                make_unique<pascal::VariableExpr>("p")})}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "p:    resq    1\n\n"
//         "section .text\n"
//         "extern malloc\n"
//         "global main\n"
//         "main:\n"
//         "    mov    rdi, 8\n"
//         "    call   malloc\n"
//         "    mov    qword [p], rax\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(PointerTests, Ptr3) {
//     std::string input_str = "p^:=1;";
//     vector<Token> expected_tokens = {
//         {TT::Identifier, "p"}, {TT::Caret, "^"},  {TT::Colon, ":"},
//         {TT::Assign, "="},     {TT::Number, "1"}, {TT::Semicolon, ";"},
//         {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     pascal::VariableExpr::Selector ptrSel(
//         "", pascal::VariableExpr::Selector::Kind::Pointer);
//     expected_ast.root = make_unique<pascal::Program>(
//         "test", make_unique<pascal::Block>(
//                     vector<unique_ptr<pascal::Declaration>>{},
//                     vector<unique_ptr<pascal::Statement>>{
//                         make_unique<pascal::AssignStmt>(
//                             make_unique<pascal::VariableExpr>(
//                                 "p", vector{ptrSel}),
//                             make_unique<pascal::LiteralExpr>("1"))}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "p:    resq    1\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    mov    rax, [p]\n"
//         "    mov    qword [rax], 1\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(PointerTests, Ptr4) {
//     std::string input_str = "dispose(p);";
//     vector<Token> expected_tokens = {
//         {TT::Dispose, "dispose"}, {TT::LeftParen, "("}, {TT::Identifier,
//         "p"}, {TT::RightParen, ")"},    {TT::Semicolon, ";"}, {TT::EndOfFile,
//         ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test", make_unique<pascal::Block>(
//                     vector<unique_ptr<pascal::Declaration>>{},
//                     vector<unique_ptr<pascal::Statement>>{
//                         make_unique<pascal::ProcCall>(
//                             "dispose",
//                             vector<unique_ptr<pascal::Expression>>{
//                                 make_unique<pascal::VariableExpr>("p")})}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "p:    resq    1\n\n"
//         "section .text\n"
//         "extern free\n"
//         "global main\n"
//         "main:\n"
//         "    mov    rdi, [p]\n"
//         "    call   free\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(PointerTests, Ptr5) {
//     std::string input_str = "p:=nil;";
//     vector<Token> expected_tokens = {
//         {TT::Identifier, "p"},   {TT::Colon, ":"},     {TT::Assign, "="},
//         {TT::Identifier, "nil"},{TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test", make_unique<pascal::Block>(
//                     vector<unique_ptr<pascal::Declaration>>{},
//                     vector<unique_ptr<pascal::Statement>>{
//                         make_unique<pascal::AssignStmt>(
//                             make_unique<pascal::VariableExpr>("p"),
//                             make_unique<pascal::VariableExpr>("nil"))}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "p:    resq    1\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    mov    qword [p], 0\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
// TEST(ArrayTests, Arr1) {
//     std::string input_str = "var a: array[1..5] of integer;";
//     vector<Token> expected_tokens = {
//         {TT::Var, "var"},       {TT::Identifier, "a"},
//         {TT::Colon, ":"},       {TT::Array, "array"},
//         {TT::LeftBracket, "["}, {TT::Number, "1"},
//         {TT::Dot, "."},         {TT::Dot, "."},
//         {TT::Number, "5"},      {TT::RightBracket, "]"},
//         {TT::Of, "of"},         {TT::Identifier, "integer"},
//         {TT::Semicolon, ";"},   {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test", make_unique<pascal::Block>(
//                     vector<unique_ptr<pascal::Declaration>>{
//                         make_unique<pascal::VarDecl>(
//                             vector<std::string>{"a"},
//                             make_unique<pascal::ArrayTypeSpec>(
//                                 vector<pascal::Range>{pascal::Range(1,
//                                 5)},
//                                 make_unique<pascal::SimpleTypeSpec>(
//                                     pascal::BasicType::Integer,
//                                     "integer")))},
//                     vector<unique_ptr<pascal::Statement>>{}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "a:    resq    5\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(ArrayTests, Arr2) {
//     std::string input_str = "a[1]:=0;";
//     vector<Token> expected_tokens = {
//         {TT::Identifier, "a"},   {TT::LeftBracket, "["}, {TT::Number, "1"},
//         {TT::RightBracket, "]"}, {TT::Colon, ":"},       {TT::Assign, "="},
//         {TT::Number, "0"},       {TT::Semicolon, ";"},   {TT::EndOfFile,
//         ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test",
//         make_unique<pascal::Block>(
//             vector<unique_ptr<pascal::Declaration>>{},
//             vector<unique_ptr<pascal::Statement>>{
//                 make_unique<pascal::AssignStmt>(
//                     make_unique<pascal::VariableExpr>(
//                         "a",
//                         vector<pascal::VariableExpr::Selector>{
//                             pascal::VariableExpr::Selector(
//                                 make_unique<pascal::LiteralExpr>("1"))}),
//                     make_unique<pascal::LiteralExpr>("0"))}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "a:    resq    5\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    mov    qword [a], 0\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(ArrayTests, Arr3) {
//     std::string input_str = "for i:=1 to 5 do a[i]:=i;";
//     vector<Token> expected_tokens = {
//         {TT::For, "for"},       {TT::Identifier, "i"}, {TT::Colon, ":"},
//         {TT::Assign, "="},      {TT::Number, "1"},     {TT::To, "to"},
//         {TT::Number, "5"},      {TT::Do, "do"},        {TT::Identifier, "a"},
//         {TT::LeftBracket, "["}, {TT::Identifier, "i"}, {TT::RightBracket,
//         "]"}, {TT::Colon, ":"},       {TT::Assign, "="},     {TT::Identifier,
//         "i"}, {TT::Semicolon, ";"},   {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test",
//         make_unique<pascal::Block>(
//             vector<unique_ptr<pascal::Declaration>>{},
//             vector<unique_ptr<pascal::Statement>>{
//                 make_unique<pascal::ForStmt>(
//                     make_unique<pascal::AssignStmt>(
//                         make_unique<pascal::VariableExpr>("i"),
//                         make_unique<pascal::LiteralExpr>("1")),
//                     false, make_unique<pascal::LiteralExpr>("5"),
//                     make_unique<pascal::AssignStmt>(
//                         make_unique<pascal::VariableExpr>(
//                             "a",
//                             vector<pascal::VariableExpr::Selector>{
//                                 pascal::VariableExpr::Selector(
//                                     make_unique<pascal::VariableExpr>(
//                                         "i"))}),
//                         make_unique<pascal::VariableExpr>("i")))}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "i:    resq    1\n"
//         "a:    resq    5\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    mov    qword [i], 1\n"
//         "L1:\n"
//         "    mov    rax, [i]\n"
//         "    cmp    rax, 5\n"
//         "    jg     L2\n"
//         "    mov    rcx, rax\n"
//         "    sub    rcx, 1\n"
//         "    imul   rcx, 8\n"
//         "    lea    rcx, [a + rcx]\n"
//         "    mov    [rcx], rax\n"
//         "    add    qword [i], 1\n"
//         "    jmp    L1\n"
//         "L2:\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(ArrayTests, Arr4) {
//     std::string input_str = "writeln(a[1]);";
//     vector<Token> expected_tokens = {
//         {TT::Identifier, "writeln"}, {TT::LeftParen, "("},
//         {TT::Identifier, "a"},       {TT::LeftBracket, "["},
//         {TT::Number, "1"},           {TT::RightBracket, "]"},
//         {TT::RightParen, ")"},       {TT::Semicolon, ";"},
//         {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test",
//         make_unique<pascal::Block>(
//             vector<unique_ptr<pascal::Declaration>>{},
//             vector<unique_ptr<pascal::Statement>>{
//                 make_unique<pascal::ProcCall>(
//                     "writeln",
//                     vector<unique_ptr<pascal::Expression>>{
//                         make_unique<pascal::VariableExpr>(
//                             "a", vector<pascal::VariableExpr::Selector>{
//                                      pascal::VariableExpr::Selector(
//                                          make_unique<
//                                              pascal::LiteralExpr>("1"))})})}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "a:    resq    5\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(ArrayTests, Arr5) {
//     std::string input_str = "if a[1]=0 then a[1]:=1;";
//     vector<Token> expected_tokens = {
//         {TT::If, "if"},         {TT::Identifier, "a"},   {TT::LeftBracket,
//         "["}, {TT::Number, "1"},      {TT::RightBracket, "]"}, {TT::Equal,
//         "="}, {TT::Number, "0"},      {TT::Then, "then"}, {TT::Identifier,
//         "a"}, {TT::LeftBracket, "["}, {TT::Number, "1"}, {TT::RightBracket,
//         "]"}, {TT::Colon, ":"},       {TT::Assign, "="},       {TT::Number,
//         "1"}, {TT::Semicolon, ";"},   {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test",
//         make_unique<pascal::Block>(
//             vector<unique_ptr<pascal::Declaration>>{},
//             vector<unique_ptr<pascal::Statement>>{
//                 make_unique<pascal::IfStmt>(
//                     make_unique<pascal::BinaryExpr>(
//                         make_unique<pascal::VariableExpr>(
//                             "a",
//                             vector<pascal::VariableExpr::Selector>{
//                                 pascal::VariableExpr::Selector(
//                                     make_unique<pascal::LiteralExpr>("1"))}),
//                         "=", make_unique<pascal::LiteralExpr>("0")),
//                     make_unique<pascal::AssignStmt>(
//                         make_unique<pascal::VariableExpr>(
//                             "a",
//                             vector<pascal::VariableExpr::Selector>{
//                                 pascal::VariableExpr::Selector(
//                                     make_unique<pascal::LiteralExpr>("1"))}),
//                         make_unique<pascal::LiteralExpr>("1")))}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "a:    resq    5\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    mov    rax, [a]\n"
//         "    cmp    rax, 0\n"
//         "    jne    L1\n"
//         "    mov    qword [a], 1\n"
//         "L1:\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// // Struct (record)
// TEST(StructTests, Rec1) {
//     std::string input_str = "type r=record a:integer; end;";
//     vector<Token> expected_tokens = {
//         {TT::Type, "type"},          {TT::Identifier, "r"}, {TT::Equal, "="},
//         {TT::Record, "record"},      {TT::Identifier, "a"}, {TT::Colon, ":"},
//         {TT::Identifier, "integer"}, {TT::Semicolon, ";"},  {TT::End, "end"},
//         {TT::Semicolon, ";"},        {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test",
//         make_unique<pascal::Block>(
//             vector<unique_ptr<pascal::Declaration>>{
//                 make_unique<pascal::TypeDecl>(
//                     "r",
//                     make_unique<pascal::RecordTypeSpec>(
//                         vector<unique_ptr<pascal::VarDecl>>{
//                             make_unique<pascal::VarDecl>(
//                                 vector<std::string>{"a"},
//                                 make_unique<pascal::SimpleTypeSpec>(
//                                     pascal::BasicType::Integer,
//                                     "integer"))}))},
//             vector<unique_ptr<pascal::Statement>>{}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(StructTests, Rec2) {
//     std::string input_str = "var v:r;";
//     vector<Token> expected_tokens = {
//         {TT::Var, "var"},      {TT::Identifier, "v"}, {TT::Colon, ":"},
//         {TT::Identifier, "r"}, {TT::Semicolon, ";"},  {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test", make_unique<pascal::Block>(
//                     vector<unique_ptr<pascal::Declaration>>{
//                         make_unique<pascal::VarDecl>(
//                             vector<std::string>{"v"},
//                             make_unique<pascal::SimpleTypeSpec>(
//                                 pascal::BasicType::Integer, "r"))},
//                     vector<unique_ptr<pascal::Statement>>{}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "v:    resq    1\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(StructTests, Rec3) {
//     std::string input_str = "v.a:=1;";
//     vector<Token> expected_tokens = {
//         {TT::Identifier, "v"}, {TT::Dot, "."},     {TT::Identifier, "a"},
//         {TT::Colon, ":"},      {TT::Assign, "="},  {TT::Number, "1"},
//         {TT::Semicolon, ";"},  {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     pascal::VariableExpr::Selector fieldSel(
//         "a", pascal::VariableExpr::Selector::Kind::Field);
//     expected_ast.root = make_unique<pascal::Program>(
//         "test", make_unique<pascal::Block>(
//                     vector<unique_ptr<pascal::Declaration>>{},
//                     vector<unique_ptr<pascal::Statement>>{
//                         make_unique<pascal::AssignStmt>(
//                             make_unique<pascal::VariableExpr>(
//                                 "v", vector{fieldSel}),
//                             make_unique<pascal::LiteralExpr>("1"))}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "v:    resq    1\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    mov    qword [v], 1\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(StructTests, Rec4) {
//     std::string input_str = "with v do a:=2;";
//     vector<Token> expected_tokens = {
//         {TT::With, "with"},    {TT::Identifier, "v"}, {TT::Do, "do"},
//         {TT::Identifier, "a"}, {TT::Colon, ":"},      {TT::Assign, "="},
//         {TT::Number, "2"},     {TT::Semicolon, ";"},  {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test", make_unique<pascal::Block>(
//                     vector<unique_ptr<pascal::Declaration>>{},
//                     vector<unique_ptr<pascal::Statement>>{
//                         make_unique<pascal::WithStmt>(
//                             make_unique<pascal::VariableExpr>("v"),
//                             make_unique<pascal::AssignStmt>(
//                                 make_unique<pascal::VariableExpr>("a"),
//                                 make_unique<pascal::LiteralExpr>("2")))}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "v:    resq    1\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    mov    qword [v], 2\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(StructTests, Rec5) {
//     std::string input_str = "if v.a=0 then v.a:=1;";
//     vector<Token> expected_tokens = {
//         {TT::If, "if"},        {TT::Identifier, "v"}, {TT::Dot, "."},
//         {TT::Identifier, "a"}, {TT::Equal, "="},      {TT::Number, "0"},
//         {TT::Then, "then"},    {TT::Identifier, "v"}, {TT::Dot, "."},
//         {TT::Identifier, "a"}, {TT::Colon, ":"},      {TT::Assign, "="},
//         {TT::Number, "1"},     {TT::Semicolon, ";"},  {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     pascal::VariableExpr::Selector fieldSel(
//         "a", pascal::VariableExpr::Selector::Kind::Field);
//     expected_ast.root = make_unique<pascal::Program>(
//         "test", make_unique<pascal::Block>(
//                     vector<unique_ptr<pascal::Declaration>>{},
//                     vector<unique_ptr<pascal::Statement>>{
//                         make_unique<pascal::IfStmt>(
//                             make_unique<pascal::BinaryExpr>(
//                                 make_unique<pascal::VariableExpr>(
//                                     "v", vector{fieldSel}),
//                                 "=",
//                                 make_unique<pascal::LiteralExpr>("0")),
//                             make_unique<pascal::AssignStmt>(
//                                 make_unique<pascal::VariableExpr>(
//                                     "v", vector{fieldSel}),
//                                 make_unique<pascal::LiteralExpr>("1")))}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "v:    resq    1\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    mov    rax, [v]\n"
//         "    cmp    rax, 0\n"
//         "    jne    L1\n"
//         "    mov    qword [v], 1\n"
//         "L1:\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// // Pointers
// TEST(PointerTests, Ptr1) {
//     std::string input_str = "var p:^integer;";
//     vector<Token> expected_tokens = {
//         {TT::Var, "var"},   {TT::Identifier, "p"},       {TT::Colon, ":"},
//         {TT::Caret, "^"},   {TT::Identifier, "integer"}, {TT::Semicolon,
//         ";"}, {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test", make_unique<pascal::Block>(
//                     vector<unique_ptr<pascal::Declaration>>{
//                         make_unique<pascal::VarDecl>(
//                             vector<std::string>{"p"},
//                             make_unique<pascal::PointerTypeSpec>(
//                                 make_unique<pascal::SimpleTypeSpec>(
//                                     pascal::BasicType::Integer,
//                                     "integer")))},
//                     vector<unique_ptr<pascal::Statement>>{}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "p:    resq    1\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(PointerTests, Ptr2) {
//     std::string input_str = "new(p);";
//     vector<Token> expected_tokens = {
//         {TT::New, "new"},      {TT::LeftParen, "("}, {TT::Identifier, "p"},
//         {TT::RightParen, ")"}, {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test",
//         make_unique<pascal::Block>(
//             vector<unique_ptr<pascal::Declaration>>{},
//             vector<unique_ptr<pascal::Statement>>{
//                 make_unique<pascal::ProcCall>(
//                     "new", vector<unique_ptr<pascal::Expression>>{
//                                make_unique<pascal::VariableExpr>("p")})}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "p:    resq    1\n\n"
//         "section .text\n"
//         "extern malloc\n"
//         "global main\n"
//         "main:\n"
//         "    mov    rdi, 8\n"
//         "    call   malloc\n"
//         "    mov    qword [p], rax\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(PointerTests, Ptr3) {
//     std::string input_str = "p^:=1;";
//     vector<Token> expected_tokens = {
//         {TT::Identifier, "p"}, {TT::Caret, "^"},  {TT::Colon, ":"},
//         {TT::Assign, "="},     {TT::Number, "1"}, {TT::Semicolon, ";"},
//         {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     pascal::VariableExpr::Selector ptrSel(
//         "", pascal::VariableExpr::Selector::Kind::Pointer);
//     expected_ast.root = make_unique<pascal::Program>(
//         "test", make_unique<pascal::Block>(
//                     vector<unique_ptr<pascal::Declaration>>{},
//                     vector<unique_ptr<pascal::Statement>>{
//                         make_unique<pascal::AssignStmt>(
//                             make_unique<pascal::VariableExpr>(
//                                 "p", vector{ptrSel}),
//                             make_unique<pascal::LiteralExpr>("1"))}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "p:    resq    1\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    mov    rax, [p]\n"
//         "    mov    qword [rax], 1\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(PointerTests, Ptr4) {
//     std::string input_str = "dispose(p);";
//     vector<Token> expected_tokens = {
//         {TT::Dispose, "dispose"}, {TT::LeftParen, "("}, {TT::Identifier,
//         "p"}, {TT::RightParen, ")"},    {TT::Semicolon, ";"}, {TT::EndOfFile,
//         ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test", make_unique<pascal::Block>(
//                     vector<unique_ptr<pascal::Declaration>>{},
//                     vector<unique_ptr<pascal::Statement>>{
//                         make_unique<pascal::ProcCall>(
//                             "dispose",
//                             vector<unique_ptr<pascal::Expression>>{
//                                 make_unique<pascal::VariableExpr>("p")})}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "p:    resq    1\n\n"
//         "section .text\n"
//         "extern free\n"
//         "global main\n"
//         "main:\n"
//         "    mov    rdi, [p]\n"
//         "    call   free\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(PointerTests, Ptr5) {
//     std::string input_str = "p:=nil;";
//     vector<Token> expected_tokens = {
//         {TT::Identifier, "p"},   {TT::Colon, ":"},     {TT::Assign, "="},
//         {TT::Identifier, "nil"},{TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test", make_unique<pascal::Block>(
//                     vector<unique_ptr<pascal::Declaration>>{},
//                     vector<unique_ptr<pascal::Statement>>{
//                         make_unique<pascal::AssignStmt>(
//                             make_unique<pascal::VariableExpr>("p"),
//                             make_unique<pascal::VariableExpr>("nil"))}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .bss\n"
//         "p:    resq    1\n\n"
//         "section .text\n"
//         "global main\n"
//         "main:\n"
//         "    mov    qword [p], 0\n"
//         "    ret\n";
//     std::string expected_output = "";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// // Print output tests
// TEST(PrintTests, PrintString) {
//     std::string input_str = "writeln('hello');";
//     vector<Token> expected_tokens = {
//         {TT::Identifier, "writeln"}, {TT::LeftParen, "("},
//         {TT::Identifier, "'"},        {TT::Identifier, "hello"},
//         {TT::Identifier, "'"},        {TT::RightParen, ")"},
//         {TT::Semicolon, ";"},         {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test", make_unique<pascal::Block>(
//                     vector<unique_ptr<pascal::Declaration>>{},
//                     vector<unique_ptr<pascal::Statement>>{
//                         make_unique<pascal::ProcCall>(
//                             "writeln",
//                             vector<unique_ptr<pascal::Expression>>{
//                                 make_unique<pascal::LiteralExpr>(
//                                     "'hello'")})}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .data\n"
//         "str0: db \"hello\", 0\n\n"
//         "section .text\n"
//         "extern puts\n"
//         "global main\n"
//         "main:\n"
//         "    mov    rdi, str0\n"
//         "    call   puts\n"
//         "    ret\n";
//     std::string expected_output = "hello\n";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(PrintTests, PrintIntLiteral) {
//     std::string input_str = "writeln(123);";
//     vector<Token> expected_tokens = {
//         {TT::Identifier, "writeln"}, {TT::LeftParen, "("},
//         {TT::Number, "123"},          {TT::RightParen, ")"},
//         {TT::Semicolon, ";"},         {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test", make_unique<pascal::Block>(
//                     vector<unique_ptr<pascal::Declaration>>{},
//                     vector<unique_ptr<pascal::Statement>>{
//                         make_unique<pascal::ProcCall>(
//                             "writeln",
//                             vector<unique_ptr<pascal::Expression>>{
//                                 make_unique<pascal::LiteralExpr>("123")})}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .data\n"
//         "fmt_int: db \"%d\", 10, 0\n\n"
//         "section .text\n"
//         "extern printf\n"
//         "global main\n"
//         "main:\n"
//         "    mov    rdi, fmt_int\n"
//         "    mov    rsi, 123\n"
//         "    xor    rax, rax\n"
//         "    call   printf\n"
//         "    ret\n";
//     std::string expected_output = "123\n";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
//
// TEST(PrintTests, PrintIntExpr) {
//     std::string input_str = "writeln(10+20);";
//     vector<Token> expected_tokens = {
//         {TT::Identifier, "writeln"}, {TT::LeftParen, "("},
//         {TT::Number, "10"},           {TT::Plus, "+"},
//         {TT::Number, "20"},           {TT::RightParen, ")"},
//         {TT::Semicolon, ";"},         {TT::EndOfFile, ""}};
//     AST expected_ast{};
//     expected_ast.root = make_unique<pascal::Program>(
//         "test", make_unique<pascal::Block>(
//                     vector<unique_ptr<pascal::Declaration>>{},
//                     vector<unique_ptr<pascal::Statement>>{
//                         make_unique<pascal::ProcCall>(
//                             "writeln",
//                             vector<unique_ptr<pascal::Expression>>{
//                                 make_unique<pascal::BinaryExpr>(
//                                     make_unique<pascal::LiteralExpr>("10"),
//                                     "+",
//                                     make_unique<pascal::LiteralExpr>(
//                                         "20"))})}));
//     expected_ast.valid = true;
//     std::string expected_asm =
//         "section .data\n"
//         "fmt_int: db \"%d\", 10, 0\n\n"
//         "section .text\n"
//         "extern printf\n"
//         "global main\n"
//         "main:\n"
//         "    mov    rdi, fmt_int\n"
//         "    mov    rsi, 10\n"
//         "    add    rsi, 20\n"
//         "    xor    rax, rax\n"
//         "    call   printf\n"
//         "    ret\n";
//     std::string expected_output = "30\n";
//     run_full(input_str, expected_tokens, expected_ast, expected_asm,
//              expected_output);
// }
