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

  std::unique_ptr<ASTNode> parseBlock();
  std::unique_ptr<ASTNode> parseDeclaration();
  std::unique_ptr<ASTNode> parseStatement();
  std::unique_ptr<ASTNode> parseExpression();

  AST parseProgram();

  const std::vector<Token> &m_tokens;
  std::size_t m_current{0};
};

} // namespace pascal

#endif // PASCAL_COMPILER_PARSER_HPP
