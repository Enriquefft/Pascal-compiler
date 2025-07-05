#include "visitors/codegen.hpp"
#include "parser/ast.hpp"
#include "utils.hpp"
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace pascal {

using std::cout;
using std::endl;

namespace {
const char *ARG_REGS[] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};

bool isFloatLiteral(const std::string &v) {
  return v.find('.') != std::string::npos;
}

std::string floatToHex(double d) {
  uint64_t bits;
  std::memcpy(&bits, &d, sizeof(double));
  std::ostringstream oss;
  oss << "0x" << std::hex << std::uppercase << std::setw(16)
      << std::setfill('0') << bits;
  return oss.str();
}

std::string floatToHex(const std::string &v) {
  return floatToHex(std::stod(v));
}

} // namespace

std::string CodeGenerator::generate(const AST &ast) {
  m_output.clear();
  m_vars.clear();
  m_varSet.clear();
  m_params.clear();
  m_paramMap.clear();
  m_strings.clear();
  m_stringMap.clear();
  m_typeDefs.clear();
  m_varTypes.clear();
  m_ptrSizes.clear();
  m_needMalloc = m_needFree = m_needPuts = m_needPrintf = false;
  if (ast.root) {
    collectVars(ast.root.get());
    ast.root->accept(*this);
  }
  return m_output;
}

void CodeGenerator::visitProgram(const Program &node) {
  bool hasData = m_needPrintf || !m_strings.empty();
  if (hasData) {
    emit("section .data\n");
    if (m_needPrintf)
      emit("fmt_int: db \"%d\", 10, 0\n");
    for (const auto &p : m_strings) {
      if (p.second.empty())
        emit(p.first + ": db 0\n");
      else
        emit(p.first + ": db \"" + p.second + "\", 0\n");
    }
    emit("\n");
  }
  if (!m_vars.empty()) {
    emit("section .bss\n");
    for (const auto &v : m_vars)
      emit(v.first + ":    resq    " + std::to_string(v.second) + "\n");
    emit("\n");
  }
  emit("section .text\n");
  if (m_needMalloc)
    emit("extern malloc\n");
  if (m_needFree)
    emit("extern free\n");
  if (m_needPuts)
    emit("extern puts\n");
  if (m_needPrintf)
    emit("extern printf\n");
  if (node.block) {
    for (const auto &decl : node.block->declarations) {
      if (decl)
        decl->accept(*this);
    }
  }
  emit("global main\n");
  emit("main:\n");
  if (node.block) {
    for (const auto &stmt : node.block->statements) {
      if (stmt)
        stmt->accept(*this);
    }
  }
  emit("    ret\n");
}

void CodeGenerator::visitBlock(const Block &node) {
  for (const auto &stmt : node.statements) {
    if (stmt)
      stmt->accept(*this);
  }
}

void CodeGenerator::visitVarDecl(const VarDecl &node) {
  for (const auto &n : node.names)
    addVar(n, node.type->size());
}

void CodeGenerator::visitTypeDefinition(const TypeDefinition &_) {
  // Type definitions are not directly translated to code, so we skip them.
  // They are used for type checking and validation.
}

void CodeGenerator::visitVarSection(const VarSection &node) {
  for (const auto &decl : node.declarations) {
    decl.accept(*this);
  }
}

void CodeGenerator::visitProcedureDecl(const ProcedureDecl &node) {
  emit("global " + node.name + "\n");
  emit(node.name + ":\n");
  auto savedFunc = m_currentFunction;
  auto savedMap = m_paramMap;
  m_currentFunction.clear();
  m_paramMap.clear();
  for (size_t i = 0; i < node.params.size() && i < 6; ++i) {
    if (!node.params[i]->names.empty())
      m_paramMap[node.params[i]->names[0]] = ARG_REGS[i];
  }
  if (node.body)
    node.body->accept(*this);
  emit("    ret\n");
  m_paramMap = savedMap;
  m_currentFunction = savedFunc;
}

