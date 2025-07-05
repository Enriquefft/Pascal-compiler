#ifndef PASCAL_COMPILER_PARSER_HPP
#define PASCAL_COMPILER_PARSER_HPP

#include "parser/ast.hpp"
#include "token/token.hpp"
#include <memory>
#include <optional>
#include <stack>
#include <vector>

namespace pascal {

class Parser {
public:
  explicit Parser(const std::vector<Token> &tokens);

  [[nodiscard]] AST parse();

private:
  const Token &advance();
  const Token &peek() const;
  bool match(TokenType type);
  bool isAtEnd() const;

  std::unique_ptr<Block> parseBlock();
  std::unique_ptr<Declaration> parseDeclaration(
      const std::optional<TokenType> &expectedStart = std::nullopt);
  std::unique_ptr<Statement> parseStatement();
  std::unique_ptr<Expression> parseExpression();
  std::unique_ptr<TypeSpec> parseTypeSpec();
  std::unique_ptr<VariableExpr> parseVariable(std::string name);
  IdentifierList parseIdentifierList();

  std::unique_ptr<Program> parseProgram();

  TypeDefinition parseTypeDecl();
  VarDecl parseVarDecl();

  std::string parseIdentifier();

  const std::vector<Token> &m_tokens;
  std::size_t m_current{0};

  std::stack<std::size_t> m_stack;
  // Freeze the current state of the parser
  inline void freeze() { m_stack.push(m_current); }
  // Resets the current token index to the last saved state
  inline void reset() {
    if (!m_stack.empty()) {
      m_current = m_stack.top();
      m_stack.pop();
    }
  }
  inline void pop() {
    if (!m_stack.empty()) {
      m_stack.pop();
    }
  }
};

} // namespace pascal

#endif // PASCAL_COMPILER_PARSER_HPP
