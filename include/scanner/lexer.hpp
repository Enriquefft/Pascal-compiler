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
  std::string_view m_source;
};

} // namespace pascal

#endif // PASCAL_COMPILER_LEXER_HPP
