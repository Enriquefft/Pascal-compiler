#include "test_utils.hpp"
#include <gtest/gtest.h>

using pascal::AST;
using pascal::Lexer;
using pascal::Parser;
using pascal::Token;
using test_utils::run_full;
using TT = pascal::TokenType;

TEST(ComplexTests, FibonacciArray) {
  std::string src = R"(
program Fib;
var i: longint; f: array[1..10] of longint;
begin
  f[1]:=1; f[2]:=1;
  for i:=3 to 10 do
    f[i]:=f[i-1]+f[i-2];
end.)";
  std::vector<Token> expected_tokens =
      std::vector<Token>{{TT::Program, "program"},
                         {TT::Identifier, "Fib"},
                         {TT::Semicolon, ";"},
                         {TT::Var, "var"},
                         {TT::Identifier, "i"},
                         {TT::Colon, ":"},
                         {TT::Identifier, "longint"},
                         {TT::Semicolon, ";"},
                         {TT::Identifier, "f"},
                         {TT::Colon, ":"},
                         {TT::Array, "array"},
                         {TT::LeftBracket, "["},
                         {TT::Number, "1"},
                         {TT::Dot, "."},
                         {TT::Dot, "."},
                         {TT::Number, "10"},
                         {TT::RightBracket, "]"},
                         {TT::Of, "of"},
                         {TT::Identifier, "longint"},
                         {TT::Semicolon, ";"},
                         {TT::Begin, "begin"},
                         {TT::Identifier, "f"},
                         {TT::LeftBracket, "["},
                         {TT::Number, "1"},
                         {TT::RightBracket, "]"},
                         {TT::Colon, ":"},
                         {TT::Assign, "="},
                         {TT::Number, "1"},
                         {TT::Semicolon, ";"},
                         {TT::Identifier, "f"},
                         {TT::LeftBracket, "["},
                         {TT::Number, "2"},
                         {TT::RightBracket, "]"},
                         {TT::Colon, ":"},
                         {TT::Assign, "="},
                         {TT::Number, "1"},
                         {TT::Semicolon, ";"},
                         {TT::For, "for"},
                         {TT::Identifier, "i"},
                         {TT::Colon, ":"},
                         {TT::Assign, "="},
                         {TT::Number, "3"},
                         {TT::To, "to"},
                         {TT::Number, "10"},
                         {TT::Do, "do"},
                         {TT::Identifier, "f"},
                         {TT::LeftBracket, "["},
                         {TT::Identifier, "i"},
                         {TT::RightBracket, "]"},
                         {TT::Colon, ":"},
                         {TT::Assign, "="},
                         {TT::Identifier, "f"},
                         {TT::LeftBracket, "["},
                         {TT::Identifier, "i"},
                         {TT::Minus, "-"},
                         {TT::Number, "1"},
                         {TT::RightBracket, "]"},
                         {TT::Plus, "+"},
                         {TT::Identifier, "f"},
                         {TT::LeftBracket, "["},
                         {TT::Identifier, "i"},
                         {TT::Minus, "-"},
                         {TT::Number, "2"},
                         {TT::RightBracket, "]"},
                         {TT::Semicolon, ";"},
                         {TT::End, "end"},
                         {TT::Dot, "."},
                         {TT::EndOfFile, ""}};
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(src, expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, FactorialUnsigned) {
  std::string src = R"(
function fact(n: unsigned): unsigned;
begin
  if n=0 then fact:=1
  else fact:=n*fact(n-1);
end;
begin
  fact(5);
end.)";
  std::vector<Token> expected_tokens =
      std::vector<Token>{{TT::Function, "function"},
                         {TT::Identifier, "fact"},
                         {TT::LeftParen, "("},
                         {TT::Identifier, "n"},
                         {TT::Colon, ":"},
                         {TT::Identifier, "unsigned"},
                         {TT::RightParen, ")"},
                         {TT::Colon, ":"},
                         {TT::Identifier, "unsigned"},
                         {TT::Semicolon, ";"},
                         {TT::Begin, "begin"},
                         {TT::If, "if"},
                         {TT::Identifier, "n"},
                         {TT::Equal, "="},
                         {TT::Number, "0"},
                         {TT::Then, "then"},
                         {TT::Identifier, "fact"},
                         {TT::Colon, ":"},
                         {TT::Assign, "="},
                         {TT::Number, "1"},
                         {TT::Else, "else"},
                         {TT::Identifier, "fact"},
                         {TT::Colon, ":"},
                         {TT::Assign, "="},
                         {TT::Identifier, "n"},
                         {TT::Star, "*"},
                         {TT::Identifier, "fact"},
                         {TT::LeftParen, "("},
                         {TT::Identifier, "n"},
                         {TT::Minus, "-"},
                         {TT::Number, "1"},
                         {TT::RightParen, ")"},
                         {TT::Semicolon, ";"},
                         {TT::End, "end"},
                         {TT::Semicolon, ";"},
                         {TT::Begin, "begin"},
                         {TT::Identifier, "fact"},
                         {TT::LeftParen, "("},
                         {TT::Number, "5"},
                         {TT::RightParen, ")"},
                         {TT::Semicolon, ";"},
                         {TT::End, "end"},
                         {TT::Dot, "."},
                         {TT::EndOfFile, ""}};
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(src, expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, RecordPointer) {
  std::string src = R"(
type pr = ^rec;
     rec = record val: longint; next: pr; end;
var p: pr;
begin
  new(p); p^.val:=1; dispose(p);
end.)";
  std::vector<Token> expected_tokens =
      std::vector<Token>{{TT::Type, "type"},       {TT::Identifier, "pr"},
                         {TT::Equal, "="},         {TT::Caret, "^"},
                         {TT::Identifier, "rec"},  {TT::Semicolon, ";"},
                         {TT::Identifier, "rec"},  {TT::Equal, "="},
                         {TT::Record, "record"},   {TT::Identifier, "val"},
                         {TT::Colon, ":"},         {TT::Identifier, "longint"},
                         {TT::Semicolon, ";"},     {TT::Identifier, "next"},
                         {TT::Colon, ":"},         {TT::Identifier, "pr"},
                         {TT::Semicolon, ";"},     {TT::End, "end"},
                         {TT::Semicolon, ";"},     {TT::Var, "var"},
                         {TT::Identifier, "p"},    {TT::Colon, ":"},
                         {TT::Identifier, "pr"},   {TT::Semicolon, ";"},
                         {TT::Begin, "begin"},     {TT::New, "new"},
                         {TT::LeftParen, "("},     {TT::Identifier, "p"},
                         {TT::RightParen, ")"},    {TT::Semicolon, ";"},
                         {TT::Identifier, "p"},    {TT::Caret, "^"},
                         {TT::Dot, "."},           {TT::Identifier, "val"},
                         {TT::Colon, ":"},         {TT::Assign, "="},
                         {TT::Number, "1"},        {TT::Semicolon, ";"},
                         {TT::Dispose, "dispose"}, {TT::LeftParen, "("},
                         {TT::Identifier, "p"},    {TT::RightParen, ")"},
                         {TT::Semicolon, ";"},     {TT::End, "end"},
                         {TT::Dot, "."},           {TT::EndOfFile, ""}};
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(src, expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, StringConcat) {
  std::string src = R"(
var s: string;
begin
  s:='Hello';
  s:=s+' World';
end.)";
  std::vector<Token> expected_tokens =
      std::vector<Token>{{TT::Var, "var"},          {TT::Identifier, "s"},
                         {TT::Colon, ":"},          {TT::Identifier, "string"},
                         {TT::Semicolon, ";"},      {TT::Begin, "begin"},
                         {TT::Identifier, "s"},     {TT::Colon, ":"},
                         {TT::Assign, "="},         {TT::Identifier, "'"},
                         {TT::Identifier, "Hello"}, {TT::Identifier, "'"},
                         {TT::Semicolon, ";"},      {TT::Identifier, "s"},
                         {TT::Colon, ":"},          {TT::Assign, "="},
                         {TT::Identifier, "s"},     {TT::Plus, "+"},
                         {TT::Identifier, "'"},     {TT::Identifier, "World"},
                         {TT::Identifier, "'"},     {TT::Semicolon, ";"},
                         {TT::End, "end"},          {TT::Dot, "."},
                         {TT::EndOfFile, ""}};
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(src, expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, FloatArray) {
  std::string src = R"(
var a: array[1..3] of real; i: integer;
begin
  a[1]:=1.1; a[2]:=2.2; a[3]:=3.3;
  for i:=1 to 3 do a[i]:=a[i]*2.0;
end.)";
  std::vector<Token> expected_tokens =
      std::vector<Token>{{TT::Var, "var"},       {TT::Identifier, "a"},
                         {TT::Colon, ":"},       {TT::Array, "array"},
                         {TT::LeftBracket, "["}, {TT::Number, "1"},
                         {TT::Dot, "."},         {TT::Dot, "."},
                         {TT::Number, "3"},      {TT::RightBracket, "]"},
                         {TT::Of, "of"},         {TT::Identifier, "real"},
                         {TT::Semicolon, ";"},   {TT::Identifier, "i"},
                         {TT::Colon, ":"},       {TT::Identifier, "integer"},
                         {TT::Semicolon, ";"},   {TT::Begin, "begin"},
                         {TT::Identifier, "a"},  {TT::LeftBracket, "["},
                         {TT::Number, "1"},      {TT::RightBracket, "]"},
                         {TT::Colon, ":"},       {TT::Assign, "="},
                         {TT::Number, "1"},      {TT::Dot, "."},
                         {TT::Number, "1"},      {TT::Semicolon, ";"},
                         {TT::Identifier, "a"},  {TT::LeftBracket, "["},
                         {TT::Number, "2"},      {TT::RightBracket, "]"},
                         {TT::Colon, ":"},       {TT::Assign, "="},
                         {TT::Number, "2"},      {TT::Dot, "."},
                         {TT::Number, "2"},      {TT::Semicolon, ";"},
                         {TT::Identifier, "a"},  {TT::LeftBracket, "["},
                         {TT::Number, "3"},      {TT::RightBracket, "]"},
                         {TT::Colon, ":"},       {TT::Assign, "="},
                         {TT::Number, "3"},      {TT::Dot, "."},
                         {TT::Number, "3"},      {TT::Semicolon, ";"},
                         {TT::For, "for"},       {TT::Identifier, "i"},
                         {TT::Colon, ":"},       {TT::Assign, "="},
                         {TT::Number, "1"},      {TT::To, "to"},
                         {TT::Number, "3"},      {TT::Do, "do"},
                         {TT::Identifier, "a"},  {TT::LeftBracket, "["},
                         {TT::Identifier, "i"},  {TT::RightBracket, "]"},
                         {TT::Colon, ":"},       {TT::Assign, "="},
                         {TT::Identifier, "a"},  {TT::LeftBracket, "["},
                         {TT::Identifier, "i"},  {TT::RightBracket, "]"},
                         {TT::Star, "*"},        {TT::Number, "2"},
                         {TT::Dot, "."},         {TT::Number, "0"},
                         {TT::Semicolon, ";"},   {TT::End, "end"},
                         {TT::Dot, "."},         {TT::EndOfFile, ""}};
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(src, expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, PointerRecord) {
  std::string src = R"(
type r = record x: integer; end;
var p:^r;
begin
  new(p); p^.x:=5; dispose(p);
end.)";
  std::vector<Token> expected_tokens =
      std::vector<Token>{{TT::Type, "type"},
                         {TT::Identifier, "r"},
                         {TT::Equal, "="},
                         {TT::Record, "record"},
                         {TT::Identifier, "x"},
                         {TT::Colon, ":"},
                         {TT::Identifier, "integer"},
                         {TT::Semicolon, ";"},
                         {TT::End, "end"},
                         {TT::Semicolon, ";"},
                         {TT::Var, "var"},
                         {TT::Identifier, "p"},
                         {TT::Colon, ":"},
                         {TT::Caret, "^"},
                         {TT::Identifier, "r"},
                         {TT::Semicolon, ";"},
                         {TT::Begin, "begin"},
                         {TT::New, "new"},
                         {TT::LeftParen, "("},
                         {TT::Identifier, "p"},
                         {TT::RightParen, ")"},
                         {TT::Semicolon, ";"},
                         {TT::Identifier, "p"},
                         {TT::Caret, "^"},
                         {TT::Dot, "."},
                         {TT::Identifier, "x"},
                         {TT::Colon, ":"},
                         {TT::Assign, "="},
                         {TT::Number, "5"},
                         {TT::Semicolon, ";"},
                         {TT::Dispose, "dispose"},
                         {TT::LeftParen, "("},
                         {TT::Identifier, "p"},
                         {TT::RightParen, ")"},
                         {TT::Semicolon, ";"},
                         {TT::End, "end"},
                         {TT::Dot, "."},
                         {TT::RightParen, ")"},
                         {TT::EndOfFile, ""}};
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(src, expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, MatrixLongint) {
  std::string src = R"(
var m: array[1..2,1..2] of longint;
begin
  m[1,1]:=1; m[1,2]:=2;
  m[2,1]:=3; m[2,2]:=4;
end.)";
  std::vector<Token> expected_tokens =
      std::vector<Token>{{TT::Var, "var"},
                         {TT::Identifier, "m"},
                         {TT::Colon, ":"},
                         {TT::Array, "array"},
                         {TT::LeftBracket, "["},
                         {TT::Number, "1"},
                         {TT::Dot, "."},
                         {TT::Dot, "."},
                         {TT::Number, "2"},
                         {TT::Comma, ","},
                         {TT::Number, "1"},
                         {TT::Dot, "."},
                         {TT::Dot, "."},
                         {TT::Number, "2"},
                         {TT::RightBracket, "]"},
                         {TT::Of, "of"},
                         {TT::Identifier, "longint"},
                         {TT::Semicolon, ";"},
                         {TT::Begin, "begin"},
                         {TT::Identifier, "m"},
                         {TT::LeftBracket, "["},
                         {TT::Number, "1"},
                         {TT::Comma, ","},
                         {TT::Number, "1"},
                         {TT::RightBracket, "]"},
                         {TT::Colon, ":"},
                         {TT::Assign, "="},
                         {TT::Number, "1"},
                         {TT::Semicolon, ";"},
                         {TT::Identifier, "m"},
                         {TT::LeftBracket, "["},
                         {TT::Number, "1"},
                         {TT::Comma, ","},
                         {TT::Number, "2"},
                         {TT::RightBracket, "]"},
                         {TT::Colon, ":"},
                         {TT::Assign, "="},
                         {TT::Number, "2"},
                         {TT::Semicolon, ";"},
                         {TT::Identifier, "m"},
                         {TT::LeftBracket, "["},
                         {TT::Number, "2"},
                         {TT::Comma, ","},
                         {TT::Number, "1"},
                         {TT::RightBracket, "]"},
                         {TT::Colon, ":"},
                         {TT::Assign, "="},
                         {TT::Number, "3"},
                         {TT::Semicolon, ";"},
                         {TT::Identifier, "m"},
                         {TT::LeftBracket, "["},
                         {TT::Number, "2"},
                         {TT::Comma, ","},
                         {TT::Number, "2"},
                         {TT::RightBracket, "]"},
                         {TT::Colon, ":"},
                         {TT::Assign, "="},
                         {TT::Number, "4"},
                         {TT::Semicolon, ";"},
                         {TT::End, "end"},
                         {TT::Dot, "."},
                         {TT::EndOfFile, ""}};
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(src, expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, LinkedList) {
  std::string src = R"(
type nodep = ^node;
     node = record val: longint; next: nodep; end;
var head,node1: nodep;
begin
  new(head); new(node1);
  head^.next:=node1; node1^.next:=nil;
end.)";
  std::vector<Token> expected_tokens =
      std::vector<Token>{{TT::Type, "type"},
                         {TT::Identifier, "nodep"},
                         {TT::Equal, "="},
                         {TT::Caret, "^"},
                         {TT::Identifier, "node"},
                         {TT::Semicolon, ";"},
                         {TT::Identifier, "node"},
                         {TT::Equal, "="},
                         {TT::Record, "record"},
                         {TT::Identifier, "val"},
                         {TT::Colon, ":"},
                         {TT::Identifier, "longint"},
                         {TT::Semicolon, ";"},
                         {TT::Identifier, "next"},
                         {TT::Colon, ":"},
                         {TT::Identifier, "nodep"},
                         {TT::Semicolon, ";"},
                         {TT::End, "end"},
                         {TT::Semicolon, ";"},
                         {TT::Var, "var"},
                         {TT::Identifier, "head"},
                         {TT::Comma, ","},
                         {TT::Identifier, "node1"},
                         {TT::Colon, ":"},
                         {TT::Identifier, "nodep"},
                         {TT::Semicolon, ";"},
                         {TT::Begin, "begin"},
                         {TT::New, "new"},
                         {TT::LeftParen, "("},
                         {TT::Identifier, "head"},
                         {TT::RightParen, ")"},
                         {TT::Semicolon, ";"},
                         {TT::New, "new"},
                         {TT::LeftParen, "("},
                         {TT::Identifier, "node1"},
                         {TT::RightParen, ")"},
                         {TT::Semicolon, ";"},
                         {TT::Identifier, "head"},
                         {TT::Caret, "^"},
                         {TT::Dot, "."},
                         {TT::Identifier, "next"},
                         {TT::Colon, ":"},
                         {TT::Assign, "="},
                         {TT::Identifier, "node1"},
                         {TT::Semicolon, ";"},
                         {TT::Identifier, "node1"},
                         {TT::Caret, "^"},
                         {TT::Dot, "."},
                         {TT::Identifier, "next"},
                         {TT::Colon, ":"},
                         {TT::Assign, "="},
                         {TT::Identifier, "nil"},
                         {TT::Semicolon, ";"},
                         {TT::End, "end"},
                         {TT::Dot, "."},
                         {TT::EndOfFile, ""}};
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(src, expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, DynamicFloatArray) {
  std::string src = R"(
type arrp = ^array[1..5] of real;
var p: arrp;
begin
  new(p); p^[1]:=1.0; dispose(p);
end.)";
  std::vector<Token> expected_tokens =
      std::vector<Token>{{TT::Type, "type"},       {TT::Identifier, "arrp"},
                         {TT::Equal, "="},         {TT::Caret, "^"},
                         {TT::Array, "array"},     {TT::LeftBracket, "["},
                         {TT::Number, "1"},        {TT::Dot, "."},
                         {TT::Dot, "."},           {TT::Number, "5"},
                         {TT::RightBracket, "]"},  {TT::Of, "of"},
                         {TT::Identifier, "real"}, {TT::Semicolon, ";"},
                         {TT::Var, "var"},         {TT::Identifier, "p"},
                         {TT::Colon, ":"},         {TT::Identifier, "arrp"},
                         {TT::Semicolon, ";"},     {TT::Begin, "begin"},
                         {TT::New, "new"},         {TT::LeftParen, "("},
                         {TT::Identifier, "p"},    {TT::RightParen, ")"},
                         {TT::Semicolon, ";"},     {TT::Identifier, "p"},
                         {TT::Caret, "^"},         {TT::LeftBracket, "["},
                         {TT::Number, "1"},        {TT::RightBracket, "]"},
                         {TT::Colon, ":"},         {TT::Assign, "="},
                         {TT::Number, "1"},        {TT::Dot, "."},
                         {TT::Number, "0"},        {TT::Semicolon, ";"},
                         {TT::Dispose, "dispose"}, {TT::LeftParen, "("},
                         {TT::Identifier, "p"},    {TT::RightParen, ")"},
                         {TT::Semicolon, ";"},     {TT::End, "end"},
                         {TT::Dot, "."},           {TT::EndOfFile, ""}};
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(src, expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, SortLongint) {
  std::string src = R"(
var a: array[1..3] of longint; i,j,t: longint;
begin
  for i:=1 to 2 do
    for j:=i+1 to 3 do
      if a[i]>a[j] then
        begin t:=a[i]; a[i]:=a[j]; a[j]:=t; end;
end.)";
  std::vector<Token> expected_tokens =
      std::vector<Token>{{TT::Var, "var"},        {TT::Identifier, "a"},
                         {TT::Colon, ":"},        {TT::Array, "array"},
                         {TT::LeftBracket, "["},  {TT::Number, "1"},
                         {TT::Dot, "."},          {TT::Dot, "."},
                         {TT::Number, "3"},       {TT::RightBracket, "]"},
                         {TT::Of, "of"},          {TT::Identifier, "longint"},
                         {TT::Semicolon, ";"},    {TT::Identifier, "i"},
                         {TT::Comma, ","},        {TT::Identifier, "j"},
                         {TT::Comma, ","},        {TT::Identifier, "t"},
                         {TT::Colon, ":"},        {TT::Identifier, "longint"},
                         {TT::Semicolon, ";"},    {TT::Begin, "begin"},
                         {TT::For, "for"},        {TT::Identifier, "i"},
                         {TT::Colon, ":"},        {TT::Assign, "="},
                         {TT::Number, "1"},       {TT::To, "to"},
                         {TT::Number, "2"},       {TT::Do, "do"},
                         {TT::For, "for"},        {TT::Identifier, "j"},
                         {TT::Colon, ":"},        {TT::Assign, "="},
                         {TT::Identifier, "i"},   {TT::Plus, "+"},
                         {TT::Number, "1"},       {TT::To, "to"},
                         {TT::Number, "3"},       {TT::Do, "do"},
                         {TT::If, "if"},          {TT::Identifier, "a"},
                         {TT::LeftBracket, "["},  {TT::Identifier, "i"},
                         {TT::RightBracket, "]"}, {TT::Greater, ">"},
                         {TT::Identifier, "a"},   {TT::LeftBracket, "["},
                         {TT::Identifier, "j"},   {TT::RightBracket, "]"},
                         {TT::Then, "then"},      {TT::Begin, "begin"},
                         {TT::Identifier, "t"},   {TT::Colon, ":"},
                         {TT::Assign, "="},       {TT::Identifier, "a"},
                         {TT::LeftBracket, "["},  {TT::Identifier, "i"},
                         {TT::RightBracket, "]"}, {TT::Semicolon, ";"},
                         {TT::Identifier, "a"},   {TT::LeftBracket, "["},
                         {TT::Identifier, "i"},   {TT::RightBracket, "]"},
                         {TT::Colon, ":"},        {TT::Assign, "="},
                         {TT::Identifier, "a"},   {TT::LeftBracket, "["},
                         {TT::Identifier, "j"},   {TT::RightBracket, "]"},
                         {TT::Semicolon, ";"},    {TT::Identifier, "a"},
                         {TT::LeftBracket, "["},  {TT::Identifier, "j"},
                         {TT::RightBracket, "]"}, {TT::Colon, ":"},
                         {TT::Assign, "="},       {TT::Identifier, "t"},
                         {TT::Semicolon, ";"},    {TT::End, "end"},
                         {TT::Semicolon, ";"},    {TT::End, "end"},
                         {TT::Dot, "."},          {TT::EndOfFile, ""}};
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(src, expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, ArrayOfStrings) {
  std::string src = R"(
var names: array[1..2] of string;
begin
  names[1]:='Alice';
  names[2]:='Bob';
end.)";
  std::vector<Token> expected_tokens =
      std::vector<Token>{{TT::Var, "var"},
                         {TT::Identifier, "names"},
                         {TT::Colon, ":"},
                         {TT::Array, "array"},
                         {TT::LeftBracket, "["},
                         {TT::Number, "1"},
                         {TT::Dot, "."},
                         {TT::Dot, "."},
                         {TT::Number, "2"},
                         {TT::RightBracket, "]"},
                         {TT::Of, "of"},
                         {TT::Identifier, "string"},
                         {TT::Semicolon, ";"},
                         {TT::Begin, "begin"},
                         {TT::Identifier, "names"},
                         {TT::LeftBracket, "["},
                         {TT::Number, "1"},
                         {TT::RightBracket, "]"},
                         {TT::Colon, ":"},
                         {TT::Assign, "="},
                         {TT::Identifier, "'"},
                         {TT::Identifier, "Alice"},
                         {TT::Identifier, "'"},
                         {TT::Semicolon, ";"},
                         {TT::Identifier, "names"},
                         {TT::LeftBracket, "["},
                         {TT::Number, "2"},
                         {TT::RightBracket, "]"},
                         {TT::Colon, ":"},
                         {TT::Assign, "="},
                         {TT::Identifier, "'"},
                         {TT::Identifier, "Bob"},
                         {TT::Identifier, "'"},
                         {TT::Semicolon, ";"},
                         {TT::End, "end"},
                         {TT::Dot, "."},
                         {TT::EndOfFile, ""}};
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(src, expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, PointerRecordString) {
  std::string src = R"(
type person = record name: string; end;
var p:^person;
begin
  new(p); p^.name:='Ana'; dispose(p);
end.)";
  std::vector<Token> expected_tokens =
      std::vector<Token>{{TT::Type, "type"},
                         {TT::Identifier, "person"},
                         {TT::Equal, "="},
                         {TT::Record, "record"},
                         {TT::Identifier, "name"},
                         {TT::Colon, ":"},
                         {TT::Identifier, "string"},
                         {TT::Semicolon, ";"},
                         {TT::End, "end"},
                         {TT::Semicolon, ";"},
                         {TT::Var, "var"},
                         {TT::Identifier, "p"},
                         {TT::Colon, ":"},
                         {TT::Caret, "^"},
                         {TT::Identifier, "person"},
                         {TT::Semicolon, ";"},
                         {TT::Begin, "begin"},
                         {TT::New, "new"},
                         {TT::LeftParen, "("},
                         {TT::Identifier, "p"},
                         {TT::RightParen, ")"},
                         {TT::Semicolon, ";"},
                         {TT::Identifier, "p"},
                         {TT::Caret, "^"},
                         {TT::Dot, "."},
                         {TT::Identifier, "name"},
                         {TT::Colon, ":"},
                         {TT::Assign, "="},
                         {TT::Identifier, "'"},
                         {TT::Identifier, "Ana"},
                         {TT::Identifier, "'"},
                         {TT::Semicolon, ";"},
                         {TT::Dispose, "dispose"},
                         {TT::LeftParen, "("},
                         {TT::Identifier, "p"},
                         {TT::RightParen, ")"},
                         {TT::Semicolon, ";"},
                         {TT::End, "end"},
                         {TT::Dot, "."},
                         {TT::EndOfFile, ""}};
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(src, expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, NestedRecordPointer) {
  std::string src = R"(
type inner = record a: longint; end;
     outer = record i:^inner; end;
var o: outer;
begin
  new(o.i); o.i^.a:=10; dispose(o.i);
end.)";
  std::vector<Token> expected_tokens =
      std::vector<Token>{{TT::Type, "type"},
                         {TT::Identifier, "inner"},
                         {TT::Equal, "="},
                         {TT::Record, "record"},
                         {TT::Identifier, "a"},
                         {TT::Colon, ":"},
                         {TT::Identifier, "longint"},
                         {TT::Semicolon, ";"},
                         {TT::End, "end"},
                         {TT::Semicolon, ";"},
                         {TT::Identifier, "outer"},
                         {TT::Equal, "="},
                         {TT::Record, "record"},
                         {TT::Identifier, "i"},
                         {TT::Colon, ":"},
                         {TT::Caret, "^"},
                         {TT::Identifier, "inner"},
                         {TT::Semicolon, ";"},
                         {TT::End, "end"},
                         {TT::Semicolon, ";"},
                         {TT::Var, "var"},
                         {TT::Identifier, "o"},
                         {TT::Colon, ":"},
                         {TT::Identifier, "outer"},
                         {TT::Semicolon, ";"},
                         {TT::Begin, "begin"},
                         {TT::New, "new"},
                         {TT::LeftParen, "("},
                         {TT::Identifier, "o"},
                         {TT::Dot, "."},
                         {TT::Identifier, "i"},
                         {TT::RightParen, ")"},
                         {TT::Semicolon, ";"},
                         {TT::Identifier, "o"},
                         {TT::Dot, "."},
                         {TT::Identifier, "i"},
                         {TT::Caret, "^"},
                         {TT::Dot, "."},
                         {TT::Identifier, "a"},
                         {TT::Colon, ":"},
                         {TT::Assign, "="},
                         {TT::Number, "10"},
                         {TT::Semicolon, ";"},
                         {TT::Dispose, "dispose"},
                         {TT::LeftParen, "("},
                         {TT::Identifier, "o"},
                         {TT::Dot, "."},
                         {TT::Identifier, "i"},
                         {TT::RightParen, ")"},
                         {TT::Semicolon, ";"},
                         {TT::End, "end"},
                         {TT::Dot, "."},
                         {TT::RightParen, ")"},
                         {TT::EndOfFile, ""}};
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(src, expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, PointerMath) {
  std::string src = R"(
var p:^longint;
begin
  new(p); p^:=2; p^:=p^*3; dispose(p);
end.)";
  std::vector<Token> expected_tokens =
      std::vector<Token>{{TT::Var, "var"},
                         {TT::Identifier, "p"},
                         {TT::Colon, ":"},
                         {TT::Caret, "^"},
                         {TT::Identifier, "longint"},
                         {TT::Semicolon, ";"},
                         {TT::Begin, "begin"},
                         {TT::New, "new"},
                         {TT::LeftParen, "("},
                         {TT::Identifier, "p"},
                         {TT::RightParen, ")"},
                         {TT::Semicolon, ";"},
                         {TT::Identifier, "p"},
                         {TT::Caret, "^"},
                         {TT::Colon, ":"},
                         {TT::Assign, "="},
                         {TT::Number, "2"},
                         {TT::Semicolon, ";"},
                         {TT::Identifier, "p"},
                         {TT::Caret, "^"},
                         {TT::Colon, ":"},
                         {TT::Assign, "="},
                         {TT::Identifier, "p"},
                         {TT::Caret, "^"},
                         {TT::Star, "*"},
                         {TT::Number, "3"},
                         {TT::Semicolon, ";"},
                         {TT::Dispose, "dispose"},
                         {TT::LeftParen, "("},
                         {TT::Identifier, "p"},
                         {TT::RightParen, ")"},
                         {TT::Semicolon, ";"},
                         {TT::End, "end"},
                         {TT::Dot, "."},
                         {TT::EndOfFile, ""}};
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(src, expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, RecordArrayDynamic) {
  std::string src = R"(
type item = record val: unsigned; end;
     itemArr = ^array[1..10] of item;
var p: itemArr;
begin
  new(p); p^[1].val:=0; dispose(p);
end.)";
  std::vector<Token> expected_tokens =
      std::vector<Token>{{TT::Type, "type"},
                         {TT::Identifier, "item"},
                         {TT::Equal, "="},
                         {TT::Record, "record"},
                         {TT::Identifier, "val"},
                         {TT::Colon, ":"},
                         {TT::Identifier, "unsigned"},
                         {TT::Semicolon, ";"},
                         {TT::End, "end"},
                         {TT::Semicolon, ";"},
                         {TT::Identifier, "itemArr"},
                         {TT::Equal, "="},
                         {TT::Caret, "^"},
                         {TT::Array, "array"},
                         {TT::LeftBracket, "["},
                         {TT::Number, "1"},
                         {TT::Dot, "."},
                         {TT::Dot, "."},
                         {TT::Number, "10"},
                         {TT::RightBracket, "]"},
                         {TT::Of, "of"},
                         {TT::Identifier, "item"},
                         {TT::Semicolon, ";"},
                         {TT::Var, "var"},
                         {TT::Identifier, "p"},
                         {TT::Colon, ":"},
                         {TT::Identifier, "itemArr"},
                         {TT::Semicolon, ";"},
                         {TT::Begin, "begin"},
                         {TT::New, "new"},
                         {TT::LeftParen, "("},
                         {TT::Identifier, "p"},
                         {TT::RightParen, ")"},
                         {TT::Semicolon, ";"},
                         {TT::Identifier, "p"},
                         {TT::Caret, "^"},
                         {TT::LeftBracket, "["},
                         {TT::Number, "1"},
                         {TT::RightBracket, "]"},
                         {TT::Dot, "."},
                         {TT::Identifier, "val"},
                         {TT::Colon, ":"},
                         {TT::Assign, "="},
                         {TT::Number, "0"},
                         {TT::Semicolon, ";"},
                         {TT::Dispose, "dispose"},
                         {TT::LeftParen, "("},
                         {TT::Identifier, "p"},
                         {TT::RightParen, ")"},
                         {TT::Semicolon, ";"},
                         {TT::End, "end"},
                         {TT::Dot, "."},
                         {TT::EndOfFile, ""}};
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(src, expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, PointerToUIntArray) {
  std::string src = R"(
type uintArr = ^array[1..5] of unsigned;
var p: uintArr;
begin
  new(p); p^[5]:=10; dispose(p);
end.)";
  std::vector<Token> expected_tokens =
      std::vector<Token>{{TT::Type, "type"},
                         {TT::Identifier, "uintArr"},
                         {TT::Equal, "="},
                         {TT::Caret, "^"},
                         {TT::Array, "array"},
                         {TT::LeftBracket, "["},
                         {TT::Number, "1"},
                         {TT::Dot, "."},
                         {TT::Dot, "."},
                         {TT::Number, "5"},
                         {TT::RightBracket, "]"},
                         {TT::Of, "of"},
                         {TT::Identifier, "unsigned"},
                         {TT::Semicolon, ";"},
                         {TT::Var, "var"},
                         {TT::Identifier, "p"},
                         {TT::Colon, ":"},
                         {TT::Identifier, "uintArr"},
                         {TT::Semicolon, ";"},
                         {TT::Begin, "begin"},
                         {TT::New, "new"},
                         {TT::LeftParen, "("},
                         {TT::Identifier, "p"},
                         {TT::RightParen, ")"},
                         {TT::Semicolon, ";"},
                         {TT::Identifier, "p"},
                         {TT::Caret, "^"},
                         {TT::LeftBracket, "["},
                         {TT::Number, "5"},
                         {TT::RightBracket, "]"},
                         {TT::Colon, ":"},
                         {TT::Assign, "="},
                         {TT::Number, "10"},
                         {TT::Semicolon, ";"},
                         {TT::Dispose, "dispose"},
                         {TT::LeftParen, "("},
                         {TT::Identifier, "p"},
                         {TT::RightParen, ")"},
                         {TT::Semicolon, ";"},
                         {TT::End, "end"},
                         {TT::Dot, "."},
                         {TT::EndOfFile, ""}};
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(src, expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, StructWithMatrix) {
  std::string src = R"(
type mat = record m: array[1..2,1..2] of longint; end;
var v: mat;
begin
  v.m[1,1]:=1;
end.)";
  std::vector<Token> expected_tokens =
      std::vector<Token>{{TT::Type, "type"},      {TT::Identifier, "mat"},
                         {TT::Equal, "="},        {TT::Record, "record"},
                         {TT::Identifier, "m"},   {TT::Colon, ":"},
                         {TT::Array, "array"},    {TT::LeftBracket, "["},
                         {TT::Number, "1"},       {TT::Dot, "."},
                         {TT::Dot, "."},          {TT::Number, "2"},
                         {TT::Comma, ","},        {TT::Number, "1"},
                         {TT::Dot, "."},          {TT::Dot, "."},
                         {TT::Number, "2"},       {TT::RightBracket, "]"},
                         {TT::Of, "of"},          {TT::Identifier, "longint"},
                         {TT::Semicolon, ";"},    {TT::End, "end"},
                         {TT::Semicolon, ";"},    {TT::Var, "var"},
                         {TT::Identifier, "v"},   {TT::Colon, ":"},
                         {TT::Identifier, "mat"}, {TT::Semicolon, ";"},
                         {TT::Begin, "begin"},    {TT::Identifier, "v"},
                         {TT::Dot, "."},          {TT::Identifier, "m"},
                         {TT::LeftBracket, "["},  {TT::Number, "1"},
                         {TT::Comma, ","},        {TT::Number, "1"},
                         {TT::RightBracket, "]"}, {TT::Colon, ":"},
                         {TT::Assign, "="},       {TT::Number, "1"},
                         {TT::Semicolon, ";"},    {TT::End, "end"},
                         {TT::Dot, "."},          {TT::EndOfFile, ""}};
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(src, expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, FloatPointer) {
  std::string src = R"(
var p:^real;
begin
  new(p); p^:=3.14; dispose(p);
end.)";
  std::vector<Token> expected_tokens = std::vector<Token>{
      {TT::Var, "var"},      {TT::Identifier, "p"},    {TT::Colon, ":"},
      {TT::Caret, "^"},      {TT::Identifier, "real"}, {TT::Semicolon, ";"},
      {TT::Begin, "begin"},  {TT::New, "new"},         {TT::LeftParen, "("},
      {TT::Identifier, "p"}, {TT::RightParen, ")"},    {TT::Semicolon, ";"},
      {TT::Identifier, "p"}, {TT::Caret, "^"},         {TT::Colon, ":"},
      {TT::Assign, "="},     {TT::Number, "3"},        {TT::Dot, "."},
      {TT::Number, "14"},    {TT::Semicolon, ";"},     {TT::Dispose, "dispose"},
      {TT::LeftParen, "("},  {TT::Identifier, "p"},    {TT::RightParen, ")"},
      {TT::Semicolon, ";"},  {TT::End, "end"},         {TT::Dot, "."},
      {TT::EndOfFile, ""}};
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(src, expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, DynamicStringPointer) {
  std::string src = R"(
type sp = ^string;
var p: sp;
begin
  new(p); p^:='hi'; dispose(p);
end.)";
  std::vector<Token> expected_tokens =
      std::vector<Token>{{TT::Type, "type"},         {TT::Identifier, "sp"},
                         {TT::Equal, "="},           {TT::Caret, "^"},
                         {TT::Identifier, "string"}, {TT::Semicolon, ";"},
                         {TT::Var, "var"},           {TT::Identifier, "p"},
                         {TT::Colon, ":"},           {TT::Identifier, "sp"},
                         {TT::Semicolon, ";"},       {TT::Begin, "begin"},
                         {TT::New, "new"},           {TT::LeftParen, "("},
                         {TT::Identifier, "p"},      {TT::RightParen, ")"},
                         {TT::Semicolon, ";"},       {TT::Identifier, "p"},
                         {TT::Caret, "^"},           {TT::Colon, ":"},
                         {TT::Assign, "="},          {TT::Identifier, "'"},
                         {TT::Identifier, "hi"},     {TT::Identifier, "'"},
                         {TT::Semicolon, ";"},       {TT::Dispose, "dispose"},
                         {TT::LeftParen, "("},       {TT::Identifier, "p"},
                         {TT::RightParen, ")"},      {TT::Semicolon, ";"},
                         {TT::End, "end"},           {TT::Dot, "."},
                         {TT::EndOfFile, ""}};
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(src, expected_tokens, expected_ast, expected_asm, expected_output);
}

