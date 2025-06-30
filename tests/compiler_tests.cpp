#include "test_utils.hpp"
#include <gtest/gtest.h>

using pascal::AST;
using pascal::Lexer;
using pascal::Parser;
using test_utils::make_expected;
using test_utils::run_full;

// Variable declarations
TEST(VarDeclTests, Decl1) {
  run_full("var a: integer;", make_expected({"var", "a", ":", "integer", ";"}));
}
TEST(VarDeclTests, Decl2) {
  run_full("var b: real;", make_expected({"var", "b", ":", "real", ";"}));
}
TEST(VarDeclTests, Decl3) {
  run_full("var c: unsigned;",
           make_expected({"var", "c", ":", "unsigned", ";"}));
}

// Expressions
TEST(ExpressionTests, Expr1) {
  run_full("begin a := 1; end.",
           make_expected({"begin", "a", ":", "=", "1", ";", "end", "."}));
}
TEST(ExpressionTests, Expr2) {
  run_full(
      "begin b := a + 1; end.",
      make_expected({"begin", "b", ":", "=", "a", "+", "1", ";", "end", "."}));
}
TEST(ExpressionTests, Expr3) {
  run_full(
      "begin c := b * 2; end.",
      make_expected({"begin", "c", ":", "=", "b", "*", "2", ";", "end", "."}));
}

// Control statements
TEST(ControlTests, IfStmt) {
  run_full("if a > 0 then b := 1;", make_expected({"if", "a", ">", "0", "then",
                                                   "b", ":", "=", "1", ";"}));
}
TEST(ControlTests, IfElse) {
  run_full("if a > 0 then b := 1 else b := 2;",
           make_expected({"if", "a", ">", "0", "then", "b", ":", "=", "1",
                          "else", "b", ":", "=", "2", ";"}));
}
TEST(ControlTests, CaseStmt) {
  run_full("case a of 1: b := 1; end;",
           make_expected({"case", "a", "of", "1", ":", "b", ":", "=", "1", ";",
                          "end", ";"}));
}
TEST(ControlTests, WhileStmt) {
  run_full("while a > 0 do a := a - 1;",
           make_expected({"while", "a", ">", "0", "do", "a", ":", "=", "a", "-",
                          "1", ";"}));
}
TEST(ControlTests, ForStmt) {
  run_full("for i:=1 to 10 do a:=i;",
           make_expected({"for", "i", ":", "=", "1", "to", "10", "do", "a", ":",
                          "=", "i", ";"}));
}
TEST(ControlTests, RepeatStmt) {
  run_full("repeat a:=a-1 until a=0;",
           make_expected({"repeat", "a", ":", "=", "a", "-", "1", "until", "a",
                          "=", "0", ";"}));
}

// Functions
TEST(FunctionTests, Func1) {
  run_full("function f: integer; begin f:=0; end;",
           make_expected({"function", "f", ":", "integer", ";", "begin", "f",
                          ":", "=", "0", ";", "end", ";"}));
}
TEST(FunctionTests, Func2) {
  run_full("procedure p; begin end;",
           make_expected({"procedure", "p", ";", "begin", "end", ";"}));
}
TEST(FunctionTests, Func3) {
  run_full("function g(x: integer): integer; begin g:=x; end;",
           make_expected({"function", "g", "(", "x", ":", "integer", ")", ":",
                          "integer", ";", "begin", "g", ":", "=", "x", ";",
                          "end", ";"}));
}

// Float
TEST(FloatTests, Float1) {
  run_full("var x: real; x:=1.0;",
           make_expected({"var", "x", ":", "real", ";", "x", ":", "=", "1", ".",
                          "0", ";"}));
}
TEST(FloatTests, Float2) {
  run_full("x:=1.5+2.5;", make_expected({"x", ":", "=", "1", ".", "5", "+", "2",
                                         ".", "5", ";"}));
}
TEST(FloatTests, Float3) {
  run_full("if 0.0 < 1.0 then x:=0.0;",
           make_expected({"if", "0", ".", "0", "<", "1", ".", "0", "then", "x",
                          ":", "=", "0", ".", "0", ";"}));
}
TEST(FloatTests, Float4) {
  run_full("while x < 1.0 do x:=x+0.1;",
           make_expected({"while", "x", "<", "1", ".", "0", "do", "x", ":", "=",
                          "x", "+", "0", ".", "1", ";"}));
}
TEST(FloatTests, Float5) {
  run_full("function f: real; begin f:=0.0; end;",
           make_expected({"function", "f", ":", "real", ";", "begin", "f", ":",
                          "=", "0", ".", "0", ";", "end", ";"}));
}

// Unsigned int
TEST(UnsignedTests, Uns1) {
  run_full("var u: unsigned;",
           make_expected({"var", "u", ":", "unsigned", ";"}));
}
TEST(UnsignedTests, Uns2) {
  run_full("u:=1;", make_expected({"u", ":", "=", "1", ";"}));
}
TEST(UnsignedTests, Uns3) {
  run_full("while u>0 do u:=u-1;",
           make_expected({"while", "u", ">", "0", "do", "u", ":", "=", "u", "-",
                          "1", ";"}));
}
TEST(UnsignedTests, Uns4) {
  run_full("function f: unsigned; begin f:=0; end;",
           make_expected({"function", "f", ":", "unsigned", ";", "begin", "f",
                          ":", "=", "0", ";", "end", ";"}));
}
TEST(UnsignedTests, Uns5) {
  run_full("for u:=1 to 5 do u:=u;",
           make_expected({"for", "u", ":", "=", "1", "to", "5", "do", "u", ":",
                          "=", "u", ";"}));
}

