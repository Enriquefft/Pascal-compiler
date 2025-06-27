#ifndef PASCAL_COMPILER_TOKENS_HPP
#define PASCAL_COMPILER_TOKENS_HPP

#include <string>

namespace pascal {

enum class TokenType {
  Identifier,
  Number,
  String,
  EndOfFile
};

struct Token {
  TokenType type;
  std::string lexeme;
};

} // namespace pascal

#endif // PASCAL_COMPILER_TOKENS_HPP
