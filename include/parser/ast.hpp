#ifndef PASCAL_COMPILER_AST_HPP
#define PASCAL_COMPILER_AST_HPP

#include "token/types.hpp"
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace pascal {

enum class NodeKind {
  Program,
  Block,
  VarDecl,
  TypeDecl,
  TypeDefinition,
  ConstDecl,
  ProcedureDecl,
  FunctionDecl,
  ParamDecl,
  CompoundStmt,
  AssignStmt,
  ProcCall,
  IfStmt,
  WhileStmt,
  ForStmt,
  RepeatStmt,
  CaseStmt,
  WithStmt,
  BinaryExpr,
  UnaryExpr,
  LiteralExpr,
  VarSection,
  VariableExpr,
  Range,
  TypeSpec,
  SimpleTypeSpec,
  ArrayTypeSpec,
  RecordTypeSpec,
  PointerTypeSpec,
  CaseLabel,
  NewExpr,
  DisposeExpr,
  IdentifierList
};

inline std::ostream &operator<<(std::ostream &os, const NodeKind kind) {
  switch (kind) {
  case NodeKind::Program:
    return os << "Program";
  case NodeKind::Block:
    return os << "Block";
  case NodeKind::VarDecl:
    return os << "VarDecl";
  case NodeKind::TypeDecl:
    return os << "TypeDecl";
  case NodeKind::TypeDefinition:
    return os << "TypeDefinition";
  case NodeKind::ConstDecl:
    return os << "ConstDecl";
  case NodeKind::ProcedureDecl:
    return os << "ProcedureDecl";
  case NodeKind::FunctionDecl:
    return os << "FunctionDecl";
  case NodeKind::ParamDecl:
    return os << "ParamDecl";
  case NodeKind::CompoundStmt:
    return os << "CompoundStmt";
  case NodeKind::AssignStmt:
    return os << "AssignStmt";
  case NodeKind::ProcCall:
    return os << "ProcCall";
  case NodeKind::IfStmt:
    return os << "IfStmt";
  case NodeKind::WhileStmt:
    return os << "WhileStmt";
  case NodeKind::ForStmt:
    return os << "ForStmt";
  case NodeKind::RepeatStmt:
    return os << "RepeatStmt";
  case NodeKind::CaseStmt:
    return os << "CaseStmt";
  case NodeKind::WithStmt:
    return os << "WithStmt";
  case NodeKind::BinaryExpr:
    return os << "BinaryExpr";
  case NodeKind::UnaryExpr:
    return os << "UnaryExpr";
  case NodeKind::LiteralExpr:
    return os << "LiteralExpr";
  case NodeKind::VarSection:
    return os << "VarSection";
  case NodeKind::VariableExpr:
    return os << "VariableExpr";
  case NodeKind::Range:
    return os << "Range";
  case NodeKind::TypeSpec:
    return os << "TypeSpec";
  case NodeKind::SimpleTypeSpec:
    return os << "SimpleTypeSpec";
  case NodeKind::ArrayTypeSpec:
    return os << "ArrayTypeSpec";
  case NodeKind::RecordTypeSpec:
    return os << "RecordTypeSpec";
  case NodeKind::PointerTypeSpec:
    return os << "PointerTypeSpec";
  case NodeKind::CaseLabel:
    return os << "CaseLabel";
  case NodeKind::NewExpr:
    return os << "NewExpr";
  case NodeKind::DisposeExpr:
    return os << "DisposeExpr";
  case NodeKind::IdentifierList:
    return os << "IdentifierList";
  }
}

// Forward declarations for visitor
struct Program;
struct Block;
struct VarDecl;
struct TypeDecl;
struct ConstDecl;
struct ProcedureDecl;
struct FunctionDecl;
struct ParamDecl;
struct CompoundStmt;
struct AssignStmt;
struct ProcCall;
struct IfStmt;
struct WhileStmt;
struct ForStmt;
struct RepeatStmt;
struct CaseStmt;
struct WithStmt;
struct BinaryExpr;
struct UnaryExpr;
struct LiteralExpr;
struct VariableExpr;
struct Range;
struct TypeSpec;
struct SimpleTypeSpec;
struct ArrayTypeSpec;
struct RecordTypeSpec;
struct PointerTypeSpec;
struct CaseLabel;
struct NewExpr;
struct DisposeExpr;
struct IdentifierList;
struct VarSection;
struct TypeDefinition;

class NodeVisitor {
public:
  virtual ~NodeVisitor();
  virtual void visitTypeDefinition(const TypeDefinition &) = 0;
  virtual void visitVarSection(const VarSection &) = 0;
  virtual void visitProgram(const Program &) = 0;
  virtual void visitIdentifierList(const IdentifierList &) = 0;
  virtual void visitBlock(const Block &) = 0;
  virtual void visitVarDecl(const VarDecl &) = 0;
  virtual void visitParamDecl(const ParamDecl &) = 0;
  virtual void visitTypeDecl(const TypeDecl &) = 0;
  virtual void visitConstDecl(const ConstDecl &) = 0;
  virtual void visitProcedureDecl(const ProcedureDecl &) = 0;
  virtual void visitFunctionDecl(const FunctionDecl &) = 0;
  virtual void visitCompoundStmt(const CompoundStmt &) = 0;
  virtual void visitAssignStmt(const AssignStmt &) = 0;
  virtual void visitProcCall(const ProcCall &) = 0;
  virtual void visitIfStmt(const IfStmt &) = 0;
  virtual void visitWhileStmt(const WhileStmt &) = 0;
  virtual void visitForStmt(const ForStmt &) = 0;
  virtual void visitRepeatStmt(const RepeatStmt &) = 0;
  virtual void visitCaseStmt(const CaseStmt &) = 0;
  virtual void visitWithStmt(const WithStmt &) = 0;
  virtual void visitBinaryExpr(const BinaryExpr &) = 0;
  virtual void visitUnaryExpr(const UnaryExpr &) = 0;
  virtual void visitLiteralExpr(const LiteralExpr &) = 0;
  virtual void visitVariableExpr(const VariableExpr &) = 0;
  virtual void visitRange(const Range &) = 0;
  virtual void visitTypeSpec(const TypeSpec &) = 0;
  virtual void visitSimpleTypeSpec(const SimpleTypeSpec &) = 0;
  virtual void visitArrayTypeSpec(const ArrayTypeSpec &) = 0;
  virtual void visitRecordTypeSpec(const RecordTypeSpec &) = 0;
  virtual void visitPointerTypeSpec(const PointerTypeSpec &) = 0;
  virtual void visitCaseLabel(const CaseLabel &) = 0;
  virtual void visitNewExpr(const NewExpr &) = 0;
  virtual void visitDisposeExpr(const DisposeExpr &) = 0;
};

struct ASTNode {
  ASTNode(const ASTNode &) = default;
  ASTNode(ASTNode &&) = delete;
  ASTNode &operator=(const ASTNode &) = default;
  ASTNode &operator=(ASTNode &&) = delete;
  explicit ASTNode(NodeKind k, std::size_t l = 0, std::size_t c = 0)
      : kind(k), line(l), column(c) {}
  virtual ~ASTNode();
  virtual void accept(NodeVisitor &v) const = 0;

  NodeKind kind;
  std::size_t line{0};
  std::size_t column{0};
};
inline std::ostream &operator<<(std::ostream &os, const ASTNode &node) {
  return os << "ASTNode(" << node.kind << ", line: " << node.line
            << ", column: " << node.column << ")";
}

struct Expression : ASTNode {
  using ASTNode::ASTNode;
  ~Expression() override;
};

struct Statement : ASTNode {
  using ASTNode::ASTNode;
  ~Statement() override;
};
struct Declaration : ASTNode {
  Declaration(const Declaration &) = delete;
  Declaration &operator=(const Declaration &) = delete;
  using ASTNode::ASTNode;
  ~Declaration() override;
};

struct Program : ASTNode {
  std::string name;
  std::unique_ptr<Block> block;

  Program();
  Program(std::string n, std::unique_ptr<Block> b);

  ~Program() override;

  void accept(NodeVisitor &v) const override { v.visitProgram(*this); }
};

struct Block : ASTNode {
  std::vector<std::unique_ptr<Declaration>> declarations;
  std::vector<std::unique_ptr<Statement>> statements;

  Block() : ASTNode(NodeKind::Block) {}
  Block(std::vector<std::unique_ptr<Declaration>> decls,
        std::vector<std::unique_ptr<Statement>> stmts)
      : ASTNode(NodeKind::Block), declarations(std::move(decls)),
        statements(std::move(stmts)) {}

  ~Block() override;

  void accept(NodeVisitor &v) const override { v.visitBlock(*this); }
};

struct IdentifierList : ASTNode {
  std::vector<std::string> identifiers;
  IdentifierList() : ASTNode(NodeKind::IdentifierList) {}

  IdentifierList(const std::vector<std::string> &ids)
      : ASTNode(NodeKind::IdentifierList), identifiers(ids) {}

  IdentifierList(const IdentifierList &) = default;

  IdentifierList(IdentifierList &&id_list)
      : ASTNode(NodeKind::IdentifierList),
        identifiers(std::move(id_list.identifiers)) {}
  IdentifierList &operator=(const IdentifierList &) = default;

  IdentifierList &operator=(IdentifierList &&id_list) {
    if (this != &id_list) {
      identifiers = std::move(id_list.identifiers);
    }
    return *this;
  }
  ~IdentifierList() override;
  void accept(NodeVisitor &v) const override { v.visitIdentifierList(*this); }

  auto empty() const { return identifiers.empty(); }

  // Iterator support
  auto begin() { return identifiers.begin(); }
  auto end() { return identifiers.end(); }

  auto begin() const { return identifiers.begin(); }
  auto end() const { return identifiers.end(); }
};

struct VarDecl : Declaration {
  IdentifierList names;
  std::unique_ptr<TypeSpec> type;

  VarDecl();
  VarDecl(IdentifierList n, std::unique_ptr<TypeSpec> t, std::size_t _line = 0,
          std::size_t _column = 0);

  VarDecl(VarDecl &&other);
  VarDecl &operator=(VarDecl &&other);

  VarDecl(const VarDecl &) = delete;
  VarDecl &operator=(const VarDecl &) = delete;

  ~VarDecl() override;

  void accept(NodeVisitor &v) const override { v.visitVarDecl(*this); }
};

struct ConstDecl : Declaration {
  std::string name;
  std::unique_ptr<Expression> value;

  ConstDecl() : Declaration(NodeKind::ConstDecl) {}
  ConstDecl(std::string n, std::unique_ptr<Expression> v)
      : Declaration(NodeKind::ConstDecl), name(std::move(n)),
        value(std::move(v)) {}

  ~ConstDecl() override;

  void accept(NodeVisitor &v) const override { v.visitConstDecl(*this); }
};

struct TypeDefinition : Declaration {
  std::string name;
  std::unique_ptr<TypeSpec> type;
  TypeDefinition();
  TypeDefinition(TypeDefinition &&other);
  TypeDefinition &operator=(TypeDefinition &&) = delete;
  TypeDefinition(const TypeDefinition &) = delete;
  TypeDefinition &operator=(const TypeDefinition &) = delete;
  TypeDefinition(std::string n, std::unique_ptr<TypeSpec> &t);
  ~TypeDefinition() override;

  void accept(NodeVisitor &v) const override { v.visitTypeDefinition(*this); }
};

struct VarSection : Declaration {

  std::vector<VarDecl> declarations;
  VarSection() : Declaration(NodeKind::VarSection) {}
  explicit VarSection(std::vector<VarDecl> &decls, std::size_t _line = 0,
                      std::size_t _column = 0);
  ~VarSection() override;
  void accept(NodeVisitor &v) const override { v.visitVarSection(*this); }
};

struct TypeDecl : Declaration {

  std::vector<TypeDefinition> definitions;

  TypeDecl();
  TypeDecl(std::vector<TypeDefinition> &defs, std::size_t _line = 0,
           std::size_t _column = 0);

  TypeDecl(const TypeDecl &) = delete;
  TypeDecl &operator=(const TypeDecl &) = delete;

  ~TypeDecl() override;

  void accept(NodeVisitor &v) const override { v.visitTypeDecl(*this); }
};

struct ProcedureDecl : Declaration {
  std::string name;
  std::vector<std::unique_ptr<ParamDecl>> params;
  std::unique_ptr<Block> body;

  ProcedureDecl();
  ProcedureDecl(std::string n, std::vector<std::unique_ptr<ParamDecl>> p,
                std::unique_ptr<Block> b);

  ~ProcedureDecl() override;

  void accept(NodeVisitor &v) const override { v.visitProcedureDecl(*this); }
};

struct ParamDecl : Declaration {
  std::vector<std::string> names;
  std::unique_ptr<TypeSpec> type;

  ParamDecl();
  ParamDecl(std::vector<std::string> n, std::unique_ptr<TypeSpec> t);

  ~ParamDecl() override;

  void accept(NodeVisitor &v) const override { v.visitParamDecl(*this); }
};

struct FunctionDecl : Declaration {
  std::string name;
  std::vector<std::unique_ptr<ParamDecl>> params;
  std::unique_ptr<TypeSpec> returnType;
  std::unique_ptr<Block> body;

  FunctionDecl();
  FunctionDecl(std::string n, std::vector<std::unique_ptr<ParamDecl>> p,
               std::unique_ptr<TypeSpec> r, std::unique_ptr<Block> b);

  ~FunctionDecl() override;

  void accept(NodeVisitor &v) const override { v.visitFunctionDecl(*this); }
};

struct CompoundStmt : Statement {
  std::vector<std::unique_ptr<Statement>> statements;

  CompoundStmt() : Statement(NodeKind::CompoundStmt) {}
  explicit CompoundStmt(std::vector<std::unique_ptr<Statement>> stmts)
      : Statement(NodeKind::CompoundStmt), statements(std::move(stmts)) {}

  ~CompoundStmt() override;

  void accept(NodeVisitor &v) const override { v.visitCompoundStmt(*this); }
};

struct AssignStmt : Statement {
  std::unique_ptr<Expression> target;
  std::unique_ptr<Expression> value;

  AssignStmt() : Statement(NodeKind::AssignStmt) {}
  AssignStmt(std::unique_ptr<Expression> t, std::unique_ptr<Expression> v)
      : Statement(NodeKind::AssignStmt), target(std::move(t)),
        value(std::move(v)) {}

  ~AssignStmt() override;

  void accept(NodeVisitor &v) const override { v.visitAssignStmt(*this); }
};

struct ProcCall : Statement {
  std::string name;
  std::vector<std::unique_ptr<Expression>> args;

  ProcCall() : Statement(NodeKind::ProcCall) {}
  ProcCall(std::string n, std::vector<std::unique_ptr<Expression>> a)
      : Statement(NodeKind::ProcCall), name(std::move(n)), args(std::move(a)) {}

  ~ProcCall() override;

  void accept(NodeVisitor &v) const override { v.visitProcCall(*this); }
};

struct IfStmt : Statement {
  std::unique_ptr<Expression> condition;
  std::unique_ptr<Statement> thenBranch;
  std::unique_ptr<Statement> elseBranch;

  IfStmt() : Statement(NodeKind::IfStmt) {}
  IfStmt(std::unique_ptr<Expression> cond, std::unique_ptr<Statement> thenB,
         std::unique_ptr<Statement> elseB = {})
      : Statement(NodeKind::IfStmt), condition(std::move(cond)),
        thenBranch(std::move(thenB)), elseBranch(std::move(elseB)) {}

  ~IfStmt() override;

  void accept(NodeVisitor &v) const override { v.visitIfStmt(*this); }
};

struct WhileStmt : Statement {
  std::unique_ptr<Expression> condition;
  std::unique_ptr<Statement> body;

  WhileStmt() : Statement(NodeKind::WhileStmt) {}
  WhileStmt(std::unique_ptr<Expression> cond, std::unique_ptr<Statement> b)
      : Statement(NodeKind::WhileStmt), condition(std::move(cond)),
        body(std::move(b)) {}

  ~WhileStmt() override;

  void accept(NodeVisitor &v) const override { v.visitWhileStmt(*this); }
};

struct ForStmt : Statement {
  std::unique_ptr<AssignStmt> init;
  bool downto{false};
  std::unique_ptr<Expression> limit;
  std::unique_ptr<Statement> body;

  ForStmt() : Statement(NodeKind::ForStmt) {}
  ForStmt(std::unique_ptr<AssignStmt> i, bool d, std::unique_ptr<Expression> l,
          std::unique_ptr<Statement> b)
      : Statement(NodeKind::ForStmt), init(std::move(i)), downto(d),
        limit(std::move(l)), body(std::move(b)) {}

  ~ForStmt() override;

  void accept(NodeVisitor &v) const override { v.visitForStmt(*this); }
};

struct RepeatStmt : Statement {
  std::vector<std::unique_ptr<Statement>> body;
  std::unique_ptr<Expression> condition;

  RepeatStmt() : Statement(NodeKind::RepeatStmt) {}
  RepeatStmt(std::vector<std::unique_ptr<Statement>> b,
             std::unique_ptr<Expression> cond)
      : Statement(NodeKind::RepeatStmt), body(std::move(b)),
        condition(std::move(cond)) {}

  ~RepeatStmt() override;

  void accept(NodeVisitor &v) const override { v.visitRepeatStmt(*this); }
};

struct CaseStmt : Statement {
  std::unique_ptr<Expression> expr;
  std::vector<std::unique_ptr<CaseLabel>> cases;

  CaseStmt();
  CaseStmt(std::unique_ptr<Expression> e,
           std::vector<std::unique_ptr<CaseLabel>> c);

  ~CaseStmt() override;

  void accept(NodeVisitor &v) const override { v.visitCaseStmt(*this); }
};

struct WithStmt : Statement {
  std::unique_ptr<Expression> recordExpr;
  std::unique_ptr<Statement> body;

  WithStmt() : Statement(NodeKind::WithStmt) {}
  WithStmt(std::unique_ptr<Expression> expr, std::unique_ptr<Statement> b)
      : Statement(NodeKind::WithStmt), recordExpr(std::move(expr)),
        body(std::move(b)) {}

  ~WithStmt() override;

  void accept(NodeVisitor &v) const override { v.visitWithStmt(*this); }
};

struct BinaryExpr : Expression {
  std::unique_ptr<Expression> left;
  std::unique_ptr<Expression> right;
  std::string op;

  BinaryExpr() : Expression(NodeKind::BinaryExpr) {}
  BinaryExpr(std::unique_ptr<Expression> l, std::string o,
             std::unique_ptr<Expression> r)
      : Expression(NodeKind::BinaryExpr), left(std::move(l)),
        right(std::move(r)), op(std::move(o)) {}

  ~BinaryExpr() override;

  void accept(NodeVisitor &v) const override { v.visitBinaryExpr(*this); }
};

struct UnaryExpr : Expression {
  std::unique_ptr<Expression> operand;
  std::string op;

  UnaryExpr() : Expression(NodeKind::UnaryExpr) {}
  UnaryExpr(std::string o, std::unique_ptr<Expression> oper)
      : Expression(NodeKind::UnaryExpr), operand(std::move(oper)),
        op(std::move(o)) {}

  ~UnaryExpr() override;

  void accept(NodeVisitor &v) const override { v.visitUnaryExpr(*this); }
};

struct LiteralExpr : Expression {
  std::string value;

  LiteralExpr() : Expression(NodeKind::LiteralExpr) {}
  explicit LiteralExpr(std::string val)
      : Expression(NodeKind::LiteralExpr), value(std::move(val)) {}

  ~LiteralExpr() override;

  void accept(NodeVisitor &v) const override { v.visitLiteralExpr(*this); }
};

struct VariableExpr : Expression {
  std::string name;
  struct Selector {
    enum class Kind { Field, Index, Pointer };
    Kind kind{Kind::Field};
    std::string field;
    std::unique_ptr<Expression> index;

    Selector() = default;
    Selector(std::string f, Kind k)
        : kind(k), field(std::move(f)), index(nullptr) {}
    Selector(std::unique_ptr<Expression> idx)
        : kind(Kind::Index), index(std::move(idx)) {}
  };
  std::vector<Selector> selectors;

  VariableExpr() : Expression(NodeKind::VariableExpr) {}
  explicit VariableExpr(std::string n)
      : Expression(NodeKind::VariableExpr), name(std::move(n)) {}
  VariableExpr(std::string n, std::vector<Selector> sels)
      : Expression(NodeKind::VariableExpr), name(std::move(n)),
        selectors(std::move(sels)) {}

  ~VariableExpr() override;

  void accept(NodeVisitor &v) const override { v.visitVariableExpr(*this); }
};

struct Range : ASTNode {
  int start{0};
  int end{0};

  Range() : ASTNode(NodeKind::Range) {}
  Range(const Range &) = default;
  Range(Range &&) = delete;
  Range &operator=(const Range &) = default;
  Range &operator=(Range &&) = delete;
  Range(int s, int e) : ASTNode(NodeKind::Range), start(s), end(e) {}

  ~Range() override;

  void accept(NodeVisitor &v) const override { v.visitRange(*this); }
};

struct TypeSpec : ASTNode {
  using ASTNode::ASTNode;

  virtual size_t size() const = 0;
  ~TypeSpec() override;
};

struct SimpleTypeSpec : TypeSpec {
  BasicType basic{BasicType::Integer};
  std::string name;

  size_t size() const override { return 1; }

  SimpleTypeSpec() : TypeSpec(NodeKind::SimpleTypeSpec) {}
  SimpleTypeSpec(BasicType b, std::string n = {})
      : TypeSpec(NodeKind::SimpleTypeSpec), basic(b), name(std::move(n)) {}

  ~SimpleTypeSpec() override;

  void accept(NodeVisitor &v) const override { v.visitSimpleTypeSpec(*this); }
};

struct ArrayTypeSpec : TypeSpec {
  std::vector<Range> ranges;
  std::unique_ptr<TypeSpec> elementType;

  ArrayTypeSpec();
  ArrayTypeSpec(std::vector<Range> r, std::unique_ptr<TypeSpec> elem);

  ~ArrayTypeSpec() override;

  void accept(NodeVisitor &v) const override { v.visitArrayTypeSpec(*this); }

  size_t size() const override {
    size_t count = 1;
    for (auto &r : ranges) {
      auto lo = r.start;
      auto hi = r.end;
      count *= static_cast<size_t>(hi - lo + 1);
    }
    return count * elementType->size();
  }
};

struct RecordTypeSpec : TypeSpec {
  std::vector<std::unique_ptr<VarDecl>> fields;

  RecordTypeSpec() : TypeSpec(NodeKind::RecordTypeSpec) {}
  explicit RecordTypeSpec(std::vector<std::unique_ptr<VarDecl>> f)
      : TypeSpec(NodeKind::RecordTypeSpec), fields(std::move(f)) {}

  ~RecordTypeSpec() override;

  void accept(NodeVisitor &v) const override { v.visitRecordTypeSpec(*this); }

  size_t size() const override {
    size_t total = 0;
    for (auto &fieldDecl : fields) {
      // each fieldDecl->type is a TypeSpec*
      total += fieldDecl->type->size();
    }
    return total;
  }
};

struct PointerTypeSpec : TypeSpec {
  std::unique_ptr<TypeSpec> refType;

  PointerTypeSpec();
  explicit PointerTypeSpec(std::unique_ptr<TypeSpec> r);

  ~PointerTypeSpec() override;

  size_t size() const override { return 1; }

  void accept(NodeVisitor &v) const override { v.visitPointerTypeSpec(*this); }
};

struct CaseLabel : ASTNode {
  std::vector<std::unique_ptr<Expression>> constants;
  std::unique_ptr<Statement> stmt;

  CaseLabel() : ASTNode(NodeKind::CaseLabel) {}
  CaseLabel(std::vector<std::unique_ptr<Expression>> c,
            std::unique_ptr<Statement> s)
      : ASTNode(NodeKind::CaseLabel), constants(std::move(c)),
        stmt(std::move(s)) {}

  ~CaseLabel() override;

  void accept(NodeVisitor &v) const override { v.visitCaseLabel(*this); }
};

struct NewExpr : Expression {
  std::unique_ptr<VariableExpr> variable;

  NewExpr() : Expression(NodeKind::NewExpr) {}
  explicit NewExpr(std::unique_ptr<VariableExpr> var)
      : Expression(NodeKind::NewExpr), variable(std::move(var)) {}

  ~NewExpr() override;

  void accept(NodeVisitor &v) const override { v.visitNewExpr(*this); }
};

struct DisposeExpr : Expression {
  std::unique_ptr<VariableExpr> variable;

  DisposeExpr() : Expression(NodeKind::DisposeExpr) {}
  explicit DisposeExpr(std::unique_ptr<VariableExpr> var)
      : Expression(NodeKind::DisposeExpr), variable(std::move(var)) {}

  ~DisposeExpr() override;

  void accept(NodeVisitor &v) const override { v.visitDisposeExpr(*this); }
};

struct AST {
  std::unique_ptr<Program> root{};
  bool valid{false};
};

inline Program::Program() : ASTNode(NodeKind::Program) {}
inline Program::Program(std::string n, std::unique_ptr<Block> b)
    : ASTNode(NodeKind::Program), name(std::move(n)), block(std::move(b)) {}

inline VarDecl::VarDecl() : Declaration(NodeKind::VarDecl) {}

inline TypeDecl::TypeDecl() : Declaration(NodeKind::TypeDecl) {}

inline ProcedureDecl::ProcedureDecl() : Declaration(NodeKind::ProcedureDecl) {}
inline ProcedureDecl::ProcedureDecl(std::string n,
                                    std::vector<std::unique_ptr<ParamDecl>> p,
                                    std::unique_ptr<Block> b)
    : Declaration(NodeKind::ProcedureDecl), name(std::move(n)),
      params(std::move(p)), body(std::move(b)) {}

inline ParamDecl::ParamDecl() : Declaration(NodeKind::ParamDecl) {}
inline ParamDecl::ParamDecl(std::vector<std::string> n,
                            std::unique_ptr<TypeSpec> t)
    : Declaration(NodeKind::ParamDecl), names(std::move(n)),
      type(std::move(t)) {}
inline TypeDefinition::TypeDefinition(TypeDefinition &&other)
    : Declaration(NodeKind::TypeDefinition), name(std::move(other.name)),
      type(std::move(other.type)) {}

inline FunctionDecl::FunctionDecl() : Declaration(NodeKind::FunctionDecl) {}
inline FunctionDecl::FunctionDecl(std::string n,
                                  std::vector<std::unique_ptr<ParamDecl>> p,
                                  std::unique_ptr<TypeSpec> r,
                                  std::unique_ptr<Block> b)
    : Declaration(NodeKind::FunctionDecl), name(std::move(n)),
      params(std::move(p)), returnType(std::move(r)), body(std::move(b)) {}

inline ArrayTypeSpec::ArrayTypeSpec() : TypeSpec(NodeKind::ArrayTypeSpec) {}
inline ArrayTypeSpec::ArrayTypeSpec(std::vector<Range> r,
                                    std::unique_ptr<TypeSpec> elem)
    : TypeSpec(NodeKind::ArrayTypeSpec), ranges(std::move(r)),
      elementType(std::move(elem)) {}

inline PointerTypeSpec::PointerTypeSpec()
    : TypeSpec(NodeKind::PointerTypeSpec) {}
inline PointerTypeSpec::PointerTypeSpec(std::unique_ptr<TypeSpec> r)
    : TypeSpec(NodeKind::PointerTypeSpec), refType(std::move(r)) {}
inline CaseStmt::CaseStmt() : Statement(NodeKind::CaseStmt) {}
inline CaseStmt::CaseStmt(std::unique_ptr<Expression> e,
                          std::vector<std::unique_ptr<CaseLabel>> c)
    : Statement(NodeKind::CaseStmt), expr(std::move(e)), cases(std::move(c)) {}
inline TypeDecl::TypeDecl(std::vector<TypeDefinition> &defs, std::size_t _line,
                          std::size_t _column)
    : Declaration(NodeKind::TypeDecl, _line, _column),
      definitions(std::move(defs)) {}
inline TypeDefinition::TypeDefinition(std::string n,
                                      std::unique_ptr<TypeSpec> &t)
    : Declaration(NodeKind::TypeDefinition), name(std::move(n)),
      type(std::move(t)) {}

inline VarSection::VarSection(std::vector<VarDecl> &decls, std::size_t _line,
                              std::size_t _column)
    : Declaration(NodeKind::VarSection, _line, _column),
      declarations(std::move(decls)) {}
inline VarDecl::VarDecl(VarDecl &&other)
    : Declaration(NodeKind::VarDecl), names(std::move(other.names)),
      type(std::move(other.type)) {}
inline VarDecl::VarDecl(IdentifierList n, std::unique_ptr<TypeSpec> t,
                        std::size_t _line, std::size_t _column)
    : Declaration(NodeKind::VarDecl, _line, _column), names(std::move(n)),
      type(std::move(t)) {}

inline TypeDefinition::TypeDefinition()
    : Declaration(NodeKind::TypeDefinition) {}

inline VarDecl &VarDecl::operator=(VarDecl &&other) {
  if (this != &other) {
    names = std::move(other.names);
    type = std::move(other.type);
  }
  return *this;
}

} // namespace pascal

#endif // PASCAL_COMPILER_AST_HPP
