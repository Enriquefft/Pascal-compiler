#ifndef PASCAL_COMPILER_LEXER_HPP
#define PASCAL_COMPILER_LEXER_HPP

#include "token/token.hpp"
#include <string_view>
#include <vector>

namespace pascal {

class Lexer {
public:
  explicit Lexer(std::string_view source);

  std::vector<Token> scanTokens();

private:
  void scanToken();
  void scanIdentifier();
  void scanNumber();
  void scanString();
  bool match(char expected);
  bool isAtEnd() const;
  char advance();
  char peek() const;
  char peekNext() const;
  void addToken(TokenType type, std::string_view lexeme);

  std::string_view m_source;
  std::size_t m_start{0};
  std::size_t m_current{0};
  std::size_t m_line{1};
  std::vector<Token> m_tokens;
};

} // namespace pascal

#endif // PASCAL_COMPILER_LEXER_HPP
