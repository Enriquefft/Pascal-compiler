#ifndef PASCAL_COMPILER_CODEGEN_HPP
#define PASCAL_COMPILER_CODEGEN_HPP

#include <string>

#include "parser/ast.hpp"

namespace pascal {

class CodeGenerator : public NodeVisitor {
public:
  [[nodiscard]] std::string generate(const AST &ast);

  void visitProgram(const Program &node) override;
  void visitBlock(const Block &node) override;
  void visitVarDecl(const VarDecl &node) override;
  void visitParamDecl(const ParamDecl & /*node*/) override {}
  void visitConstDecl(const ConstDecl & /*node*/) override {}
  void visitTypeDecl(const TypeDecl & /*node*/) override {}
  void visitProcedureDecl(const ProcedureDecl & /*node*/) override {}
  void visitFunctionDecl(const FunctionDecl & /*node*/) override {}
  void visitCompoundStmt(const CompoundStmt &node) override;
  void visitAssignStmt(const AssignStmt &node) override;
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
  void visitRange(const Range & /*node*/) override {}
  void visitTypeSpec(const TypeSpec & /*node*/) override {}
  void visitSimpleTypeSpec(const SimpleTypeSpec & /*node*/) override {}
  void visitArrayTypeSpec(const ArrayTypeSpec & /*node*/) override {}
  void visitRecordTypeSpec(const RecordTypeSpec & /*node*/) override {}
  void visitPointerTypeSpec(const PointerTypeSpec & /*node*/) override {}
  void visitCaseLabel(const CaseLabel & /*node*/) override {}
  void visitNewExpr(const NewExpr & /*node*/) override {}
  void visitDisposeExpr(const DisposeExpr & /*node*/) override {}

private:
  void emit(const std::string &text);
  void genExpr(const Expression *expr);

  std::string m_output;
};

} // namespace pascal

#endif // PASCAL_COMPILER_CODEGEN_HPP
