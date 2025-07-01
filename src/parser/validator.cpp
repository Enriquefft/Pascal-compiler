#include "parser/validator.hpp"

namespace pascal {

bool ASTValidator::validate(const AST &ast) {
  m_valid = ast.valid && ast.root != nullptr;
  if (!m_valid)
    return false;
  ast.root->accept(*this);
  return m_valid;
}

void ASTValidator::visitProgram(const Program &node) {
  if (node.name.empty() || !node.block)
    m_valid = false;
  if (node.block)
    node.block->accept(*this);
}

void ASTValidator::visitBlock(const Block &node) {
  for (const auto &decl : node.declarations) {
    if (!decl)
      m_valid = false;
    else
      decl->accept(*this);
  }
  for (const auto &stmt : node.statements) {
    if (!stmt)
      m_valid = false;
    else
      stmt->accept(*this);
  }
}

void ASTValidator::visitVarDecl(const VarDecl &node) {
  if (node.names.empty() || !node.type)
    m_valid = false;
  if (node.type)
    node.type->accept(*this);
}

void ASTValidator::visitParamDecl(const ParamDecl &node) {
  if (node.names.empty() || !node.type)
    m_valid = false;
  if (node.type)
    node.type->accept(*this);
}

void ASTValidator::visitConstDecl(const ConstDecl &node) {
  if (node.name.empty() || !node.value)
    m_valid = false;
  if (node.value)
    node.value->accept(*this);
}

void ASTValidator::visitTypeDecl(const TypeDecl &node) {
  if (node.name.empty() || !node.type)
    m_valid = false;
  if (node.type)
    node.type->accept(*this);
}

void ASTValidator::visitProcedureDecl(const ProcedureDecl &node) {
  if (node.name.empty() || !node.body)
    m_valid = false;
  for (const auto &p : node.params) {
    if (!p)
      m_valid = false;
    else
      p->accept(*this);
  }
  if (node.body)
    node.body->accept(*this);
}

void ASTValidator::visitFunctionDecl(const FunctionDecl &node) {
  if (node.name.empty() || !node.returnType || !node.body)
    m_valid = false;
  for (const auto &p : node.params) {
    if (!p)
      m_valid = false;
    else
      p->accept(*this);
  }
  if (node.returnType)
    node.returnType->accept(*this);
  if (node.body)
    node.body->accept(*this);
}

void ASTValidator::visitCompoundStmt(const CompoundStmt &node) {
  for (const auto &s : node.statements) {
    if (!s)
      m_valid = false;
    else
      s->accept(*this);
  }
}

void ASTValidator::visitAssignStmt(const AssignStmt &node) {
  if (!node.target || !node.value)
    m_valid = false;
  if (node.target)
    node.target->accept(*this);
  if (node.value)
    node.value->accept(*this);
}

void ASTValidator::visitProcCall(const ProcCall &node) {
  if (node.name.empty())
    m_valid = false;
  for (const auto &a : node.args) {
    if (!a)
      m_valid = false;
    else
      a->accept(*this);
  }
}

void ASTValidator::visitIfStmt(const IfStmt &node) {
  if (!node.condition || !node.thenBranch)
    m_valid = false;
  if (node.condition)
    node.condition->accept(*this);
  if (node.thenBranch)
    node.thenBranch->accept(*this);
  if (node.elseBranch)
    node.elseBranch->accept(*this);
}

void ASTValidator::visitWhileStmt(const WhileStmt &node) {
  if (!node.condition || !node.body)
    m_valid = false;
  if (node.condition)
    node.condition->accept(*this);
  if (node.body)
    node.body->accept(*this);
}

void ASTValidator::visitForStmt(const ForStmt &node) {
  if (!node.init || !node.limit || !node.body)
    m_valid = false;
  if (node.init)
    node.init->accept(*this);
  if (node.limit)
    node.limit->accept(*this);
  if (node.body)
    node.body->accept(*this);
}

void ASTValidator::visitRepeatStmt(const RepeatStmt &node) {
  if (!node.condition)
    m_valid = false;
  for (const auto &s : node.body) {
    if (!s)
      m_valid = false;
    else
      s->accept(*this);
  }
  if (node.condition)
    node.condition->accept(*this);
}

void ASTValidator::visitCaseStmt(const CaseStmt &node) {
  if (!node.expr)
    m_valid = false;
  if (node.expr)
    node.expr->accept(*this);
  for (const auto &c : node.cases) {
    if (!c)
      m_valid = false;
    else
      c->accept(*this);
  }
}

void ASTValidator::visitWithStmt(const WithStmt &node) {
  if (!node.recordExpr || !node.body)
    m_valid = false;
  if (node.recordExpr)
    node.recordExpr->accept(*this);
  if (node.body)
    node.body->accept(*this);
}

void ASTValidator::visitBinaryExpr(const BinaryExpr &node) {
  if (!node.left || !node.right || node.op.empty())
    m_valid = false;
  if (node.left)
    node.left->accept(*this);
  if (node.right)
    node.right->accept(*this);
}

void ASTValidator::visitUnaryExpr(const UnaryExpr &node) {
  if (!node.operand || node.op.empty())
    m_valid = false;
  if (node.operand)
    node.operand->accept(*this);
}

void ASTValidator::visitLiteralExpr(const LiteralExpr &node) {
  if (node.value.empty())
    m_valid = false;
}

void ASTValidator::visitVariableExpr(const VariableExpr &node) {
  if (node.name.empty())
    m_valid = false;
  for (const auto &sel : node.selectors) {
    if (sel.kind == VariableExpr::Selector::Kind::Index && sel.index)
      sel.index->accept(*this);
  }
}

void ASTValidator::visitRange(const Range &node) {
  if (node.start > node.end)
    m_valid = false;
}

void ASTValidator::visitTypeSpec(const TypeSpec & /*node*/) {}

void ASTValidator::visitSimpleTypeSpec(const SimpleTypeSpec & /*node*/) {}

void ASTValidator::visitArrayTypeSpec(const ArrayTypeSpec &node) {
  for (const auto &r : node.ranges)
    visitRange(r);
  if (!node.elementType)
    m_valid = false;
  if (node.elementType)
    node.elementType->accept(*this);
}

void ASTValidator::visitRecordTypeSpec(const RecordTypeSpec &node) {
  if (node.fields.empty())
    m_valid = false;
  for (const auto &f : node.fields) {
    if (!f)
      m_valid = false;
    else
      f->accept(*this);
  }
}

void ASTValidator::visitPointerTypeSpec(const PointerTypeSpec &node) {
  if (!node.refType)
    m_valid = false;
  if (node.refType)
    node.refType->accept(*this);
}

void ASTValidator::visitCaseLabel(const CaseLabel &node) {
  if (node.constants.empty() || !node.stmt)
    m_valid = false;
  for (const auto &c : node.constants) {
    if (!c)
      m_valid = false;
    else
      c->accept(*this);
  }
  if (node.stmt)
    node.stmt->accept(*this);
}

void ASTValidator::visitNewExpr(const NewExpr &node) {
  if (!node.variable)
    m_valid = false;
  if (node.variable)
    node.variable->accept(*this);
}

void ASTValidator::visitDisposeExpr(const DisposeExpr &node) {
  if (!node.variable)
    m_valid = false;
  if (node.variable)
    node.variable->accept(*this);
}

} // namespace pascal
