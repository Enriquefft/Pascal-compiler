#ifndef PASCAL_COMPILER_PARSER_HPP
#define PASCAL_COMPILER_PARSER_HPP

#include "parser/ast.hpp"
#include "token/token.hpp"
#include <memory>
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
  std::unique_ptr<Declaration> parseDeclaration();
  std::unique_ptr<Statement> parseStatement();
  std::unique_ptr<Expression> parseExpression();
  std::unique_ptr<TypeSpec> parseTypeSpec();
  std::unique_ptr<VariableExpr> parseVariable(std::string name);
  IdentifierList parseIdentifierList();

  std::unique_ptr<Program> parseProgram();

  std::string parseIdentifier();

  const std::vector<Token> &m_tokens;
  std::size_t m_current{0};
};

} // namespace pascal

#endif // PASCAL_COMPILER_PARSER_HPP