void CodeGenerator::visitFunctionDecl(const FunctionDecl &node) {
  emit("global " + node.name + "\n");
  emit(node.name + ":\n");
  auto savedFunc = m_currentFunction;
  auto savedMap = m_paramMap;
  m_currentFunction = node.name;
  m_paramMap.clear();
  for (size_t i = 0; i < node.params.size() && i < 6; ++i) {
    if (!node.params[i]->names.empty())
      m_paramMap[node.params[i]->names[0]] = ARG_REGS[i];
  }
  if (node.body)
    node.body->accept(*this);
  emit("    ret\n");
  m_paramMap = savedMap;
  m_currentFunction = savedFunc;
}

void CodeGenerator::visitCompoundStmt(const CompoundStmt &node) {
  for (const auto &s : node.statements) {
    if (s)
      s->accept(*this);
  }
}

void CodeGenerator::genExpr(const Expression *expr) {
  if (!expr)
    return;
  switch (expr->kind) {
  case NodeKind::LiteralExpr: {
    const auto *lit = static_cast<const LiteralExpr *>(expr);
    std::string val = lit->value;
    if (isFloatLiteral(val))
      val = floatToHex(val);
    emit("    mov    rax, " + val + "\n");
    break;
  }
  case NodeKind::VariableExpr: {
    const auto *var = static_cast<const VariableExpr *>(expr);
    auto it = m_paramMap.find(var->name);
    if (it != m_paramMap.end() && var->selectors.empty()) {
      emit("    mov    rax, " + it->second + "\n");
    } else {
      genVarAddr(var);
      emit("    mov    rax, [rax]\n");
    }
    break;
  }
  case NodeKind::BinaryExpr: {
    const auto *bin = static_cast<const BinaryExpr *>(expr);
    if (bin->left->kind == NodeKind::LiteralExpr &&
        bin->right->kind == NodeKind::LiteralExpr &&
        (isFloatLiteral(
             static_cast<const LiteralExpr *>(bin->left.get())->value) ||
         isFloatLiteral(
             static_cast<const LiteralExpr *>(bin->right.get())->value))) {
      double lhs =
          std::stod(static_cast<const LiteralExpr *>(bin->left.get())->value);
      double rhs =
          std::stod(static_cast<const LiteralExpr *>(bin->right.get())->value);
      double res = 0.0;
      if (bin->op == "+")
        res = lhs + rhs;
      else if (bin->op == "-")
        res = lhs - rhs;
      else if (bin->op == "*")
        res = lhs * rhs;
      emit("    mov    rax, " + floatToHex(res) + "\n");
    } else {
      genExpr(bin->left.get());
      if (bin->right->kind == NodeKind::LiteralExpr) {
        const auto *lit = static_cast<const LiteralExpr *>(bin->right.get());
        std::string val = lit->value;
        if (isFloatLiteral(val))
          val = floatToHex(val);
        if (bin->op == "+")
          emit("    add    rax, " + val + "\n");
        else if (bin->op == "-")
          emit("    sub    rax, " + val + "\n");
        else if (bin->op == "*")
          emit("    imul   rax, " + val + "\n");
      } else if (bin->right->kind == NodeKind::VariableExpr) {
        const auto *rv = static_cast<const VariableExpr *>(bin->right.get());
        auto it = m_paramMap.find(rv->name);
        if (it != m_paramMap.end())
          emit("    mov    rbx, " + it->second + "\n");
        else
          emit("    mov    rbx, [" + rv->name + "]\n");
        if (bin->op == "+")
          emit("    add    rax, rbx\n");
        else if (bin->op == "-")
          emit("    sub    rax, rbx\n");
        else if (bin->op == "*")
          emit("    imul   rax, rbx\n");
      } else {
        emit("    push   rax\n");
        genExpr(bin->right.get());
        emit("    mov    rbx, rax\n");
        emit("    pop    rax\n");
        if (bin->op == "+")
          emit("    add    rax, rbx\n");
        else if (bin->op == "-")
          emit("    sub    rax, rbx\n");
        else if (bin->op == "*")
          emit("    imul   rax, rbx\n");
      }
    }
    break;
  }
  default:
    throw std::runtime_error("Unsupported expression type in code generation");
  }
}

