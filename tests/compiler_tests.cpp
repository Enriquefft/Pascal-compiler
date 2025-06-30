#include "test_utils.hpp"
#include <gtest/gtest.h>

using pascal::AST;
using pascal::Lexer;
using pascal::Parser;
using pascal::Token;
using test_utils::run_full;
using TT = pascal::TokenType;

// Variable declarations
TEST(VarDeclTests, Decl1) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Var, "var"},
                          {TT::Identifier, "a"},
                          {TT::Colon, ":"},
                          {TT::Identifier, "integer"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("var a: integer;", expected_tokens, expected_ast, expected_asm,
           expected_output);
}

TEST(VarDeclTests, Decl2) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Var, "var"},
                          {TT::Identifier, "b"},
                          {TT::Colon, ":"},
                          {TT::Identifier, "real"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("var b: real;", expected_tokens, expected_ast, expected_asm,
           expected_output);
}

TEST(VarDeclTests, Decl3) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Var, "var"},
                          {TT::Identifier, "c"},
                          {TT::Colon, ":"},
                          {TT::Identifier, "unsigned"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("var c: unsigned;", expected_tokens, expected_ast, expected_asm,
           expected_output);
}

// Expressions
TEST(ExpressionTests, Expr1) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Begin, "begin"},
                          {TT::Identifier, "a"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Number, "1"},
                          {TT::Semicolon, ";"},
                          {TT::End, "end"},
                          {TT::Dot, "."},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("begin a := 1; end.", expected_tokens, expected_ast, expected_asm,
           expected_output);
}

TEST(ExpressionTests, Expr2) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Begin, "begin"},
                          {TT::Identifier, "b"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Identifier, "a"},
                          {TT::Plus, "+"},
                          {TT::Number, "1"},
                          {TT::Semicolon, ";"},
                          {TT::End, "end"},
                          {TT::Dot, "."},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("begin b := a + 1; end.", expected_tokens, expected_ast,
           expected_asm, expected_output);
}

TEST(ExpressionTests, Expr3) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Begin, "begin"},
                          {TT::Identifier, "c"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Identifier, "b"},
                          {TT::Star, "*"},
                          {TT::Number, "2"},
                          {TT::Semicolon, ";"},
                          {TT::End, "end"},
                          {TT::Dot, "."},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("begin c := b * 2; end.", expected_tokens, expected_ast,
           expected_asm, expected_output);
}

// Control statements
TEST(ControlTests, IfStmt) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::If, "if"},
                          {TT::Identifier, "a"},
                          {TT::Greater, ">"},
                          {TT::Number, "0"},
                          {TT::Then, "then"},
                          {TT::Identifier, "b"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Number, "1"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("if a > 0 then b := 1;", expected_tokens, expected_ast, expected_asm,
           expected_output);
}
TEST(ControlTests, IfElse) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::If, "if"},
                          {TT::Identifier, "a"},
                          {TT::Greater, ">"},
                          {TT::Number, "0"},
                          {TT::Then, "then"},
                          {TT::Identifier, "b"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Number, "1"},
                          {TT::Else, "else"},
                          {TT::Identifier, "b"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Number, "2"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("if a > 0 then b := 1 else b := 2;", expected_tokens, expected_ast,
           expected_asm, expected_output);
}
TEST(ControlTests, CaseStmt) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Case, "case"},
                          {TT::Identifier, "a"},
                          {TT::Of, "of"},
                          {TT::Number, "1"},
                          {TT::Colon, ":"},
                          {TT::Identifier, "b"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Number, "1"},
                          {TT::Semicolon, ";"},
                          {TT::End, "end"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("case a of 1: b := 1; end;", expected_tokens, expected_ast,
           expected_asm, expected_output);
}
TEST(ControlTests, WhileStmt) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::While, "while"},
                          {TT::Identifier, "a"},
                          {TT::Greater, ">"},
                          {TT::Number, "0"},
                          {TT::Do, "do"},
                          {TT::Identifier, "a"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Identifier, "a"},
                          {TT::Minus, "-"},
                          {TT::Number, "1"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("while a > 0 do a := a - 1;", expected_tokens, expected_ast,
           expected_asm, expected_output);
}
TEST(ControlTests, ForStmt) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::For, "for"},
                          {TT::Identifier, "i"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Number, "1"},
                          {TT::To, "to"},
                          {TT::Number, "10"},
                          {TT::Do, "do"},
                          {TT::Identifier, "a"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Identifier, "i"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("for i:=1 to 10 do a:=i;", expected_tokens, expected_ast,
           expected_asm, expected_output);
}
TEST(ControlTests, RepeatStmt) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Repeat, "repeat"},
                          {TT::Identifier, "a"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Identifier, "a"},
                          {TT::Minus, "-"},
                          {TT::Number, "1"},
                          {TT::Until, "until"},
                          {TT::Identifier, "a"},
                          {TT::Equal, "="},
                          {TT::Number, "0"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("repeat a:=a-1 until a=0;", expected_tokens, expected_ast,
           expected_asm, expected_output);
}