TEST(ComplexTests, FibonacciRecursive) {
  std::string src = R"(
function fib(n: longint): longint;
begin
  if n<2 then fib:=n else fib:=fib(n-1)+fib(n-2);
end;
begin
  fib(5);
end.)";
  std::vector<Token> expected_tokens =
      std::vector<Token>{{TT::Function, "function"},
                         {TT::Identifier, "fib"},
                         {TT::LeftParen, "("},
                         {TT::Identifier, "n"},
                         {TT::Colon, ":"},
                         {TT::Identifier, "longint"},
                         {TT::RightParen, ")"},
                         {TT::Colon, ":"},
                         {TT::Identifier, "longint"},
                         {TT::Semicolon, ";"},
                         {TT::Begin, "begin"},
                         {TT::If, "if"},
                         {TT::Identifier, "n"},
                         {TT::Less, "<"},
                         {TT::Number, "2"},
                         {TT::Then, "then"},
                         {TT::Identifier, "fib"},
                         {TT::Colon, ":"},
                         {TT::Assign, "="},
                         {TT::Identifier, "n"},
                         {TT::Else, "else"},
                         {TT::Identifier, "fib"},
                         {TT::Colon, ":"},
                         {TT::Assign, "="},
                         {TT::Identifier, "fib"},
                         {TT::LeftParen, "("},
                         {TT::Identifier, "n"},
                         {TT::Minus, "-"},
                         {TT::Number, "1"},
                         {TT::RightParen, ")"},
                         {TT::Plus, "+"},
                         {TT::Identifier, "fib"},
                         {TT::LeftParen, "("},
                         {TT::Identifier, "n"},
                         {TT::Minus, "-"},
                         {TT::Number, "2"},
                         {TT::RightParen, ")"},
                         {TT::Semicolon, ";"},
                         {TT::End, "end"},
                         {TT::Semicolon, ";"},
                         {TT::Begin, "begin"},
                         {TT::Identifier, "fib"},
                         {TT::LeftParen, "("},
                         {TT::Number, "5"},
                         {TT::RightParen, ")"},
                         {TT::Semicolon, ";"},
                         {TT::End, "end"},
                         {TT::Dot, "."},
                         {TT::EndOfFile, ""}};
  AST expected_ast{};
  expected_ast.valid = true;
  std::string expected_asm = "section .text";
  std::string expected_output = "";
  run_full(src, expected_tokens, expected_ast, expected_asm, expected_output);
}
