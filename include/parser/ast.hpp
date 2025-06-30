#ifndef PASCAL_COMPILER_AST_HPP
#define PASCAL_COMPILER_AST_HPP

#include <memory>
#include <string>
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
  VariableExpr
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
  void accept(NodeVisitor &v) const override { v.visitProgram(*this); }
};

struct Block : ASTNode {
  std::vector<std::unique_ptr<Declaration>> declarations;
  std::vector<std::unique_ptr<Statement>> statements;
  void accept(NodeVisitor &v) const override { v.visitBlock(*this); }
};

struct VarDecl : Declaration {
  std::string name;
  std::string type;
  void accept(NodeVisitor &v) const override { v.visitVarDecl(*this); }
};

struct ConstDecl : Declaration {
  std::string name;
  std::unique_ptr<Expression> value;
  void accept(NodeVisitor &v) const override { v.visitConstDecl(*this); }
};

struct TypeDecl : Declaration {
  std::string name;
  void accept(NodeVisitor &v) const override { v.visitTypeDecl(*this); }
};

struct ProcedureDecl : Declaration {
  std::string name;
  std::vector<std::unique_ptr<Declaration>> params;
  std::unique_ptr<Block> body;
  void accept(NodeVisitor &v) const override { v.visitProcedureDecl(*this); }
};

struct ParamDecl : Declaration {
  std::string name;
  std::string type;
  void accept(NodeVisitor &v) const override { v.visitParamDecl(*this); }
};

struct FunctionDecl : Declaration {
  std::string name;
  std::vector<std::unique_ptr<Declaration>> params;
  std::string returnType;
  std::unique_ptr<Block> body;
  void accept(NodeVisitor &v) const override { v.visitFunctionDecl(*this); }
};

struct CompoundStmt : Statement {
  std::vector<std::unique_ptr<Statement>> statements;
  void accept(NodeVisitor &v) const override { v.visitCompoundStmt(*this); }
};

struct AssignStmt : Statement {
  std::unique_ptr<Expression> target;
  std::unique_ptr<Expression> value;
  void accept(NodeVisitor &v) const override { v.visitAssignStmt(*this); }
};

struct ProcCall : Statement {
  std::string name;
  std::vector<std::unique_ptr<Expression>> args;
  void accept(NodeVisitor &v) const override { v.visitProcCall(*this); }
};

struct IfStmt : Statement {
  std::unique_ptr<Expression> condition;
  std::unique_ptr<Statement> thenBranch;
  std::unique_ptr<Statement> elseBranch;
  void accept(NodeVisitor &v) const override { v.visitIfStmt(*this); }
};

struct WhileStmt : Statement {
  std::unique_ptr<Expression> condition;
  std::unique_ptr<Statement> body;
  void accept(NodeVisitor &v) const override { v.visitWhileStmt(*this); }
};

struct ForStmt : Statement {
  std::unique_ptr<Statement> init;
  std::unique_ptr<Expression> limit;
  std::unique_ptr<Statement> body;
  void accept(NodeVisitor &v) const override { v.visitForStmt(*this); }
};

struct RepeatStmt : Statement {
  std::vector<std::unique_ptr<Statement>> body;
  std::unique_ptr<Expression> condition;
  void accept(NodeVisitor &v) const override { v.visitRepeatStmt(*this); }
};

struct CaseStmt : Statement {
  std::unique_ptr<Expression> expr;
  std::vector<std::unique_ptr<Statement>> cases;
  void accept(NodeVisitor &v) const override { v.visitCaseStmt(*this); }
};

struct WithStmt : Statement {
  std::unique_ptr<Expression> recordExpr;
  std::unique_ptr<Statement> body;
  void accept(NodeVisitor &v) const override { v.visitWithStmt(*this); }
};

struct BinaryExpr : Expression {
  std::unique_ptr<Expression> left;
  std::unique_ptr<Expression> right;
  std::string op;
  void accept(NodeVisitor &v) const override { v.visitBinaryExpr(*this); }
};

struct UnaryExpr : Expression {
  std::unique_ptr<Expression> operand;
  std::string op;
  void accept(NodeVisitor &v) const override { v.visitUnaryExpr(*this); }
};

struct LiteralExpr : Expression {
  std::string value;
  void accept(NodeVisitor &v) const override { v.visitLiteralExpr(*this); }
};

struct VariableExpr : Expression {
  std::string name;
  void accept(NodeVisitor &v) const override { v.visitVariableExpr(*this); }
};

struct AST {
  std::unique_ptr<Program> root{};
  bool valid{false};
};

} // namespace pascal

#endif // PASCAL_COMPILER_AST_HPP