// Functions
TEST(FunctionTests, Func1) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Function, "function"},
                          {TT::Identifier, "f"},
                          {TT::Colon, ":"},
                          {TT::Identifier, "integer"},
                          {TT::Semicolon, ";"},
                          {TT::Begin, "begin"},
                          {TT::Identifier, "f"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Number, "0"},
                          {TT::Semicolon, ";"},
                          {TT::End, "end"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("function f: integer; begin f:=0; end;", expected_tokens,
           expected_ast, expected_asm, expected_output);
}
TEST(FunctionTests, Func2) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Procedure, "procedure"},
                          {TT::Identifier, "p"},
                          {TT::Semicolon, ";"},
                          {TT::Begin, "begin"},
                          {TT::End, "end"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("procedure p; begin end;", expected_tokens, expected_ast,
           expected_asm, expected_output);
}
TEST(FunctionTests, Func3) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Function, "function"},
                          {TT::Identifier, "g"},
                          {TT::LeftParen, "("},
                          {TT::Identifier, "x"},
                          {TT::Colon, ":"},
                          {TT::Identifier, "integer"},
                          {TT::RightParen, ")"},
                          {TT::Colon, ":"},
                          {TT::Identifier, "integer"},
                          {TT::Semicolon, ";"},
                          {TT::Begin, "begin"},
                          {TT::Identifier, "g"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Identifier, "x"},
                          {TT::Semicolon, ";"},
                          {TT::End, "end"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("function g(x: integer): integer; begin g:=x; end;", expected_tokens,
           expected_ast, expected_asm, expected_output);
}

// Float
TEST(FloatTests, Float1) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Var, "var"},
                          {TT::Identifier, "x"},
                          {TT::Colon, ":"},
                          {TT::Identifier, "real"},
                          {TT::Semicolon, ";"},
                          {TT::Identifier, "x"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Number, "1"},
                          {TT::Dot, "."},
                          {TT::Number, "0"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("var x: real; x:=1.0;", expected_tokens, expected_ast, expected_asm,
           expected_output);
}
TEST(FloatTests, Float2) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Identifier, "x"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Number, "1"},
                          {TT::Dot, "."},
                          {TT::Number, "5"},
                          {TT::Plus, "+"},
                          {TT::Number, "2"},
                          {TT::Dot, "."},
                          {TT::Number, "5"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("x:=1.5+2.5;", expected_tokens, expected_ast, expected_asm,
           expected_output);
}
TEST(FloatTests, Float3) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::If, "if"},
                          {TT::Number, "0"},
                          {TT::Dot, "."},
                          {TT::Number, "0"},
                          {TT::Less, "<"},
                          {TT::Number, "1"},
                          {TT::Dot, "."},
                          {TT::Number, "0"},
                          {TT::Then, "then"},
                          {TT::Identifier, "x"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Number, "0"},
                          {TT::Dot, "."},
                          {TT::Number, "0"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("if 0.0 < 1.0 then x:=0.0;", expected_tokens, expected_ast,
           expected_asm, expected_output);
}
TEST(FloatTests, Float4) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::While, "while"},
                          {TT::Identifier, "x"},
                          {TT::Less, "<"},
                          {TT::Number, "1"},
                          {TT::Dot, "."},
                          {TT::Number, "0"},
                          {TT::Do, "do"},
                          {TT::Identifier, "x"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Identifier, "x"},
                          {TT::Plus, "+"},
                          {TT::Number, "0"},
                          {TT::Dot, "."},
                          {TT::Number, "1"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("while x < 1.0 do x:=x+0.1;", expected_tokens, expected_ast,
           expected_asm, expected_output);
}
TEST(FloatTests, Float5) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Function, "function"},
                          {TT::Identifier, "f"},
                          {TT::Colon, ":"},
                          {TT::Identifier, "real"},
                          {TT::Semicolon, ";"},
                          {TT::Begin, "begin"},
                          {TT::Identifier, "f"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Number, "0"},
                          {TT::Dot, "."},
                          {TT::Number, "0"},
                          {TT::Semicolon, ";"},
                          {TT::End, "end"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("function f: real; begin f:=0.0; end;", expected_tokens,
           expected_ast, expected_asm, expected_output);
}

