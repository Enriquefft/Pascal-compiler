#ifndef PASCAL_COMPILER_VALIDATOR_HPP
#define PASCAL_COMPILER_VALIDATOR_HPP

#include "parser/ast.hpp"

namespace pascal {

class ASTValidator : public NodeVisitor {
public:
  bool validate(const AST &ast);

  void visitProgram(const Program & /*node*/) override {}
  void visitBlock(const Block & /*node*/) override {}
  void visitVarDecl(const VarDecl & /*node*/) override {}
  void visitParamDecl(const ParamDecl & /*node*/) override {}
  void visitConstDecl(const ConstDecl & /*node*/) override {}
  void visitTypeDecl(const TypeDecl & /*node*/) override {}
  void visitProcedureDecl(const ProcedureDecl & /*node*/) override {}
  void visitFunctionDecl(const FunctionDecl & /*node*/) override {}
  void visitCompoundStmt(const CompoundStmt & /*node*/) override {}
  void visitAssignStmt(const AssignStmt & /*node*/) override {}
  void visitProcCall(const ProcCall & /*node*/) override {}
  void visitIfStmt(const IfStmt & /*node*/) override {}
  void visitWhileStmt(const WhileStmt & /*node*/) override {}
  void visitForStmt(const ForStmt & /*node*/) override {}
  void visitRepeatStmt(const RepeatStmt & /*node*/) override {}
  void visitCaseStmt(const CaseStmt & /*node*/) override {}
  void visitWithStmt(const WithStmt & /*node*/) override {}
  void visitBinaryExpr(const BinaryExpr & /*node*/) override {}
  void visitUnaryExpr(const UnaryExpr & /*node*/) override {}
  void visitLiteralExpr(const LiteralExpr & /*node*/) override {}
  void visitVariableExpr(const VariableExpr & /*node*/) override {}

private:
  bool m_valid{true};
};

} // namespace pascal

#endif // PASCAL_COMPILER_VALIDATOR_HPP
