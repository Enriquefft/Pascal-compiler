#include "parser/parser.hpp"
#include "scanner/lexer.hpp"
#include <gtest/gtest.h>

using pascal::Lexer;
using pascal::Parser;

static void run_no_throw(std::string_view src) {
  Lexer lex(src);
  auto tokens = lex.scanTokens();
  Parser parser(tokens);
  EXPECT_NO_THROW(parser.parse());
}

// Variable declarations
TEST(VarDeclTests, Decl1) { run_no_throw("var a: integer;"); }
TEST(VarDeclTests, Decl2) { run_no_throw("var b: real;"); }
TEST(VarDeclTests, Decl3) { run_no_throw("var c: unsigned;"); }

// Expressions
TEST(ExpressionTests, Expr1) { run_no_throw("begin a := 1; end."); }
TEST(ExpressionTests, Expr2) { run_no_throw("begin b := a + 1; end."); }
TEST(ExpressionTests, Expr3) { run_no_throw("begin c := b * 2; end."); }

// Control statements
TEST(ControlTests, IfStmt) { run_no_throw("if a > 0 then b := 1;"); }
TEST(ControlTests, IfElse) {
  run_no_throw("if a > 0 then b := 1 else b := 2;");
}
TEST(ControlTests, CaseStmt) { run_no_throw("case a of 1: b := 1; end;"); }
TEST(ControlTests, WhileStmt) { run_no_throw("while a > 0 do a := a - 1;"); }
TEST(ControlTests, ForStmt) { run_no_throw("for i:=1 to 10 do a:=i;"); }
TEST(ControlTests, RepeatStmt) { run_no_throw("repeat a:=a-1 until a=0;"); }

// Functions
TEST(FunctionTests, Func1) {
  run_no_throw("function f: integer; begin f:=0; end;");
}
TEST(FunctionTests, Func2) { run_no_throw("procedure p; begin end;"); }
TEST(FunctionTests, Func3) {
  run_no_throw("function g(x: integer): integer; begin g:=x; end;");
}

// Float
TEST(FloatTests, Float1) { run_no_throw("var x: real; x:=1.0;"); }
TEST(FloatTests, Float2) { run_no_throw("x:=1.5+2.5;"); }
TEST(FloatTests, Float3) { run_no_throw("if 0.0 < 1.0 then x:=0.0;"); }
TEST(FloatTests, Float4) { run_no_throw("while x < 1.0 do x:=x+0.1;"); }
TEST(FloatTests, Float5) {
  run_no_throw("function f: real; begin f:=0.0; end;");
}

// Unsigned int
TEST(UnsignedTests, Uns1) { run_no_throw("var u: unsigned;"); }
TEST(UnsignedTests, Uns2) { run_no_throw("u:=1;"); }
TEST(UnsignedTests, Uns3) { run_no_throw("while u>0 do u:=u-1;"); }
TEST(UnsignedTests, Uns4) {
  run_no_throw("function f: unsigned; begin f:=0; end;");
}
TEST(UnsignedTests, Uns5) { run_no_throw("for u:=1 to 5 do u:=u;"); }

// Long int
TEST(LongIntTests, Long1) { run_no_throw("var l: longint;"); }
TEST(LongIntTests, Long2) { run_no_throw("l:=1;"); }
TEST(LongIntTests, Long3) { run_no_throw("while l>0 do l:=l-1;"); }
TEST(LongIntTests, Long4) {
  run_no_throw("function f: longint; begin f:=0; end;");
}
TEST(LongIntTests, Long5) { run_no_throw("for l:=1 to 5 do l:=l;"); }

// Dynamic memory
TEST(DynamicTests, Dyn1) { run_no_throw("new(p);"); }
TEST(DynamicTests, Dyn2) { run_no_throw("dispose(p);"); }
TEST(DynamicTests, Dyn3) { run_no_throw("var p:^integer;"); }
TEST(DynamicTests, Dyn4) { run_no_throw("p^:=1;"); }
TEST(DynamicTests, Dyn5) { run_no_throw("if p<>nil then dispose(p);"); }

// Strings
TEST(StringTests, Str1) { run_no_throw("var s: string;"); }
TEST(StringTests, Str2) { run_no_throw("s:='hi';"); }
TEST(StringTests, Str3) { run_no_throw("s:=s+'!';"); }
TEST(StringTests, Str4) { run_no_throw("writeln(s);"); }
TEST(StringTests, Str5) { run_no_throw("if s='' then s:='a';"); }

// Arrays
TEST(ArrayTests, Arr1) { run_no_throw("var a: array[1..5] of integer;"); }
TEST(ArrayTests, Arr2) { run_no_throw("a[1]:=0;"); }
TEST(ArrayTests, Arr3) { run_no_throw("for i:=1 to 5 do a[i]:=i;"); }
TEST(ArrayTests, Arr4) { run_no_throw("writeln(a[1]);"); }
TEST(ArrayTests, Arr5) { run_no_throw("if a[1]=0 then a[1]:=1;"); }

// Struct (record)
TEST(StructTests, Rec1) { run_no_throw("type r=record a:integer; end;"); }
TEST(StructTests, Rec2) { run_no_throw("var v:r;"); }
TEST(StructTests, Rec3) { run_no_throw("v.a:=1;"); }
TEST(StructTests, Rec4) { run_no_throw("with v do a:=2;"); }
TEST(StructTests, Rec5) { run_no_throw("if v.a=0 then v.a:=1;"); }

// Pointers
TEST(PointerTests, Ptr1) { run_no_throw("var p:^integer;"); }
TEST(PointerTests, Ptr2) { run_no_throw("new(p);"); }
TEST(PointerTests, Ptr3) { run_no_throw("p^:=1;"); }
TEST(PointerTests, Ptr4) { run_no_throw("dispose(p);"); }
TEST(PointerTests, Ptr5) { run_no_throw("p:=nil;"); }
