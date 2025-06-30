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
  VariableExpr,
  Range,
  TypeSpec,
  SimpleTypeSpec,
  ArrayTypeSpec,
  RecordTypeSpec,
  PointerTypeSpec,
  CaseLabel,
  NewExpr,
  DisposeExpr
};

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

class NodeVisitor {
public:
  virtual ~NodeVisitor() = default;
  virtual void visitProgram(const Program &) = 0;
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
  explicit ASTNode(NodeKind k) : kind(k) {}
  virtual ~ASTNode() = default;
  virtual void accept(NodeVisitor &v) const = 0;

  NodeKind kind;
};

struct Expression : ASTNode {
  using ASTNode::ASTNode;
};

struct Statement : ASTNode {
  using ASTNode::ASTNode;
};

struct Declaration : ASTNode {
  using ASTNode::ASTNode;
};

struct Program : ASTNode {
  std::string name;
  std::unique_ptr<class Block> block;

  Program();
  Program(std::string n, std::unique_ptr<Block> b);

  ~Program();

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

  void accept(NodeVisitor &v) const override { v.visitBlock(*this); }
};

struct VarDecl : Declaration {
  std::vector<std::string> names;
  std::unique_ptr<TypeSpec> type;

  VarDecl();
  VarDecl(std::vector<std::string> n, std::unique_ptr<TypeSpec> t);

  ~VarDecl();

  void accept(NodeVisitor &v) const override { v.visitVarDecl(*this); }
};

struct ConstDecl : Declaration {
  std::string name;
  std::unique_ptr<Expression> value;

  ConstDecl() : Declaration(NodeKind::ConstDecl) {}
  ConstDecl(std::string n, std::unique_ptr<Expression> v)
      : Declaration(NodeKind::ConstDecl), name(std::move(n)),
        value(std::move(v)) {}

  void accept(NodeVisitor &v) const override { v.visitConstDecl(*this); }
};

struct TypeDecl : Declaration {
  std::string name;
  std::unique_ptr<TypeSpec> type;

  TypeDecl();
  TypeDecl(std::string n, std::unique_ptr<TypeSpec> t);

  ~TypeDecl();

  void accept(NodeVisitor &v) const override { v.visitTypeDecl(*this); }
};

struct ProcedureDecl : Declaration {
  std::string name;
  std::vector<std::unique_ptr<ParamDecl>> params;
  std::unique_ptr<Block> body;

  ProcedureDecl();
  ProcedureDecl(std::string n, std::vector<std::unique_ptr<ParamDecl>> p,
                std::unique_ptr<Block> b);

  ~ProcedureDecl();

  void accept(NodeVisitor &v) const override { v.visitProcedureDecl(*this); }
};

struct ParamDecl : Declaration {
  std::vector<std::string> names;
  std::unique_ptr<TypeSpec> type;

  ParamDecl();
  ParamDecl(std::vector<std::string> n, std::unique_ptr<TypeSpec> t);

  ~ParamDecl();

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

  ~FunctionDecl();

  void accept(NodeVisitor &v) const override { v.visitFunctionDecl(*this); }
};

struct CompoundStmt : Statement {
  std::vector<std::unique_ptr<Statement>> statements;

  CompoundStmt() : Statement(NodeKind::CompoundStmt) {}
  explicit CompoundStmt(std::vector<std::unique_ptr<Statement>> stmts)
      : Statement(NodeKind::CompoundStmt), statements(std::move(stmts)) {}

  void accept(NodeVisitor &v) const override { v.visitCompoundStmt(*this); }
};

struct AssignStmt : Statement {
  std::unique_ptr<Expression> target;
  std::unique_ptr<Expression> value;

  AssignStmt() : Statement(NodeKind::AssignStmt) {}
  AssignStmt(std::unique_ptr<Expression> t, std::unique_ptr<Expression> v)
      : Statement(NodeKind::AssignStmt), target(std::move(t)),
        value(std::move(v)) {}

  void accept(NodeVisitor &v) const override { v.visitAssignStmt(*this); }
};

struct ProcCall : Statement {
  std::string name;
  std::vector<std::unique_ptr<Expression>> args;

  ProcCall() : Statement(NodeKind::ProcCall) {}
  ProcCall(std::string n, std::vector<std::unique_ptr<Expression>> a)
      : Statement(NodeKind::ProcCall), name(std::move(n)), args(std::move(a)) {}

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

  void accept(NodeVisitor &v) const override { v.visitIfStmt(*this); }
};