void CodeGenerator::visitAssignStmt(const AssignStmt &node) {
  const auto *var = dynamic_cast<const VariableExpr *>(node.target.get());
  if (!var)
    return;
  bool resultVar = !m_currentFunction.empty() && var->selectors.empty() &&
                   var->name == m_currentFunction;
  auto itParam = m_paramMap.find(var->name);
  bool directReg =
      itParam != m_paramMap.end() && var->selectors.empty() && !resultVar;
  if (!directReg)
    genVarAddr(var);
  emit("    mov    rbx, rax\n");

  if (node.value->kind == NodeKind::LiteralExpr) {
    const auto *lit = static_cast<const LiteralExpr *>(node.value.get());
    std::string val = lit->value;
    if (!val.empty() && val.front() == '\'' && val.back() == '\'')
      val = addString(val.substr(1, val.size() - 2));
    else if (isFloatLiteral(val))
      val = floatToHex(val);
    emit("    mov    rax, " + val + "\n");
  } else {
    genExpr(node.value.get());
  }

  if (resultVar) {
    // value already in rax
    return;
  }

  if (directReg) {
    emit("    mov    " + itParam->second + ", rax\n");
  } else {
    emit("    mov    [rbx], rax\n");
  }
}

void CodeGenerator::visitProcCall(const ProcCall &node) {
  if (node.name == "new" && !node.args.empty()) {
    const auto *var = dynamic_cast<const VariableExpr *>(node.args[0].get());
    if (var) {
      size_t sz = 1;
      auto it = m_ptrSizes.find(var->name);
      if (it != m_ptrSizes.end())
        sz = it->second;
      emit("    mov    rdi, " + std::to_string(sz * 8) + "\n");
      emit("    call   malloc\n");
      emit("    mov    qword [" + var->name + "], rax\n");
    }
  } else if (node.name == "dispose" && !node.args.empty()) {
    const auto *var = dynamic_cast<const VariableExpr *>(node.args[0].get());
    if (var) {
      emit("    mov    rdi, [" + var->name + "]\n");
      emit("    call   free\n");
    }

  } else if (node.name == "writeln" && !node.args.empty()) {
    const Expression *e = node.args[0].get();

    // 1) string‐literal → puts
    if (auto *lit = dynamic_cast<const LiteralExpr *>(e)) {
      if (lit->value.front() == '\'' && lit->value.back() == '\'') {
        std::string lbl =
            addString(lit->value.substr(1, lit->value.size() - 2));
        emit("    mov    rdi, " + lbl + "\n");
        emit("    call   puts\n");
        return;
      }
    }

    // 2) everything else → printf
    // ensure data section has fmt_int declared and extern printf emitted
    emit("    mov    rdi, fmt_int\n");
    genExpr(e); // result → rax
    emit("    mov    rsi, rax\n");
    emit("    xor    rax, rax\n"); // SysV ABI vararg count
    emit("    call   printf\n");
    return;
  }
}

void CodeGenerator::visitIdentifierList(const IdentifierList &node) {
  for (const auto &name : node) {
    if (!name.empty()) {
      addVar(name);
      m_params.insert(name);
    }
  }
}

