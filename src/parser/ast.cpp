#include "parser/ast.hpp"

namespace pascal {

NodeVisitor::~NodeVisitor() = default;

ASTNode::~ASTNode() = default;

Program::~Program() = default;
VarDecl::~VarDecl() = default;
TypeDecl::~TypeDecl() = default;
ProcedureDecl::~ProcedureDecl() = default;
ParamDecl::~ParamDecl() = default;
FunctionDecl::~FunctionDecl() = default;
CaseStmt::~CaseStmt() = default;
ArrayTypeSpec::~ArrayTypeSpec() = default;
PointerTypeSpec::~PointerTypeSpec() = default;
IdentifierList ::~IdentifierList() = default;

Expression::~Expression() = default;
Statement::~Statement() = default;
Declaration::~Declaration() = default;
Block::~Block() = default;
ConstDecl::~ConstDecl() = default;
CompoundStmt::~CompoundStmt() = default;
AssignStmt::~AssignStmt() = default;
ProcCall::~ProcCall() = default;
IfStmt::~IfStmt() = default;
WhileStmt::~WhileStmt() = default;
ForStmt::~ForStmt() = default;
RepeatStmt::~RepeatStmt() = default;
WithStmt::~WithStmt() = default;
BinaryExpr::~BinaryExpr() = default;
UnaryExpr::~UnaryExpr() = default;
LiteralExpr::~LiteralExpr() = default;
VariableExpr::~VariableExpr() = default;
Range::~Range() = default;
TypeSpec::~TypeSpec() = default;
SimpleTypeSpec::~SimpleTypeSpec() = default;
RecordTypeSpec::~RecordTypeSpec() = default;
CaseLabel::~CaseLabel() = default;
NewExpr::~NewExpr() = default;
DisposeExpr::~DisposeExpr() = default;

} // namespace pascal
