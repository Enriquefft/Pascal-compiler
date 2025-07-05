#ifndef PASCAL_COMPILER_TOKENS_HPP
#define PASCAL_COMPILER_TOKENS_HPP

#include <iostream>
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
  Caret,

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

// Convert a token type enum to a string representation used in the JSON
// response.  The strings mirror the enum names.
inline std::string tokenTypeToString(pascal::TokenType type) {
  using pascal::TokenType;
  switch (type) {
  case TokenType::Plus:
    return "Plus";
  case TokenType::Minus:
    return "Minus";
  case TokenType::Star:
    return "Star";
  case TokenType::Slash:
    return "Slash";
  case TokenType::Comma:
    return "Comma";
  case TokenType::Semicolon:
    return "Semicolon";
  case TokenType::Colon:
    return "Colon";
  case TokenType::Dot:
    return "Dot";
  case TokenType::LeftParen:
    return "LeftParen";
  case TokenType::RightParen:
    return "RightParen";
  case TokenType::LeftBracket:
    return "LeftBracket";
  case TokenType::RightBracket:
    return "RightBracket";
  case TokenType::Caret:
    return "Caret";
  case TokenType::Assign:
    return "Assign";
  case TokenType::Equal:
    return "Equal";
  case TokenType::NotEqual:
    return "NotEqual";
  case TokenType::Less:
    return "Less";
  case TokenType::LessEqual:
    return "LessEqual";
  case TokenType::Greater:
    return "Greater";
  case TokenType::GreaterEqual:
    return "GreaterEqual";
  case TokenType::Range:
    return "Range";
  case TokenType::Identifier:
    return "Identifier";
  case TokenType::Number:
    return "Number";
  case TokenType::String:
    return "String";
  case TokenType::Program:
    return "Program";
  case TokenType::Var:
    return "Var";
  case TokenType::Const:
    return "Const";
  case TokenType::Type:
    return "Type";
  case TokenType::Procedure:
    return "Procedure";
  case TokenType::Function:
    return "Function";
  case TokenType::Begin:
    return "Begin";
  case TokenType::End:
    return "End";
  case TokenType::If:
    return "If";
  case TokenType::Then:
    return "Then";
  case TokenType::Else:
    return "Else";
  case TokenType::While:
    return "While";
  case TokenType::Do:
    return "Do";
  case TokenType::For:
    return "For";
  case TokenType::To:
    return "To";
  case TokenType::Downto:
    return "Downto";
  case TokenType::Repeat:
    return "Repeat";
  case TokenType::Until:
    return "Until";
  case TokenType::Case:
    return "Case";
  case TokenType::Of:
    return "Of";
  case TokenType::With:
    return "With";
  case TokenType::Record:
    return "Record";
  case TokenType::Array:
    return "Array";
  case TokenType::New:
    return "New";
  case TokenType::Dispose:
    return "Dispose";
  case TokenType::Not:
    return "Not";
  case TokenType::Div:
    return "Div";
  case TokenType::Mod:
    return "Mod";
  case TokenType::And:
    return "And";
  case TokenType::Or:
    return "Or";
  case TokenType::EndOfFile:
    return "EndOfFile";
  default:
    return "Unknown";
  }
}

// printable token types
inline std::ostream &operator<<(std::ostream &os, const TokenType type) {
  return os << tokenTypeToString(type);
}

struct Token {
  TokenType type{TokenType::EndOfFile};
  std::string lexeme;
  std::size_t line{0};
  std::size_t column{0};
};

} // namespace pascal

#endif // PASCAL_COMPILER_TOKENS_HPP