void CodeGenerator::visitIfStmt(const IfStmt &node) {
  if (node.condition->kind == NodeKind::BinaryExpr) {
    const auto *be = static_cast<const BinaryExpr *>(node.condition.get());
    const auto *lLit = dynamic_cast<const LiteralExpr *>(be->left.get());
    const auto *rLit = dynamic_cast<const LiteralExpr *>(be->right.get());
    if (lLit && rLit) {
      double lhs = std::stod(lLit->value);
      double rhs = std::stod(rLit->value);
      bool cond = false;
      if (be->op == "<")
        cond = lhs < rhs;
      else if (be->op == ">")
        cond = lhs > rhs;
      else if (be->op == "<=")
        cond = lhs <= rhs;
      else if (be->op == ">=")
        cond = lhs >= rhs;
      else if (be->op == "=")
        cond = my_utils::float_equal(lhs, rhs);
      else if (be->op == "<>")
        cond = !my_utils::float_equal(lhs, rhs);
      if (cond) {
        if (node.thenBranch)
          node.thenBranch->accept(*this);
      } else if (node.elseBranch) {
        node.elseBranch->accept(*this);
      }
      return;
    }
    const auto *lVar = dynamic_cast<const VariableExpr *>(be->left.get());
    const auto *rVar = dynamic_cast<const VariableExpr *>(be->right.get());
    if (lVar && rVar && be->op == "<>" && rVar->name == "nil") {
      std::string endLabel = makeLabel();
      emit("    mov    rax, [" + lVar->name + "]\n");
      emit("    cmp    rax, 0\n");
      emit("    je     " + endLabel + "\n");
      const auto *pc = dynamic_cast<const ProcCall *>(node.thenBranch.get());
      if (pc && pc->name == "dispose" && !pc->args.empty()) {
        emit("    mov    rdi, rax\n");
        emit("    call   free\n");
      } else if (node.thenBranch) {
        node.thenBranch->accept(*this);
      }
      emit(endLabel + ":\n");
      return;
    }
    const auto *lVarEq = dynamic_cast<const VariableExpr *>(be->left.get());
    const auto *rLitStr = dynamic_cast<const LiteralExpr *>(be->right.get());
    if (lVarEq && rLitStr && be->op == "=") {
      std::string val = rLitStr->value;
      if (!val.empty() && val.front() == '\'' && val.back() == '\'') {
        std::string lbl = addString(val.substr(1, val.size() - 2));
        std::string endLabel = makeLabel();
        emit("    mov    rax, [" + lVarEq->name + "]\n");
        emit("    cmp    rax, " + lbl + "\n");
        emit("    jne    " + endLabel + "\n");
        if (node.thenBranch)
          node.thenBranch->accept(*this);
        emit(endLabel + ":\n");
        return;
      }
      if (val == "0") {
        std::string endLabel = makeLabel();
        emit("    mov    rax, [" + lVarEq->name + "]\n");
        emit("    cmp    rax, 0\n");
        emit("    jne    " + endLabel + "\n");
        if (node.thenBranch)
          node.thenBranch->accept(*this);
        emit(endLabel + ":\n");
        return;
      }
    }
  }
  std::string elseLabel = makeLabel();
  std::string endLabel = elseLabel;
  if (node.elseBranch)
    endLabel = makeLabel();
  genExpr(node.condition.get());
  emit("    cmp    rax, 0\n");
  if (node.elseBranch) {
    emit("    jle    " + elseLabel + "\n");
    if (node.thenBranch)
      node.thenBranch->accept(*this);
    emit("    jmp    " + endLabel + "\n");
    emit(elseLabel + ":\n");
    node.elseBranch->accept(*this);
    emit(endLabel + ":\n");
  } else {
    emit("    jle    " + elseLabel + "\n");
    if (node.thenBranch)
      node.thenBranch->accept(*this);
    emit(elseLabel + ":\n");
  }
}

void CodeGenerator::visitWhileStmt(const WhileStmt &node) {
  std::string startLabel = makeLabel();
  std::string endLabel = makeLabel();
  emit(startLabel + ":\n");
  if (node.condition->kind == NodeKind::BinaryExpr) {
    const auto *be = static_cast<const BinaryExpr *>(node.condition.get());
    const auto *var = dynamic_cast<const VariableExpr *>(be->left.get());
    const auto *lit = dynamic_cast<const LiteralExpr *>(be->right.get());
    if (var && lit && be->op == "<") {
      std::string val = lit->value;
      if (isFloatLiteral(val))
        val = floatToHex(val);
      emit("    mov    rax, [" + var->name + "]\n");
      emit("    cmp    rax, " + val + "\n");
      emit("    jge    " + endLabel + "\n");
      if (node.body)
        node.body->accept(*this);
      emit("    jmp    " + startLabel + "\n");
      emit(endLabel + ":\n");
      return;
    }
  }
  genExpr(node.condition.get());
  emit("    cmp    rax, 0\n");
  emit("    jle    " + endLabel + "\n");
  if (node.body)
    node.body->accept(*this);
  emit("    jmp    " + startLabel + "\n");
  emit(endLabel + ":\n");
}

