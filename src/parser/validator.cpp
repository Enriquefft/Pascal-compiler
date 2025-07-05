#include "parser/validator.hpp"

namespace pascal {
ASTValidator::Result ASTValidator::validate(const AST &ast) {
  m_valid = ast.valid && ast.root != nullptr;
  m_errorMsg.clear();
  m_errorLine = 0;
  m_errorColumn = 0;
  if (!m_valid) {
    m_errorMsg = "Invalid AST";
    return {false, m_errorMsg, m_errorLine, m_errorColumn};
  }
  pushScope();
  ast.root->accept(*this);
  popScope();
  return {m_valid, m_errorMsg, m_errorLine, m_errorColumn};
}

void ASTValidator::setError(const std::string &msg, const ASTNode &node) {
  if (m_errorMsg.empty()) {
    m_errorMsg = msg;
    m_errorLine = node.line;
    m_errorColumn = node.column;
  }
  m_valid = false;
}

void ASTValidator::visitProgram(const Program &node) {
  pushScope();

  if (node.name.empty())
    setError("Program missing name", node);
  else {
    declare(node.name);
    if (!node.block)
      setError("Program missing block", node);
  }
  if (node.block)
    node.block->accept(*this);
  popScope();
}

void ASTValidator::visitBlock(const Block &node) {
  pushScope();
  for (const auto &decl : node.declarations) {
    if (!decl)
      setError("Null declaration", node);
    else
      decl->accept(*this);
  }
  for (const auto &stmt : node.statements) {
    if (!stmt)
      setError("Null statement", node);
    else
      stmt->accept(*this);
  }
  popScope();
}

void ASTValidator::visitVarDecl(const VarDecl &node) {
  if (node.names.empty())
    setError("VarDecl missing names", node);

  else {
    for (auto &n : node.names)
      declare(n);
    if (!node.type)
      setError("VarDecl missing type", node);
  }

  if (node.type)
    node.type->accept(*this);
}

void ASTValidator::visitTypeDefinition(const TypeDefinition &node) {
  if (node.name.empty())
    setError("TypeDefinition missing name", node);
  else if (!node.type)
    setError("TypeDefinition missing type", node);
  if (node.type)
    node.type->accept(*this);
}

void ASTValidator::visitVarSection(const VarSection &node) {

  for (const auto &decl : node.declarations) {
    decl.accept(*this);
  }
}

void ASTValidator::visitParamDecl(const ParamDecl &node) {
  if (node.names.empty())
    setError("ParamDecl missing names", node);

  else {
    for (auto &n : node.names)
      declare(n);
    if (!node.type)
      setError("ParamDecl missing type", node);
  }

  if (node.type)
    node.type->accept(*this);
}

void ASTValidator::visitConstDecl(const ConstDecl &node) {
  if (node.name.empty())
    setError("ConstDecl missing name", node);
  else if (!node.value)
    setError("ConstDecl missing value", node);
  if (node.value)
    node.value->accept(*this);
}

void ASTValidator::visitTypeDecl(const TypeDecl &node) {

  for (const auto &n : node.definitions) {
    if (n.name.empty())
      setError("TypeDecl missing name", node);
    else if (!n.type)
      setError("TypeDecl missing type", node);
    if (n.type)
      n.type->accept(*this);
  }
}

void ASTValidator::visitProcedureDecl(const ProcedureDecl &node) {
  pushScope();
  if (node.name.empty())
    setError("ProcedureDecl missing name", node);
  else {
    declare(node.name);
    if (!node.body)
      setError("ProcedureDecl missing body", node);
  }
  for (const auto &p : node.params) {
    if (!p)
      setError("Null parameter", node);
    else
      p->accept(*this);
  }
  if (node.body) {

    node.body->accept(*this);
  }
  popScope();
}

void ASTValidator::visitFunctionDecl(const FunctionDecl &node) {

  pushScope();

  if (node.name.empty())
    setError("FunctionDecl missing name", node);

  else {
    declare(node.name);
    if (!node.returnType)

      setError("FunctionDecl missing return type", node);
    else if (!node.body)
      setError("FunctionDecl missing body", node);
  }
  for (const auto &p : node.params) {
    if (!p)
      setError("Null parameter", node);
    else
      p->accept(*this);
  }
  if (node.returnType)
    node.returnType->accept(*this);
  if (node.body) {

    node.body->accept(*this);
  }
  popScope();
}

void ASTValidator::visitCompoundStmt(const CompoundStmt &node) {
  for (const auto &s : node.statements) {
    if (!s)
      setError("Null statement", node);
    else
      s->accept(*this);
  }
}

void ASTValidator::visitAssignStmt(const AssignStmt &node) {
  if (!node.target)
    setError("AssignStmt missing target", node);
  else if (!node.value)
    setError("AssignStmt missing value", node);
  if (node.target)
    node.target->accept(*this);
  if (node.value)
    node.value->accept(*this);
}

void ASTValidator::visitProcCall(const ProcCall &node) {
  if (node.name.empty())
    setError("ProcCall missing name", node);
  for (const auto &a : node.args) {
    if (!a)
      setError("Null argument", node);
    else
      a->accept(*this);
  }
}

void ASTValidator::visitIfStmt(const IfStmt &node) {
  if (!node.condition)
    setError("IfStmt missing condition", node);
  else if (!node.thenBranch)
    setError("IfStmt missing then branch", node);
  if (node.condition)
    node.condition->accept(*this);
  if (node.thenBranch)
    node.thenBranch->accept(*this);
  if (node.elseBranch)
    node.elseBranch->accept(*this);
}

void ASTValidator::visitWhileStmt(const WhileStmt &node) {
  if (!node.condition)
    setError("WhileStmt missing condition", node);
  else if (!node.body)
    setError("WhileStmt missing body", node);
  if (node.condition)
    node.condition->accept(*this);
  if (node.body)
    node.body->accept(*this);
}

void ASTValidator::visitForStmt(const ForStmt &node) {
  if (!node.init)
    setError("ForStmt missing init", node);
  else if (!node.limit)
    setError("ForStmt missing limit", node);
  else if (!node.body)
    setError("ForStmt missing body", node);
  if (node.init)
    node.init->accept(*this);
  if (node.limit)
    node.limit->accept(*this);
  if (node.body)
    node.body->accept(*this);
}

void ASTValidator::visitRepeatStmt(const RepeatStmt &node) {
  if (!node.condition)
    setError("RepeatStmt missing condition", node);
  for (const auto &s : node.body) {
    if (!s)
      setError("Null statement", node);
    else
      s->accept(*this);
  }
  if (node.condition)
    node.condition->accept(*this);
}

void ASTValidator::visitIdentifierList(const IdentifierList &node) {
  if (node.identifiers.empty())
    setError("IdentifierList empty", node);
  for (const auto &id : node.identifiers) {
    if (id.empty())
      setError("IdentifierList contains empty identifier", node);
  }
}

void ASTValidator::visitCaseStmt(const CaseStmt &node) {
  if (!node.expr)
    setError("CaseStmt missing expression", node);
  if (node.expr)
    node.expr->accept(*this);
  for (const auto &c : node.cases) {
    if (!c)
      setError("Null case label", node);
    else
      c->accept(*this);
  }
}

void ASTValidator::visitWithStmt(const WithStmt &node) {
  if (!node.recordExpr)
    setError("WithStmt missing record expression", node);
  else if (!node.body)
    setError("WithStmt missing body", node);
  if (node.recordExpr)
    node.recordExpr->accept(*this);
  if (node.body)
    node.body->accept(*this);
}

void ASTValidator::visitBinaryExpr(const BinaryExpr &node) {
  if (!node.left)
    setError("BinaryExpr missing left operand", node);
  else if (!node.right)
    setError("BinaryExpr missing right operand", node);
  else if (node.op.empty())
    setError("BinaryExpr missing operator", node);
  if (node.left)
    node.left->accept(*this);
  if (node.right)
    node.right->accept(*this);
}

void ASTValidator::visitUnaryExpr(const UnaryExpr &node) {
  if (!node.operand)
    setError("UnaryExpr missing operand", node);
  else if (node.op.empty())
    setError("UnaryExpr missing operator", node);
  if (node.operand)
    node.operand->accept(*this);
}

void ASTValidator::visitLiteralExpr(const LiteralExpr &node) {
  if (node.value.empty())
    setError("LiteralExpr empty value", node);
}

void ASTValidator::visitVariableExpr(const VariableExpr &node) {

  if (node.name.empty())
    setError("VariableExpr missing name", node);
  else if (!isDeclared(node.name))
    declare(node.name);

  for (const auto &sel : node.selectors) {
    if (sel.kind == VariableExpr::Selector::Kind::Index && sel.index)
      sel.index->accept(*this);
  }
}

void ASTValidator::visitRange(const Range &node) {
  if (node.start > node.end)
    setError("Invalid range", node);
}

void ASTValidator::visitTypeSpec(const TypeSpec & /*node*/) {}

void ASTValidator::visitSimpleTypeSpec(const SimpleTypeSpec & /*node*/) {}

void ASTValidator::visitArrayTypeSpec(const ArrayTypeSpec &node) {
  for (const auto &r : node.ranges)
    visitRange(r);
  if (!node.elementType)
    setError("ArrayTypeSpec missing element type", node);
  if (node.elementType)
    node.elementType->accept(*this);
}

void ASTValidator::visitRecordTypeSpec(const RecordTypeSpec &node) {
  if (node.fields.empty())
    setError("RecordTypeSpec with no fields", node);
  for (const auto &f : node.fields) {
    if (!f)
      setError("Null field", node);
    else
      f->accept(*this);
  }
}

void ASTValidator::visitPointerTypeSpec(const PointerTypeSpec &node) {
  if (!node.refType)
    setError("PointerTypeSpec missing referenced type", node);
  if (node.refType)
    node.refType->accept(*this);
}

void ASTValidator::visitCaseLabel(const CaseLabel &node) {
  if (node.constants.empty())
    setError("CaseLabel missing constants", node);
  else if (!node.stmt)
    setError("CaseLabel missing statement", node);
  for (const auto &c : node.constants) {
    if (!c)
      setError("Null constant", node);
    else
      c->accept(*this);
  }
  if (node.stmt)
    node.stmt->accept(*this);
}

void ASTValidator::visitNewExpr(const NewExpr &node) {
  if (!node.variable)
    setError("NewExpr missing variable", node);
  if (node.variable)
    node.variable->accept(*this);
}

void ASTValidator::visitDisposeExpr(const DisposeExpr &node) {
  if (!node.variable)
    setError("DisposeExpr missing variable", node);
  if (node.variable)
    node.variable->accept(*this);
}

// Helper functions for scope management

void ASTValidator::pushScope() { m_scopes.emplace_back(); }

void ASTValidator::popScope() {
  if (!m_scopes.empty())
    m_scopes.pop_back();
}

void ASTValidator::declare(const std::string &name) {
  if (m_scopes.empty())
    pushScope();
  m_scopes.back().insert(name);
}

bool ASTValidator::isDeclared(const std::string &name) const {
  for (auto it = m_scopes.rbegin(); it != m_scopes.rend(); ++it)
    if (it->count(name))
      return true;
  return false;
}

} // namespace pascal
