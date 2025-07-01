#pragma once

#include "parser/parser.hpp"
#include "parser/validator.hpp"
#include "scanner/lexer.hpp"
#include "visitors/codegen.hpp"
#include <cctype>
#include <gtest/gtest.h>
#include <string_view>
#include <vector>

namespace test_utils {
using pascal::AST;
using pascal::ASTValidator;
using pascal::CodeGenerator;
using pascal::Lexer;
using pascal::Parser;
using pascal::Token;
using pascal::TokenType;

enum class TestMode { Tokens, TokensAst, TokensAstAsm, All };

inline constexpr TestMode TEST_MODE = TestMode::TokensAstAsm;

inline std::string execute_stub(std::string_view /*asm_code*/) { return {}; }

inline bool ast_equal_node(const pascal::ASTNode *a, const pascal::ASTNode *b) {
  if (!a || !b)
    return a == b;
  if (a->kind != b->kind)
    return false;
  using pascal::NodeKind;
  switch (a->kind) {
  case NodeKind::Program: {
    auto pa = static_cast<const pascal::Program *>(a);
    auto pb = static_cast<const pascal::Program *>(b);
    if (pa->name != pb->name)
      return false;
    return ast_equal_node(pa->block.get(), pb->block.get());
  }
  case NodeKind::Block: {
    auto ba = static_cast<const pascal::Block *>(a);
    auto bb = static_cast<const pascal::Block *>(b);
    if (ba->declarations.size() != bb->declarations.size() ||
        ba->statements.size() != bb->statements.size())
      return false;
    for (size_t i = 0; i < ba->declarations.size(); ++i)
      if (!ast_equal_node(ba->declarations[i].get(), bb->declarations[i].get()))
        return false;
    for (size_t i = 0; i < ba->statements.size(); ++i)
      if (!ast_equal_node(ba->statements[i].get(), bb->statements[i].get()))
        return false;
    return true;
  }
  case NodeKind::AssignStmt: {
    auto aa = static_cast<const pascal::AssignStmt *>(a);
    auto ab = static_cast<const pascal::AssignStmt *>(b);
    return ast_equal_node(aa->target.get(), ab->target.get()) &&
           ast_equal_node(aa->value.get(), ab->value.get());
  }
  case NodeKind::BinaryExpr: {
    auto ba = static_cast<const pascal::BinaryExpr *>(a);
    auto bb = static_cast<const pascal::BinaryExpr *>(b);
    return ba->op == bb->op && ast_equal_node(ba->left.get(), bb->left.get()) &&
           ast_equal_node(ba->right.get(), bb->right.get());
  }
  case NodeKind::LiteralExpr: {
    auto la = static_cast<const pascal::LiteralExpr *>(a);
    auto lb = static_cast<const pascal::LiteralExpr *>(b);
    return la->value == lb->value;
  }
  case NodeKind::VariableExpr: {
    auto va = static_cast<const pascal::VariableExpr *>(a);
    auto vb = static_cast<const pascal::VariableExpr *>(b);
    if (va->name != vb->name || va->selectors.size() != vb->selectors.size())
      return false;
    for (size_t i = 0; i < va->selectors.size(); ++i) {
      const auto &sa = va->selectors[i];
      const auto &sb = vb->selectors[i];
      if (sa.kind != sb.kind)
        return false;
      switch (sa.kind) {
      case pascal::VariableExpr::Selector::Kind::Field:
        if (sa.field != sb.field)
          return false;
        break;
      case pascal::VariableExpr::Selector::Kind::Index:
        if (!ast_equal_node(sa.index.get(), sb.index.get()))
          return false;
        break;
      case pascal::VariableExpr::Selector::Kind::Pointer:
        break;
      }
    }
    return true;
  }
  case NodeKind::IdentifierList: {
    auto ia = static_cast<const pascal::IdentifierList *>(a);
    auto ib = static_cast<const pascal::IdentifierList *>(b);
    return ia->identifiers == ib->identifiers;
  }
  case NodeKind::VarDecl: {
    auto va = static_cast<const pascal::VarDecl *>(a);
    auto vb = static_cast<const pascal::VarDecl *>(b);
    return ast_equal_node(&va->names, &vb->names) &&
           ast_equal_node(va->type.get(), vb->type.get());
  }
  case NodeKind::TypeDecl: {
    auto ta = static_cast<const pascal::TypeDecl *>(a);
    auto tb = static_cast<const pascal::TypeDecl *>(b);
    return ta->name == tb->name &&
           ast_equal_node(ta->type.get(), tb->type.get());
  }
  case NodeKind::Range: {
    auto ra = static_cast<const pascal::Range *>(a);
    auto rb = static_cast<const pascal::Range *>(b);
    return ra->start == rb->start && ra->end == rb->end;
  }
  case NodeKind::SimpleTypeSpec: {
    auto sa = static_cast<const pascal::SimpleTypeSpec *>(a);
    auto sb = static_cast<const pascal::SimpleTypeSpec *>(b);
    return sa->basic == sb->basic && sa->name == sb->name;
  }
  case NodeKind::ArrayTypeSpec: {
    auto aa = static_cast<const pascal::ArrayTypeSpec *>(a);
    auto ab = static_cast<const pascal::ArrayTypeSpec *>(b);
    if (aa->ranges.size() != ab->ranges.size())
      return false;
    for (size_t i = 0; i < aa->ranges.size(); ++i)
      if (!ast_equal_node(&aa->ranges[i], &ab->ranges[i]))
        return false;
    return ast_equal_node(aa->elementType.get(), ab->elementType.get());
  }
  case NodeKind::CompoundStmt: {
    auto ca = static_cast<const pascal::CompoundStmt *>(a);
    auto cb = static_cast<const pascal::CompoundStmt *>(b);
    if (ca->statements.size() != cb->statements.size())
      return false;
    for (size_t i = 0; i < ca->statements.size(); ++i)
      if (!ast_equal_node(ca->statements[i].get(), cb->statements[i].get()))
        return false;
    return true;
  }
  case NodeKind::ProcCall: {
    auto pa = static_cast<const pascal::ProcCall *>(a);
    auto pb = static_cast<const pascal::ProcCall *>(b);
    if (pa->name != pb->name || pa->args.size() != pb->args.size())
      return false;
    for (size_t i = 0; i < pa->args.size(); ++i)
      if (!ast_equal_node(pa->args[i].get(), pb->args[i].get()))
        return false;
    return true;
  }
  case NodeKind::IfStmt: {
    auto ia = static_cast<const pascal::IfStmt *>(a);
    auto ib = static_cast<const pascal::IfStmt *>(b);
    return ast_equal_node(ia->condition.get(), ib->condition.get()) &&
           ast_equal_node(ia->thenBranch.get(), ib->thenBranch.get()) &&
           ast_equal_node(ia->elseBranch.get(), ib->elseBranch.get());
  }
  case NodeKind::WhileStmt: {
    auto wa = static_cast<const pascal::WhileStmt *>(a);
    auto wb = static_cast<const pascal::WhileStmt *>(b);
    return ast_equal_node(wa->condition.get(), wb->condition.get()) &&
           ast_equal_node(wa->body.get(), wb->body.get());
  }
  case NodeKind::ForStmt: {
    auto fa = static_cast<const pascal::ForStmt *>(a);
    auto fb = static_cast<const pascal::ForStmt *>(b);
    return ast_equal_node(fa->init.get(), fb->init.get()) &&
           fa->downto == fb->downto &&
           ast_equal_node(fa->limit.get(), fb->limit.get()) &&
           ast_equal_node(fa->body.get(), fb->body.get());
  }
  case NodeKind::RepeatStmt: {
    auto ra = static_cast<const pascal::RepeatStmt *>(a);
    auto rb = static_cast<const pascal::RepeatStmt *>(b);
    if (ra->body.size() != rb->body.size())
      return false;
    for (size_t i = 0; i < ra->body.size(); ++i)
      if (!ast_equal_node(ra->body[i].get(), rb->body[i].get()))
        return false;
    return ast_equal_node(ra->condition.get(), rb->condition.get());
  }
  case NodeKind::CaseLabel: {
    auto la = static_cast<const pascal::CaseLabel *>(a);
    auto lb = static_cast<const pascal::CaseLabel *>(b);
    if (la->constants.size() != lb->constants.size())
      return false;
    for (size_t i = 0; i < la->constants.size(); ++i)
      if (!ast_equal_node(la->constants[i].get(), lb->constants[i].get()))
        return false;
    return ast_equal_node(la->stmt.get(), lb->stmt.get());
  }
  case NodeKind::CaseStmt: {
    auto ca = static_cast<const pascal::CaseStmt *>(a);
    auto cb = static_cast<const pascal::CaseStmt *>(b);
    if (ca->cases.size() != cb->cases.size())
      return false;
    if (!ast_equal_node(ca->expr.get(), cb->expr.get()))
      return false;
    for (size_t i = 0; i < ca->cases.size(); ++i)
      if (!ast_equal_node(ca->cases[i].get(), cb->cases[i].get()))
        return false;
    return true;
  }
  case NodeKind::WithStmt: {
    auto wa = static_cast<const pascal::WithStmt *>(a);
    auto wb = static_cast<const pascal::WithStmt *>(b);
    return ast_equal_node(wa->recordExpr.get(), wb->recordExpr.get()) &&
           ast_equal_node(wa->body.get(), wb->body.get());
  }
  case NodeKind::UnaryExpr: {
    auto ua = static_cast<const pascal::UnaryExpr *>(a);
    auto ub = static_cast<const pascal::UnaryExpr *>(b);
    return ua->op == ub->op &&
           ast_equal_node(ua->operand.get(), ub->operand.get());
  }
  case NodeKind::ConstDecl: {
    auto ca = static_cast<const pascal::ConstDecl *>(a);
    auto cb = static_cast<const pascal::ConstDecl *>(b);
    return ca->name == cb->name &&
           ast_equal_node(ca->value.get(), cb->value.get());
  }
  case NodeKind::ProcedureDecl: {
    auto pa = static_cast<const pascal::ProcedureDecl *>(a);
    auto pb = static_cast<const pascal::ProcedureDecl *>(b);
    if (pa->name != pb->name || pa->params.size() != pb->params.size())
      return false;
    for (size_t i = 0; i < pa->params.size(); ++i)
      if (!ast_equal_node(pa->params[i].get(), pb->params[i].get()))
        return false;
    return ast_equal_node(pa->body.get(), pb->body.get());
  }
  case NodeKind::FunctionDecl: {
    auto fa = static_cast<const pascal::FunctionDecl *>(a);
    auto fb = static_cast<const pascal::FunctionDecl *>(b);
    if (fa->name != fb->name || fa->params.size() != fb->params.size())
      return false;
    for (size_t i = 0; i < fa->params.size(); ++i)
      if (!ast_equal_node(fa->params[i].get(), fb->params[i].get()))
        return false;
    return ast_equal_node(fa->returnType.get(), fb->returnType.get()) &&
           ast_equal_node(fa->body.get(), fb->body.get());
  }
  case NodeKind::ParamDecl: {
    auto pa = static_cast<const pascal::ParamDecl *>(a);
    auto pb = static_cast<const pascal::ParamDecl *>(b);
    return pa->names == pb->names &&
           ast_equal_node(pa->type.get(), pb->type.get());
  }
  case NodeKind::RecordTypeSpec: {
    auto ra = static_cast<const pascal::RecordTypeSpec *>(a);
    auto rb = static_cast<const pascal::RecordTypeSpec *>(b);
    if (ra->fields.size() != rb->fields.size())
      return false;
    for (size_t i = 0; i < ra->fields.size(); ++i)
      if (!ast_equal_node(ra->fields[i].get(), rb->fields[i].get()))
        return false;
    return true;
  }
  case NodeKind::PointerTypeSpec: {
    auto pa = static_cast<const pascal::PointerTypeSpec *>(a);
    auto pb = static_cast<const pascal::PointerTypeSpec *>(b);
    return ast_equal_node(pa->refType.get(), pb->refType.get());
  }
  case NodeKind::NewExpr: {
    auto na = static_cast<const pascal::NewExpr *>(a);
    auto nb = static_cast<const pascal::NewExpr *>(b);
    return ast_equal_node(na->variable.get(), nb->variable.get());
  }
  case NodeKind::DisposeExpr: {
    auto da = static_cast<const pascal::DisposeExpr *>(a);
    auto db = static_cast<const pascal::DisposeExpr *>(b);
    return ast_equal_node(da->variable.get(), db->variable.get());
  }
  case NodeKind::TypeSpec:
    return true;
  default:
    return true;
  }
}

inline bool ast_equal(const pascal::AST &a, const pascal::AST &b) {
  if (a.valid != b.valid)
    return false;
  return ast_equal_node(a.root.get(), b.root.get());
}

inline pascal::AST make_empty_ast(bool valid = true) {
  pascal::AST ast{};
  ast.valid = valid;
  return ast;
}

inline void run_full(std::string_view src,
                     const std::vector<Token> &expected_tokens,
                     const pascal::AST &expected_ast,
                     std::string_view expected_asm,
                     std::string_view expected_output) {
  Lexer lex(src);
  auto tokens = lex.scanTokens();
  ASSERT_EQ(tokens.size(), expected_tokens.size());

  for (size_t i = 0; i < expected_tokens.size(); ++i) {
    EXPECT_EQ(tokens[i].type, expected_tokens[i].type);
    EXPECT_EQ(tokens[i].lexeme, expected_tokens[i].lexeme);
  }

  if (TEST_MODE == TestMode::Tokens)
    return;

  Parser parser(tokens);
  AST ast{};
  EXPECT_NO_THROW({ ast = parser.parse(); });
  EXPECT_TRUE(ast_equal(ast, expected_ast));

  if (TEST_MODE == TestMode::TokensAst)
    return;

  ASTValidator validator;
  auto res = validator.validate(ast);
  EXPECT_TRUE(res.success) << res.message;

  CodeGenerator codegen;
  auto asm_code = codegen.generate(ast);
  EXPECT_EQ(asm_code, expected_asm);

  if (TEST_MODE == TestMode::TokensAstAsm)
    return;

  auto output = execute_stub(asm_code);
  EXPECT_EQ(output, expected_output);
}

inline void
run_validation_fail(std::string_view src,
                    const std::vector<Token> &expected_tokens,
                    const pascal::AST &expected_ast,
                    [[maybe_unused]] std::string_view expected_asm,
                    [[maybe_unused]] std::string_view expected_output,
                    std::string_view expected_error = "") {
  Lexer lex(src);
  auto tokens = lex.scanTokens();
  ASSERT_EQ(tokens.size(), expected_tokens.size());

  for (size_t i = 0; i < expected_tokens.size(); ++i) {
    EXPECT_EQ(tokens[i].type, expected_tokens[i].type);
    EXPECT_EQ(tokens[i].lexeme, expected_tokens[i].lexeme);
  }

  Parser parser(tokens);
  AST ast{};
  EXPECT_NO_THROW({ ast = parser.parse(); });
  EXPECT_TRUE(ast_equal(ast, expected_ast));

  ASTValidator validator;
  auto res = validator.validate(ast);
  EXPECT_FALSE(res.success);
  if (!expected_error.empty()) {
    EXPECT_EQ(res.message, expected_error);
  }
}

} // namespace test_utils