// Unsigned int
TEST(UnsignedTests, Uns1) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Var, "var"},
                          {TT::Identifier, "u"},
                          {TT::Colon, ":"},
                          {TT::Identifier, "unsigned"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("var u: unsigned;", expected_tokens, expected_ast, expected_asm,
           expected_output);
}
TEST(UnsignedTests, Uns2) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Identifier, "u"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Number, "1"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("u:=1;", expected_tokens, expected_ast, expected_asm,
           expected_output);
}
TEST(UnsignedTests, Uns3) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::While, "while"},
                          {TT::Identifier, "u"},
                          {TT::Greater, ">"},
                          {TT::Number, "0"},
                          {TT::Do, "do"},
                          {TT::Identifier, "u"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Identifier, "u"},
                          {TT::Minus, "-"},
                          {TT::Number, "1"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("while u>0 do u:=u-1;", expected_tokens, expected_ast, expected_asm,
           expected_output);
}
TEST(UnsignedTests, Uns4) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Function, "function"},
                          {TT::Identifier, "f"},
                          {TT::Colon, ":"},
                          {TT::Identifier, "unsigned"},
                          {TT::Semicolon, ";"},
                          {TT::Begin, "begin"},
                          {TT::Identifier, "f"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Number, "0"},
                          {TT::Semicolon, ";"},
                          {TT::End, "end"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("function f: unsigned; begin f:=0; end;", expected_tokens,
           expected_ast, expected_asm, expected_output);
}
TEST(UnsignedTests, Uns5) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::For, "for"},
                          {TT::Identifier, "u"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Number, "1"},
                          {TT::To, "to"},
                          {TT::Number, "5"},
                          {TT::Do, "do"},
                          {TT::Identifier, "u"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Identifier, "u"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("for u:=1 to 5 do u:=u;", expected_tokens, expected_ast,
           expected_asm, expected_output);
}

// Long int
TEST(LongIntTests, Long1) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Var, "var"},
                          {TT::Identifier, "l"},
                          {TT::Colon, ":"},
                          {TT::Identifier, "longint"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("var l: longint;", expected_tokens, expected_ast, expected_asm,
           expected_output);
}
TEST(LongIntTests, Long2) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Identifier, "l"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Number, "1"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("l:=1;", expected_tokens, expected_ast, expected_asm,
           expected_output);
}
TEST(LongIntTests, Long3) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::While, "while"},
                          {TT::Identifier, "l"},
                          {TT::Greater, ">"},
                          {TT::Number, "0"},
                          {TT::Do, "do"},
                          {TT::Identifier, "l"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Identifier, "l"},
                          {TT::Minus, "-"},
                          {TT::Number, "1"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("while l>0 do l:=l-1;", expected_tokens, expected_ast, expected_asm,
           expected_output);
}
TEST(LongIntTests, Long4) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Function, "function"},
                          {TT::Identifier, "f"},
                          {TT::Colon, ":"},
                          {TT::Identifier, "longint"},
                          {TT::Semicolon, ";"},
                          {TT::Begin, "begin"},
                          {TT::Identifier, "f"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Number, "0"},
                          {TT::Semicolon, ";"},
                          {TT::End, "end"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("function f: longint; begin f:=0; end;", expected_tokens,
           expected_ast, expected_asm, expected_output);
}
TEST(LongIntTests, Long5) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::For, "for"},
                          {TT::Identifier, "l"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Number, "1"},
                          {TT::To, "to"},
                          {TT::Number, "5"},
                          {TT::Do, "do"},
                          {TT::Identifier, "l"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Identifier, "l"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("for l:=1 to 5 do l:=l;", expected_tokens, expected_ast,
           expected_asm, expected_output);
}