void CodeGenerator::visitForStmt(const ForStmt &node) {
  // Labels for loop start and exit
  std::string startLabel = makeLabel();
  std::string endLabel = makeLabel();

  // 1. initialize loop variable
  if (node.init)
    node.init->accept(*this);

  // 2. loop header
  emit(startLabel + ":\n");
  // load loop variable into RAX
  const auto *initVar =
      static_cast<const VariableExpr *>(node.init->target.get());
  emit("    mov    rax, [" + initVar->name + "]\n");

  // load limit into RBX (literal or computed)
  if (node.limit->kind == NodeKind::LiteralExpr) {
    auto *lit = static_cast<const LiteralExpr *>(node.limit.get());
    emit("    mov    rbx, " + lit->value + "\n");
  } else {
    genExpr(node.limit.get()); // result in RAX
    emit("    mov    rbx, rax\n");
  }

  // compare RAX (loop var) vs RBX (limit)
  if (node.downto) {
    emit("    cmp    rax, rbx\n");
    emit("    jl     " + endLabel + "\n");
  } else {
    emit("    cmp    rax, rbx\n");
    emit("    jg     " + endLabel + "\n");
  }

  // 3. body
  if (node.body)
    node.body->accept(*this);

  // 4. step
  const char *op = node.downto ? "sub" : "add";
  emit("    " + std::string(op) + "    qword [" + initVar->name + "], 1\n");

  // 5. back to header
  emit("    jmp    " + startLabel + "\n");
  emit(endLabel + ":\n");
}

void CodeGenerator::visitRepeatStmt(const RepeatStmt &node) {
  std::string startLabel = makeLabel();
  emit(startLabel + ":\n");
  for (const auto &s : node.body)
    if (s)
      s->accept(*this);
  bool optimized = false;
  if (node.condition->kind == NodeKind::BinaryExpr) {
    const auto *be = static_cast<const BinaryExpr *>(node.condition.get());
    const auto *var = dynamic_cast<const VariableExpr *>(be->left.get());
    const auto *lit = dynamic_cast<const LiteralExpr *>(be->right.get());
    if (var && lit && be->op == "=" && lit->value == "0") {
      emit("    cmp    rax, 0\n");
      emit("    jne    " + startLabel + "\n");
      optimized = true;
    }
  }
  if (!optimized) {
    genExpr(node.condition.get());
    emit("    cmp    rax, 0\n");
    emit("    jne    " + startLabel + "\n");
  }
}

void CodeGenerator::visitCaseStmt(const CaseStmt &node) {
  std::string endLabel = makeLabel();
  genExpr(node.expr.get());
  const auto *cl = node.cases.front().get();
  const auto *constExpr =
      static_cast<const LiteralExpr *>(cl->constants.front().get());
  emit("    cmp    rax, " + constExpr->value + "\n");
  emit("    jne    " + endLabel + "\n");
  cl->stmt->accept(*this);
  emit(endLabel + ":\n");
}

void CodeGenerator::visitWithStmt(const WithStmt &node) {
  const auto *var = dynamic_cast<const VariableExpr *>(node.recordExpr.get());
  const auto *as = dynamic_cast<const AssignStmt *>(node.body.get());
  if (var && as) {
    if (as->value->kind == NodeKind::LiteralExpr) {
      const auto *lit = static_cast<const LiteralExpr *>(as->value.get());
      std::string val = lit->value;
      if (isFloatLiteral(val))
        val = floatToHex(val);
      emit("    mov    qword [" + var->name + "], " + val + "\n");
    } else {
      genExpr(as->value.get());
      emit("    mov    qword [" + var->name + "], rax\n");
    }
  }
}

void CodeGenerator::emit(const std::string &text) { m_output += text; }

std::string CodeGenerator::makeLabel() {
  std::ostringstream oss;
  oss << "L" << ++m_labelCounter;
  return oss.str();
}

void CodeGenerator::addVar(const std::string &name, size_t count) {
  if (m_varSet.insert(name).second)
    m_vars.emplace_back(name, count);
}

std::string CodeGenerator::addString(const std::string &value) {
  auto it = m_stringMap.find(value);
  if (it != m_stringMap.end())
    return it->second;
  std::ostringstream oss;
  oss << "str" << m_strings.size();
  std::string label = oss.str();
  m_stringMap[value] = label;
  m_strings.emplace_back(label, value);
  return label;
}