struct WhileStmt : Statement {
  std::unique_ptr<Expression> condition;
  std::unique_ptr<Statement> body;

  WhileStmt() : Statement(NodeKind::WhileStmt) {}
  WhileStmt(std::unique_ptr<Expression> cond, std::unique_ptr<Statement> b)
      : Statement(NodeKind::WhileStmt), condition(std::move(cond)),
        body(std::move(b)) {}

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

  void accept(NodeVisitor &v) const override { v.visitRepeatStmt(*this); }
};

struct CaseStmt : Statement {
  std::unique_ptr<Expression> expr;
  std::vector<std::unique_ptr<CaseLabel>> cases;

  CaseStmt();
  CaseStmt(std::unique_ptr<Expression> e,
           std::vector<std::unique_ptr<CaseLabel>> c);

  ~CaseStmt();

  void accept(NodeVisitor &v) const override { v.visitCaseStmt(*this); }
};

struct WithStmt : Statement {
  std::unique_ptr<Expression> recordExpr;
  std::unique_ptr<Statement> body;

  WithStmt() : Statement(NodeKind::WithStmt) {}
  WithStmt(std::unique_ptr<Expression> expr, std::unique_ptr<Statement> b)
      : Statement(NodeKind::WithStmt), recordExpr(std::move(expr)),
        body(std::move(b)) {}

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

  void accept(NodeVisitor &v) const override { v.visitBinaryExpr(*this); }
};

struct UnaryExpr : Expression {
  std::unique_ptr<Expression> operand;
  std::string op;

  UnaryExpr() : Expression(NodeKind::UnaryExpr) {}
  UnaryExpr(std::string o, std::unique_ptr<Expression> oper)
      : Expression(NodeKind::UnaryExpr), operand(std::move(oper)),
        op(std::move(o)) {}

  void accept(NodeVisitor &v) const override { v.visitUnaryExpr(*this); }
};

struct LiteralExpr : Expression {
  std::string value;

  LiteralExpr() : Expression(NodeKind::LiteralExpr) {}
  explicit LiteralExpr(std::string val)
      : Expression(NodeKind::LiteralExpr), value(std::move(val)) {}

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

  void accept(NodeVisitor &v) const override { v.visitVariableExpr(*this); }
};

struct Range : ASTNode {
  int start{0};
  int end{0};

  Range() : ASTNode(NodeKind::Range) {}
  Range(int s, int e) : ASTNode(NodeKind::Range), start(s), end(e) {}

  void accept(NodeVisitor &v) const override { v.visitRange(*this); }
};

struct TypeSpec : ASTNode {
  using ASTNode::ASTNode;
};

struct SimpleTypeSpec : TypeSpec {
  BasicType basic{BasicType::Integer};
  std::string name;

  SimpleTypeSpec() : TypeSpec(NodeKind::SimpleTypeSpec) {}
  SimpleTypeSpec(BasicType b, std::string n = {})
      : TypeSpec(NodeKind::SimpleTypeSpec), basic(b), name(std::move(n)) {}

  void accept(NodeVisitor &v) const override { v.visitSimpleTypeSpec(*this); }
};

struct ArrayTypeSpec : TypeSpec {
  std::vector<Range> ranges;
  std::unique_ptr<TypeSpec> elementType;

  ArrayTypeSpec();
  ArrayTypeSpec(std::vector<Range> r, std::unique_ptr<TypeSpec> elem);

  ~ArrayTypeSpec();

  void accept(NodeVisitor &v) const override { v.visitArrayTypeSpec(*this); }
};

struct RecordTypeSpec : TypeSpec {
  std::vector<std::unique_ptr<VarDecl>> fields;

  RecordTypeSpec() : TypeSpec(NodeKind::RecordTypeSpec) {}
  explicit RecordTypeSpec(std::vector<std::unique_ptr<VarDecl>> f)
      : TypeSpec(NodeKind::RecordTypeSpec), fields(std::move(f)) {}

  void accept(NodeVisitor &v) const override { v.visitRecordTypeSpec(*this); }
};

struct PointerTypeSpec : TypeSpec {
  std::unique_ptr<TypeSpec> refType;

  PointerTypeSpec();
  explicit PointerTypeSpec(std::unique_ptr<TypeSpec> r);

  ~PointerTypeSpec();

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

  void accept(NodeVisitor &v) const override { v.visitCaseLabel(*this); }
};

struct NewExpr : Expression {
  std::unique_ptr<VariableExpr> variable;