// Dynamic memory
TEST(DynamicTests, Dyn1) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::New, "new"},
                          {TT::LeftParen, "("},
                          {TT::Identifier, "p"},
                          {TT::RightParen, ")"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("new(p);", expected_tokens, expected_ast, expected_asm,
           expected_output);
}
TEST(DynamicTests, Dyn2) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Dispose, "dispose"},
                          {TT::LeftParen, "("},
                          {TT::Identifier, "p"},
                          {TT::RightParen, ")"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("dispose(p);", expected_tokens, expected_ast, expected_asm,
           expected_output);
}
TEST(DynamicTests, Dyn3) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Var, "var"},
                          {TT::Identifier, "p"},
                          {TT::Colon, ":"},
                          {TT::Caret, "^"},
                          {TT::Identifier, "integer"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("var p:^integer;", expected_tokens, expected_ast, expected_asm,
           expected_output);
}
TEST(DynamicTests, Dyn4) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Identifier, "p"},
                          {TT::Caret, "^"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Number, "1"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("p^:=1;", expected_tokens, expected_ast, expected_asm,
           expected_output);
}
TEST(DynamicTests, Dyn5) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::If, "if"},
                          {TT::Identifier, "p"},
                          {TT::Less, "<"},
                          {TT::Greater, ">"},
                          {TT::Identifier, "nil"},
                          {TT::Then, "then"},
                          {TT::Dispose, "dispose"},
                          {TT::LeftParen, "("},
                          {TT::Identifier, "p"},
                          {TT::RightParen, ")"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("if p<>nil then dispose(p);", expected_tokens, expected_ast,
           expected_asm, expected_output);
}

// Strings
TEST(StringTests, Str1) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Var, "var"},
                          {TT::Identifier, "s"},
                          {TT::Colon, ":"},
                          {TT::Identifier, "string"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("var s: string;", expected_tokens, expected_ast, expected_asm,
           expected_output);
}
TEST(StringTests, Str2) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Identifier, "s"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Identifier, "'"},
                          {TT::Identifier, "hi"},
                          {TT::Identifier, "'"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("s:='hi';", expected_tokens, expected_ast, expected_asm,
           expected_output);
}
TEST(StringTests, Str3) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Identifier, "s"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Identifier, "s"},
                          {TT::Plus, "+"},
                          {TT::Identifier, "'"},
                          {TT::Identifier, "!"},
                          {TT::Identifier, "'"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("s:=s+'!';", expected_tokens, expected_ast, expected_asm,
           expected_output);
}
TEST(StringTests, Str4) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Identifier, "writeln"},
                          {TT::LeftParen, "("},
                          {TT::Identifier, "s"},
                          {TT::RightParen, ")"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("writeln(s);", expected_tokens, expected_ast, expected_asm,
           expected_output);
}
TEST(StringTests, Str5) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::If, "if"},
                          {TT::Identifier, "s"},
                          {TT::Equal, "="},
                          {TT::Identifier, "'"},
                          {TT::Identifier, "'"},
                          {TT::Then, "then"},
                          {TT::Identifier, "s"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Identifier, "'"},
                          {TT::Identifier, "a"},
                          {TT::Identifier, "'"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("if s='' then s:='a';", expected_tokens, expected_ast, expected_asm,
           expected_output);
}