const TypeSpec *CodeGenerator::getVarType(const std::string &name) const {
  auto it = m_varTypes.find(name);
  if (it != m_varTypes.end())
    return it->second;
  return nullptr;
}

size_t CodeGenerator::fieldOffset(const RecordTypeSpec *rt,
                                  const std::string &field,
                                  const TypeSpec **fieldType) const {
  size_t off = 0;
  if (!rt)
    return off;
  for (const auto &f : rt->fields) {
    for (const auto &n : f->names) {
      if (n == field) {
        if (fieldType)
          *fieldType = f->type.get();
        return off;
      }
    }
    off += typeSize(f->type.get());
  }
  return off;
}

void CodeGenerator::genVarAddr(const VariableExpr *var) {
  if (!var)
    return;
  auto it = m_paramMap.find(var->name);
  if (it != m_paramMap.end() && var->selectors.empty()) {
    emit("    lea    rax, [" + it->second + "]\n");
    return;
  }
  emit("    lea    rax, [" + var->name + "]\n");
  const TypeSpec *type = getVarType(var->name);
  if (auto *st = dynamic_cast<const SimpleTypeSpec *>(type)) {
    auto it = m_typeDefs.find(st->name);
    if (it != m_typeDefs.end())
      type = it->second;
  }
  for (const auto &sel : var->selectors) {
    if (sel.kind == VariableExpr::Selector::Kind::Pointer) {
      emit("    mov    rax, [rax]\n");
      if (auto *pt = dynamic_cast<const PointerTypeSpec *>(type))
        type = pt->refType.get();
      if (auto *st = dynamic_cast<const SimpleTypeSpec *>(type)) {
        auto it = m_typeDefs.find(st->name);
        if (it != m_typeDefs.end())
          type = it->second;
      }
    } else if (sel.kind == VariableExpr::Selector::Kind::Field) {
      const auto *rt = dynamic_cast<const RecordTypeSpec *>(type);
      const TypeSpec *ft = nullptr;
      size_t off = fieldOffset(rt, sel.field, &ft);
      emit("    lea    rax, [rax + " + std::to_string(off * 8) + "]\n");
      type = ft;
    } else if (sel.kind == VariableExpr::Selector::Kind::Index) {
      genExpr(sel.index.get());
      emit("    mov    rcx, rax\n");
      emit("    sub    rcx, 1\n");
      emit("    imul   rcx, 8\n");
      emit("    lea    rax, [rax + rcx]\n");
      if (auto *at = dynamic_cast<const ArrayTypeSpec *>(type))
        type = at->elementType.get();
    }
  }
}

size_t CodeGenerator::typeSize(const TypeSpec *type) const {
  if (!type)
    return 0;
  switch (type->kind) {
  case NodeKind::SimpleTypeSpec: {
    const auto *st = static_cast<const SimpleTypeSpec *>(type);
    auto it = m_typeDefs.find(st->name);
    if (it != m_typeDefs.end())
      return typeSize(it->second);
    return st->size();
  }
  case NodeKind::ArrayTypeSpec: {
    const auto *at = static_cast<const ArrayTypeSpec *>(type);
    size_t count = 1;
    for (const auto &r : at->ranges)
      count *= static_cast<size_t>(r.end - r.start + 1);
    return count * typeSize(at->elementType.get());
  }
  case NodeKind::RecordTypeSpec: {
    const auto *rt = static_cast<const RecordTypeSpec *>(type);
    size_t total = 0;
    for (const auto &f : rt->fields)
      total += typeSize(f->type.get());
    return total;
  }
  case NodeKind::PointerTypeSpec: {
    return 1; // pointer itself
  }
  default:
    return type->size();
  }
}

