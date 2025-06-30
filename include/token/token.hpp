#ifndef PASCAL_COMPILER_TOKENS_HPP
#define PASCAL_COMPILER_TOKENS_HPP

#include <string>

namespace pascal {

enum class TokenType {
  // Single-character tokens
  Plus,
  Minus,
  Star,
  Slash,
  Comma,
  Semicolon,
  Colon,
  Dot,
  LeftParen,
  RightParen,
  LeftBracket,
  RightBracket,

  // One or two character tokens
  Assign,
  Equal,
  NotEqual,
  Less,
  LessEqual,
  Greater,
  GreaterEqual,
  Range,

  // Literals
  Identifier,
  Number,
  String,

  // Keywords
  Program,
  Var,
  Const,
  Type,
  Procedure,
  Function,
  Begin,
  End,
  If,
  Then,
  Else,
  While,
  Do,
  For,
  To,
  Downto,
  Repeat,
  Until,
  Case,
  Of,
  With,
  Record,
  Array,
  New,
  Dispose,
  Not,
  Div,
  Mod,
  And,
  Or,

  EndOfFile
};

struct Token {
  TokenType type{TokenType::EndOfFile};
  std::string lexeme;
  std::size_t line{0};
  std::size_t column{0};
};

} // namespace pascal

#endif // PASCAL_COMPILER_TOKENS_HPP