// Arrays
TEST(ArrayTests, Arr1) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Var, "var"},
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
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("var a: array[1..5] of integer;", expected_tokens, expected_ast,
           expected_asm, expected_output);
}
TEST(ArrayTests, Arr2) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Identifier, "a"},
                          {TT::LeftBracket, "["},
                          {TT::Number, "1"},
                          {TT::RightBracket, "]"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Number, "0"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("a[1]:=0;", expected_tokens, expected_ast, expected_asm,
           expected_output);
}
TEST(ArrayTests, Arr3) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::For, "for"},
                          {TT::Identifier, "i"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Number, "1"},
                          {TT::To, "to"},
                          {TT::Number, "5"},
                          {TT::Do, "do"},
                          {TT::Identifier, "a"},
                          {TT::LeftBracket, "["},
                          {TT::Identifier, "i"},
                          {TT::RightBracket, "]"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Identifier, "i"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("for i:=1 to 5 do a[i]:=i;", expected_tokens, expected_ast,
           expected_asm, expected_output);
}
TEST(ArrayTests, Arr4) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Identifier, "writeln"},
                          {TT::LeftParen, "("},
                          {TT::Identifier, "a"},
                          {TT::LeftBracket, "["},
                          {TT::Number, "1"},
                          {TT::RightBracket, "]"},
                          {TT::RightParen, ")"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("writeln(a[1]);", expected_tokens, expected_ast, expected_asm,
           expected_output);
}
TEST(ArrayTests, Arr5) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::If, "if"},
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
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Number, "1"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("if a[1]=0 then a[1]:=1;", expected_tokens, expected_ast,
           expected_asm, expected_output);
}

// Struct (record)
TEST(StructTests, Rec1) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Type, "type"},
                          {TT::Identifier, "r"},
                          {TT::Equal, "="},
                          {TT::Record, "record"},
                          {TT::Identifier, "a"},
                          {TT::Colon, ":"},
                          {TT::Identifier, "integer"},
                          {TT::Semicolon, ";"},
                          {TT::End, "end"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("type r=record a:integer; end;", expected_tokens, expected_ast,
           expected_asm, expected_output);
}
TEST(StructTests, Rec2) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Var, "var"},
                          {TT::Identifier, "v"},
                          {TT::Colon, ":"},
                          {TT::Identifier, "r"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("var v:r;", expected_tokens, expected_ast, expected_asm,
           expected_output);
}
TEST(StructTests, Rec3) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Identifier, "v"},
                          {TT::Dot, "."},
                          {TT::Identifier, "a"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Number, "1"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("v.a:=1;", expected_tokens, expected_ast, expected_asm,
           expected_output);
}
TEST(StructTests, Rec4) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::With, "with"},
                          {TT::Identifier, "v"},
                          {TT::Do, "do"},
                          {TT::Identifier, "a"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Number, "2"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("with v do a:=2;", expected_tokens, expected_ast, expected_asm,
           expected_output);
}
TEST(StructTests, Rec5) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::If, "if"},
                          {TT::Identifier, "v"},
                          {TT::Dot, "."},
                          {TT::Identifier, "a"},
                          {TT::Equal, "="},
                          {TT::Number, "0"},
                          {TT::Then, "then"},
                          {TT::Identifier, "v"},
                          {TT::Dot, "."},
                          {TT::Identifier, "a"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Number, "1"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("if v.a=0 then v.a:=1;", expected_tokens, expected_ast, expected_asm,
           expected_output);
}

// Pointers
TEST(PointerTests, Ptr1) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Var, "var"},
                          {TT::Identifier, "p"},
                          {TT::Colon, ":"},
                          {TT::Caret, "^"},
                          {TT::Identifier, "integer"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("var p:^integer;", expected_tokens, expected_ast, expected_asm,
           expected_output);
}
TEST(PointerTests, Ptr2) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::New, "new"},
                          {TT::LeftParen, "("},
                          {TT::Identifier, "p"},
                          {TT::RightParen, ")"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("new(p);", expected_tokens, expected_ast, expected_asm,
           expected_output);
}
TEST(PointerTests, Ptr3) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Identifier, "p"},
                          {TT::Caret, "^"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Number, "1"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("p^:=1;", expected_tokens, expected_ast, expected_asm,
           expected_output);
}
TEST(PointerTests, Ptr4) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Dispose, "dispose"},
                          {TT::LeftParen, "("},
                          {TT::Identifier, "p"},
                          {TT::RightParen, ")"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("dispose(p);", expected_tokens, expected_ast, expected_asm,
           expected_output);
}
TEST(PointerTests, Ptr5) {
  std::vector<Token> expected_tokens =
      std::vector<Token>({{TT::Identifier, "p"},
                          {TT::Colon, ":"},
                          {TT::Assign, "="},
                          {TT::Identifier, "nil"},
                          {TT::Semicolon, ";"},
                          {TT::EndOfFile, ""}});
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full("p:=nil;", expected_tokens, expected_ast, expected_asm,
           expected_output);
}