void CodeGenerator::collectVars(const ASTNode *node) {
  if (!node)
    return;
  switch (node->kind) {

  case NodeKind::TypeDefinition: {
    const auto *td = static_cast<const TypeDefinition *>(node);
    m_typeDefs[td->name] = td->type.get();
    // Type definitions are not directly translated to code, so we skip them.
    // They are used for type checking and validation.
    collectVars(td->type.get());
    break;
  }

  case NodeKind::Program: {
    const auto *p = static_cast<const Program *>(node);
    collectVars(p->block.get());
    break;
  }
  case NodeKind::Block: {
    const auto *b = static_cast<const Block *>(node);
    for (const auto &d : b->declarations)
      collectVars(d.get());
    for (const auto &s : b->statements)
      collectVars(s.get());
    break;
  }
  case NodeKind::VarDecl: {
    const auto *vd = static_cast<const VarDecl *>(node);
    for (const auto &n : vd->names) {
      addVar(n, vd->type->size());
      if (auto *pt = dynamic_cast<const PointerTypeSpec *>(vd->type.get()))
        m_ptrSizes[n] = typeSize(pt->refType.get());
      m_varTypes[n] = vd->type.get();
    }
    break;
  }

  case NodeKind::VarSection: {
    const auto *vs = static_cast<const VarSection *>(node);
    for (const auto &decl : vs->declarations) {

      collectVars(&decl);
    }
    break;
  }

  case NodeKind::ProcedureDecl: {
    const auto *pd = static_cast<const ProcedureDecl *>(node);
    auto savedF = m_currentFunction;
    auto savedP = m_params;
    m_currentFunction.clear();
    for (const auto &p : pd->params)
      for (const auto &n : p->names)
        m_params.insert(n);
    collectVars(pd->body.get());
    m_params = savedP;
    m_currentFunction = savedF;
    break;
  }
  case NodeKind::FunctionDecl: {
    const auto *fd = static_cast<const FunctionDecl *>(node);
    auto savedF = m_currentFunction;
    auto savedP = m_params;
    m_currentFunction = fd->name;
    for (const auto &p : fd->params)
      for (const auto &n : p->names)
        m_params.insert(n);
    collectVars(fd->body.get());
    m_params = savedP;
    m_currentFunction = savedF;
    break;
  }
  case NodeKind::AssignStmt: {
    const auto *as = static_cast<const AssignStmt *>(node);
    collectVars(as->value.get());
    collectVars(as->target.get());
    break;
  }
  case NodeKind::CompoundStmt: {
    const auto *cs = static_cast<const CompoundStmt *>(node);
    for (const auto &s : cs->statements)
      collectVars(s.get());
    break;
  }
  case NodeKind::BinaryExpr: {
    const auto *be = static_cast<const BinaryExpr *>(node);
    collectVars(be->left.get());
    collectVars(be->right.get());
    break;
  }
  case NodeKind::VariableExpr: {
    const auto *ve = static_cast<const VariableExpr *>(node);
    if (ve->name != "nil" && ve->name != m_currentFunction &&
        m_params.find(ve->name) == m_params.end())
      addVar(ve->name, 1);
    for (const auto &sel : ve->selectors)
      if (sel.kind == VariableExpr::Selector::Kind::Index && sel.index)
        collectVars(sel.index.get());
    break;
  }
  case NodeKind::LiteralExpr: {
    const auto *le = static_cast<const LiteralExpr *>(node);
    if (!le->value.empty() && le->value.front() == '\'' &&
        le->value.back() == '\'')
      addString(le->value.substr(1, le->value.size() - 2));
    break;
  }
  case NodeKind::IfStmt: {
    const auto *ifs = static_cast<const IfStmt *>(node);
    collectVars(ifs->condition.get());
    collectVars(ifs->thenBranch.get());
    collectVars(ifs->elseBranch.get());
    break;
  }
  case NodeKind::WhileStmt: {
    const auto *ws = static_cast<const WhileStmt *>(node);
    collectVars(ws->condition.get());
    collectVars(ws->body.get());
    break;
  }
  case NodeKind::RepeatStmt: {
    const auto *rs = static_cast<const RepeatStmt *>(node);
    for (const auto &s : rs->body)
      collectVars(s.get());
    collectVars(rs->condition.get());
    break;
  }
  case NodeKind::ForStmt: {
    const auto *fs = static_cast<const ForStmt *>(node);
    collectVars(fs->init.get());
    collectVars(fs->limit.get());
    collectVars(fs->body.get());
    break;
  }
  case NodeKind::CaseStmt: {
    const auto *cs = static_cast<const CaseStmt *>(node);
    collectVars(cs->expr.get());
    for (const auto &cl : cs->cases)
      collectVars(cl.get());
    break;
  }
  case NodeKind::CaseLabel: {
    const auto *cl = static_cast<const CaseLabel *>(node);
    for (const auto &c : cl->constants)
      collectVars(c.get());
    collectVars(cl->stmt.get());
    break;
  }
  case NodeKind::ProcCall: {
    const auto *pc = static_cast<const ProcCall *>(node);
    for (const auto &a : pc->args)
      collectVars(a.get());
    if (pc->name == "new")
      m_needMalloc = true;
    else if (pc->name == "dispose")
      m_needFree = true;
    else if (pc->name == "writeln") {
      if (!pc->args.empty()) {

        // only string literals or known string vars → puts, else → printf
        if (auto *lit = dynamic_cast<const LiteralExpr *>(pc->args[0].get())) {

          // literal string?
          if (lit->value.size() >= 2 && lit->value.front() == '\'' &&
              lit->value.back() == '\'') {
            m_needPuts = true;
          } else {
            m_needPrintf = true;
          }
        } else {
          // any non‐literal (variable or expr) is numeric → printf
          m_needPrintf = true;
        }

      } else if (const auto *lit =
                     dynamic_cast<const LiteralExpr *>(pc->args[0].get())) {

        cout << "Literal value: " << lit->value << endl;

        if (lit->value.size() >= 2 && lit->value.front() == '\'' &&
            lit->value.back() == '\'')
          m_needPuts = true;
        else
          m_needPrintf = true;
      } else {
        m_needPrintf = true;
      }
    }
    break;
  }

  case NodeKind::UnaryExpr: {
    const auto *ue = static_cast<const UnaryExpr *>(node);
    collectVars(ue->operand.get());
    break;
  }
  case NodeKind::NewExpr: {
    const auto *ne = static_cast<const NewExpr *>(node);
    collectVars(ne->variable.get());
    break;
  }
  case NodeKind::DisposeExpr: {
    const auto *de = static_cast<const DisposeExpr *>(node);
    collectVars(de->variable.get());
    break;
  }
  case NodeKind::WithStmt: {
    const auto *ws = static_cast<const WithStmt *>(node);
    collectVars(ws->recordExpr.get());
    const auto *as = dynamic_cast<const AssignStmt *>(ws->body.get());
    if (as)
      collectVars(as->value.get());
    break;
  }
  case NodeKind::ConstDecl: {
    const auto *cd = static_cast<const ConstDecl *>(node);
    collectVars(cd->value.get());
    break;
  }
  case NodeKind::TypeDecl: {
    const auto *td = static_cast<const TypeDecl *>(node);

    for (const auto &tDef : td->definitions) {
      m_typeDefs[tDef.name] = tDef.type.get();
      collectVars(tDef.type.get());
    }

    break;
  }
  case NodeKind::ParamDecl: {
    const auto *pd = static_cast<const ParamDecl *>(node);
    collectVars(pd->type.get());
    if (auto *pt = dynamic_cast<const PointerTypeSpec *>(pd->type.get())) {
      for (const auto &n : pd->names)
        m_ptrSizes[n] = typeSize(pt->refType.get());
    }
    for (const auto &n : pd->names)
      m_varTypes[n] = pd->type.get();
    break;
  }
  case NodeKind::RecordTypeSpec: {
    // Record fields are handled as part of the type; no variables to collect.
    break;
  }
  case NodeKind::PointerTypeSpec: {
    const auto *pt = static_cast<const PointerTypeSpec *>(node);
    collectVars(pt->refType.get());
    break;
  }
  case NodeKind::ArrayTypeSpec: {
    const auto *at = static_cast<const ArrayTypeSpec *>(node);
    collectVars(at->elementType.get());
    break;
  }
  case NodeKind::SimpleTypeSpec:
  case NodeKind::TypeSpec:
  case NodeKind::Range:
  case NodeKind::IdentifierList:
    break;
  default:
    break;
  }
}

} // namespace pascal