  NewExpr() : Expression(NodeKind::NewExpr) {}
  explicit NewExpr(std::unique_ptr<VariableExpr> var)
      : Expression(NodeKind::NewExpr), variable(std::move(var)) {}

  void accept(NodeVisitor &v) const override { v.visitNewExpr(*this); }
};

struct DisposeExpr : Expression {
  std::unique_ptr<VariableExpr> variable;

  DisposeExpr() : Expression(NodeKind::DisposeExpr) {}
  explicit DisposeExpr(std::unique_ptr<VariableExpr> var)
      : Expression(NodeKind::DisposeExpr), variable(std::move(var)) {}

  void accept(NodeVisitor &v) const override { v.visitDisposeExpr(*this); }
};

struct AST {
  std::unique_ptr<Program> root{};
  bool valid{false};
};

inline Program::Program() : ASTNode(NodeKind::Program) {}
inline Program::Program(std::string n, std::unique_ptr<Block> b)
    : ASTNode(NodeKind::Program), name(std::move(n)), block(std::move(b)) {}
inline Program::~Program() = default;

inline VarDecl::VarDecl() : Declaration(NodeKind::VarDecl) {}
inline VarDecl::VarDecl(std::vector<std::string> n, std::unique_ptr<TypeSpec> t)
    : Declaration(NodeKind::VarDecl), names(std::move(n)), type(std::move(t)) {}
inline VarDecl::~VarDecl() = default;

inline TypeDecl::TypeDecl() : Declaration(NodeKind::TypeDecl) {}
inline TypeDecl::TypeDecl(std::string n, std::unique_ptr<TypeSpec> t)
    : Declaration(NodeKind::TypeDecl), name(std::move(n)), type(std::move(t)) {}
inline TypeDecl::~TypeDecl() = default;

inline ProcedureDecl::ProcedureDecl() : Declaration(NodeKind::ProcedureDecl) {}
inline ProcedureDecl::ProcedureDecl(std::string n,
                                    std::vector<std::unique_ptr<ParamDecl>> p,
                                    std::unique_ptr<Block> b)
    : Declaration(NodeKind::ProcedureDecl), name(std::move(n)),
      params(std::move(p)), body(std::move(b)) {}
inline ProcedureDecl::~ProcedureDecl() = default;

inline ParamDecl::ParamDecl() : Declaration(NodeKind::ParamDecl) {}
inline ParamDecl::ParamDecl(std::vector<std::string> n,
                            std::unique_ptr<TypeSpec> t)
    : Declaration(NodeKind::ParamDecl), names(std::move(n)),
      type(std::move(t)) {}
inline ParamDecl::~ParamDecl() = default;

inline FunctionDecl::FunctionDecl() : Declaration(NodeKind::FunctionDecl) {}
inline FunctionDecl::FunctionDecl(std::string n,
                                  std::vector<std::unique_ptr<ParamDecl>> p,
                                  std::unique_ptr<TypeSpec> r,
                                  std::unique_ptr<Block> b)
    : Declaration(NodeKind::FunctionDecl), name(std::move(n)),
      params(std::move(p)), returnType(std::move(r)), body(std::move(b)) {}
inline FunctionDecl::~FunctionDecl() = default;

inline ArrayTypeSpec::ArrayTypeSpec() : TypeSpec(NodeKind::ArrayTypeSpec) {}
inline ArrayTypeSpec::ArrayTypeSpec(std::vector<Range> r,
                                    std::unique_ptr<TypeSpec> elem)
    : TypeSpec(NodeKind::ArrayTypeSpec), ranges(std::move(r)),
      elementType(std::move(elem)) {}
inline ArrayTypeSpec::~ArrayTypeSpec() = default;

inline PointerTypeSpec::PointerTypeSpec()
    : TypeSpec(NodeKind::PointerTypeSpec) {}
inline PointerTypeSpec::PointerTypeSpec(std::unique_ptr<TypeSpec> r)
    : TypeSpec(NodeKind::PointerTypeSpec), refType(std::move(r)) {}
inline PointerTypeSpec::~PointerTypeSpec() = default;
inline CaseStmt::CaseStmt() : Statement(NodeKind::CaseStmt) {}
inline CaseStmt::CaseStmt(std::unique_ptr<Expression> e,
                          std::vector<std::unique_ptr<CaseLabel>> c)
    : Statement(NodeKind::CaseStmt), expr(std::move(e)), cases(std::move(c)) {}
inline CaseStmt::~CaseStmt() = default;

} // namespace pascal

#endif // PASCAL_COMPILER_AST_HPP
