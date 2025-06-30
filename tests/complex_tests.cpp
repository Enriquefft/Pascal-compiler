#include "test_utils.hpp"
#include <gtest/gtest.h>

using pascal::AST;
using pascal::Lexer;
using pascal::Parser;
using pascal::Token;
using test_utils::make_empty_ast;
using test_utils::run_full;
using test_utils::tokens;
using TT = pascal::TokenType;

TEST(ComplexTests, FibonacciArray) {
  std::vector<Token> expected_tokens = tokens({{TT::Program, "program"}});
  AST expected_ast = make_empty_ast();
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(R"(
program Fib;
var i: longint; f: array[1..10] of longint;
begin
  f[1]:=1; f[2]:=1;
  for i:=3 to 10 do
    f[i]:=f[i-1]+f[i-2];
end.)",
           expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, FactorialUnsigned) {
  std::vector<Token> expected_tokens = tokens({{TT::Function, "function"}});
  AST expected_ast = make_empty_ast();
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(R"(
function fact(n: unsigned): unsigned;
begin
  if n=0 then fact:=1
  else fact:=n*fact(n-1);
end;
begin
  fact(5);
end.)",
           expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, RecordPointer) {
  std::vector<Token> expected_tokens = tokens({{TT::Type, "type"}});
  AST expected_ast = make_empty_ast();
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(R"(
type pr = ^rec;
     rec = record val: longint; next: pr; end;
var p: pr;
begin
  new(p); p^.val:=1; dispose(p);
end.)",
           expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, StringConcat) {
  std::vector<Token> expected_tokens = tokens({{TT::Var, "var"}});
  AST expected_ast = make_empty_ast();
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(R"(
var s: string;
begin
  s:='Hello';
  s:=s+' World';
end.)",
           expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, FloatArray) {
  std::vector<Token> expected_tokens = tokens({{TT::Var, "var"}});
  AST expected_ast = make_empty_ast();
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(R"(
var a: array[1..3] of real; i: integer;
begin
  a[1]:=1.1; a[2]:=2.2; a[3]:=3.3;
  for i:=1 to 3 do a[i]:=a[i]*2.0;
end.)",
           expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, PointerRecord) {
  std::vector<Token> expected_tokens = tokens({{TT::Type, "type"}});
  AST expected_ast = make_empty_ast();
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(R"(
type r = record x: integer; end;
var p:^r;
begin
  new(p); p^.x:=5; dispose(p);
end.)",
           expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, MatrixLongint) {
  std::vector<Token> expected_tokens = tokens({{TT::Var, "var"}});
  AST expected_ast = make_empty_ast();
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(R"(
var m: array[1..2,1..2] of longint;
begin
  m[1,1]:=1; m[1,2]:=2;
  m[2,1]:=3; m[2,2]:=4;
end.)",
           expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, LinkedList) {
  std::vector<Token> expected_tokens = tokens({{TT::Type, "type"}});
  AST expected_ast = make_empty_ast();
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(R"(
type nodep = ^node;
     node = record val: longint; next: nodep; end;
var head,node1: nodep;
begin
  new(head); new(node1);
  head^.next:=node1; node1^.next:=nil;
end.)",
           expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, DynamicFloatArray) {
  std::vector<Token> expected_tokens = tokens({{TT::Type, "type"}});
  AST expected_ast = make_empty_ast();
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(R"(
type arrp = ^array[1..5] of real;
var p: arrp;
begin
  new(p); p^[1]:=1.0; dispose(p);
end.)",
           expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, SortLongint) {
  std::vector<Token> expected_tokens = tokens({{TT::Var, "var"}});
  AST expected_ast = make_empty_ast();
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(R"(
var a: array[1..3] of longint; i,j,t: longint;
begin
  for i:=1 to 2 do
    for j:=i+1 to 3 do
      if a[i]>a[j] then
        begin t:=a[i]; a[i]:=a[j]; a[j]:=t; end;
end.)",
           expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, ArrayOfStrings) {
  std::vector<Token> expected_tokens = tokens({{TT::Var, "var"}});
  AST expected_ast = make_empty_ast();
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(R"(
var names: array[1..2] of string;
begin
  names[1]:='Alice';
  names[2]:='Bob';
end.)",
           expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, PointerRecordString) {
  std::vector<Token> expected_tokens = tokens({{TT::Type, "type"}});
  AST expected_ast = make_empty_ast();
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(R"(
type person = record name: string; end;
var p:^person;
begin
  new(p); p^.name:='Ana'; dispose(p);
end.)",
           expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, NestedRecordPointer) {
  std::vector<Token> expected_tokens = tokens({{TT::Type, "type"}});
  AST expected_ast = make_empty_ast();
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(R"(
type inner = record a: longint; end;
     outer = record i:^inner; end;
var o: outer;
begin
  new(o.i); o.i^.a:=10; dispose(o.i);
end.)",
           expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, PointerMath) {
  std::vector<Token> expected_tokens = tokens({{TT::Var, "var"}});
  AST expected_ast = make_empty_ast();
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(R"(
var p:^longint;
begin
  new(p); p^:=2; p^:=p^*3; dispose(p);
end.)",
           expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, RecordArrayDynamic) {
  std::vector<Token> expected_tokens = tokens({{TT::Type, "type"}});
  AST expected_ast = make_empty_ast();
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(R"(
type item = record val: unsigned; end;
     itemArr = ^array[1..10] of item;
var p: itemArr;
begin
  new(p); p^[1].val:=0; dispose(p);
end.)",
           expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, PointerToUIntArray) {
  std::vector<Token> expected_tokens = tokens({{TT::Type, "type"}});
  AST expected_ast = make_empty_ast();
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(R"(
type uintArr = ^array[1..5] of unsigned;
var p: uintArr;
begin
  new(p); p^[5]:=10; dispose(p);
end.)",
           expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, StructWithMatrix) {
  std::vector<Token> expected_tokens = tokens({{TT::Type, "type"}});
  AST expected_ast = make_empty_ast();
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(R"(
type mat = record m: array[1..2,1..2] of longint; end;
var v: mat;
begin
  v.m[1,1]:=1;
end.)",
           expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, FloatPointer) {
  std::vector<Token> expected_tokens = tokens({{TT::Var, "var"}});
  AST expected_ast = make_empty_ast();
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(R"(
var p:^real;
begin
  new(p); p^:=3.14; dispose(p);
end.)",
           expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, DynamicStringPointer) {
  std::vector<Token> expected_tokens = tokens({{TT::Type, "type"}});
  AST expected_ast = make_empty_ast();
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(R"(
type sp = ^string;
var p: sp;
begin
  new(p); p^:='hi'; dispose(p);
end.)",
           expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, FibonacciRecursive) {
  std::vector<Token> expected_tokens = tokens({{TT::Function, "function"}});
  AST expected_ast = make_empty_ast();
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(R"(
function fib(n: longint): longint;
begin
  if n<2 then fib:=n else fib:=fib(n-1)+fib(n-2);
end;
begin
  fib(5);
end.)",
           expected_tokens, expected_ast, expected_asm, expected_output);
}