// Long int
TEST(LongIntTests, Long1) {
  run_full("var l: longint;", make_expected({"var", "l", ":", "longint", ";"}));
}
TEST(LongIntTests, Long2) {
  run_full("l:=1;", make_expected({"l", ":", "=", "1", ";"}));
}
TEST(LongIntTests, Long3) {
  run_full("while l>0 do l:=l-1;",
           make_expected({"while", "l", ">", "0", "do", "l", ":", "=", "l", "-",
                          "1", ";"}));
}
TEST(LongIntTests, Long4) {
  run_full("function f: longint; begin f:=0; end;",
           make_expected({"function", "f", ":", "longint", ";", "begin", "f",
                          ":", "=", "0", ";", "end", ";"}));
}
TEST(LongIntTests, Long5) {
  run_full("for l:=1 to 5 do l:=l;",
           make_expected({"for", "l", ":", "=", "1", "to", "5", "do", "l", ":",
                          "=", "l", ";"}));
}

// Dynamic memory
TEST(DynamicTests, Dyn1) {
  run_full("new(p);", make_expected({"new", "(", "p", ")", ";"}));
}
TEST(DynamicTests, Dyn2) {
  run_full("dispose(p);", make_expected({"dispose", "(", "p", ")", ";"}));
}
TEST(DynamicTests, Dyn3) {
  run_full("var p:^integer;",
           make_expected({"var", "p", ":", "^", "integer", ";"}));
}
TEST(DynamicTests, Dyn4) {
  run_full("p^:=1;", make_expected({"p", "^", ":", "=", "1", ";"}));
}
TEST(DynamicTests, Dyn5) {
  run_full("if p<>nil then dispose(p);",
           make_expected({"if", "p", "<", ">", "nil", "then", "dispose", "(",
                          "p", ")", ";"}));
}

// Strings
TEST(StringTests, Str1) {
  run_full("var s: string;", make_expected({"var", "s", ":", "string", ";"}));
}
TEST(StringTests, Str2) {
  run_full("s:='hi';", make_expected({"s", ":", "=", "'", "hi", "'", ";"}));
}
TEST(StringTests, Str3) {
  run_full("s:=s+'!';",
           make_expected({"s", ":", "=", "s", "+", "'", "!", "'", ";"}));
}
TEST(StringTests, Str4) {
  run_full("writeln(s);", make_expected({"writeln", "(", "s", ")", ";"}));
}
TEST(StringTests, Str5) {
  run_full("if s='' then s:='a';",
           make_expected({"if", "s", "=", "'", "'", "then", "s", ":", "=", "'",
                          "a", "'", ";"}));
}

// Arrays
TEST(ArrayTests, Arr1) {
  run_full("var a: array[1..5] of integer;",
           make_expected({"var", "a", ":", "array", "[", "1", ".", ".", "5",
                          "]", "of", "integer", ";"}));
}
TEST(ArrayTests, Arr2) {
  run_full("a[1]:=0;", make_expected({"a", "[", "1", "]", ":", "=", "0", ";"}));
}
TEST(ArrayTests, Arr3) {
  run_full("for i:=1 to 5 do a[i]:=i;",
           make_expected({"for", "i", ":", "=", "1", "to", "5", "do", "a", "[",
                          "i", "]", ":", "=", "i", ";"}));
}
TEST(ArrayTests, Arr4) {
  run_full("writeln(a[1]);",
           make_expected({"writeln", "(", "a", "[", "1", "]", ")", ";"}));
}
TEST(ArrayTests, Arr5) {
  run_full("if a[1]=0 then a[1]:=1;",
           make_expected({"if", "a", "[", "1", "]", "=", "0", "then", "a", "[",
                          "1", "]", ":", "=", "1", ";"}));
}

// Struct (record)
TEST(StructTests, Rec1) {
  run_full("type r=record a:integer; end;",
           make_expected({"type", "r", "=", "record", "a", ":", "integer", ";",
                          "end", ";"}));
}
TEST(StructTests, Rec2) {
  run_full("var v:r;", make_expected({"var", "v", ":", "r", ";"}));
}
TEST(StructTests, Rec3) {
  run_full("v.a:=1;", make_expected({"v", ".", "a", ":", "=", "1", ";"}));
}
TEST(StructTests, Rec4) {
  run_full("with v do a:=2;",
           make_expected({"with", "v", "do", "a", ":", "=", "2", ";"}));
}
TEST(StructTests, Rec5) {
  run_full("if v.a=0 then v.a:=1;",
           make_expected({"if", "v", ".", "a", "=", "0", "then", "v", ".", "a",
                          ":", "=", "1", ";"}));
}

// Pointers
TEST(PointerTests, Ptr1) {
  run_full("var p:^integer;",
           make_expected({"var", "p", ":", "^", "integer", ";"}));
}
TEST(PointerTests, Ptr2) {
  run_full("new(p);", make_expected({"new", "(", "p", ")", ";"}));
}
TEST(PointerTests, Ptr3) {
  run_full("p^:=1;", make_expected({"p", "^", ":", "=", "1", ";"}));
}
TEST(PointerTests, Ptr4) {
  run_full("dispose(p);", make_expected({"dispose", "(", "p", ")", ";"}));
}
TEST(PointerTests, Ptr5) {
  run_full("p:=nil;", make_expected({"p", ":", "=", "nil", ";"}));
}
