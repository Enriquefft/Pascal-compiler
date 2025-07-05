#include "scanner/lexer.hpp"

#include <cctype>
#include <unordered_map>

namespace pascal {

namespace {
// Mapping of keyword strings to their corresponding token types.
const std::unordered_map<std::string_view, TokenType> KEYWORDS{
    {"program", TokenType::Program},
    {"var", TokenType::Var},
    {"const", TokenType::Const},
    {"type", TokenType::Type},
    {"procedure", TokenType::Procedure},
    {"function", TokenType::Function},
    {"begin", TokenType::Begin},
    {"end", TokenType::End},
    {"if", TokenType::If},
    {"then", TokenType::Then},
    {"else", TokenType::Else},
    {"while", TokenType::While},
    {"do", TokenType::Do},
    {"for", TokenType::For},
    {"to", TokenType::To},
    {"downto", TokenType::Downto},
    {"repeat", TokenType::Repeat},
    {"until", TokenType::Until},
    {"case", TokenType::Case},
    {"of", TokenType::Of},
    {"with", TokenType::With},
    {"record", TokenType::Record},
    {"array", TokenType::Array},
    {"new", TokenType::New},
    {"dispose", TokenType::Dispose},
    {"not", TokenType::Not},
    {"div", TokenType::Div},
    {"mod", TokenType::Mod},
    {"and", TokenType::And},
    {"or", TokenType::Or}};
} // namespace

Lexer::Lexer(std::string_view source) : m_source(source) {}

std::vector<Token> Lexer::scanTokens() {
  while (!isAtEnd()) {
    m_start = m_current;
    scanToken();
  }

  addToken(TokenType::EndOfFile, "");
  return m_tokens;
}

void Lexer::scanToken() {
  char c = advance();
  switch (c) {
  case ' ':
  case '\r':
  case '\t':
    break;
  case '\n':
    ++m_line;
    break;
  case '+':
    addToken(TokenType::Plus, "+");
    break;
  case '-':
    addToken(TokenType::Minus, "-");
    break;
  case '*':
    addToken(TokenType::Star, "*");
    break;
  case '/':
    addToken(TokenType::Slash, "/");
    break;
  case ',':
    addToken(TokenType::Comma, ",");
    break;
  case ';':
    addToken(TokenType::Semicolon, ";");
    break;
  case ':':
    if (match('=')) {
      addToken(TokenType::Assign, ":=");
    } else {

      addToken(TokenType::Colon, ":");
    }
    break;
  case '.':
    addToken(TokenType::Dot, ".");
    if (match('.'))
      addToken(TokenType::Dot, ".");
    break;
  case '(':
    addToken(TokenType::LeftParen, "(");
    break;
  case ')':
    addToken(TokenType::RightParen, ")");
    break;
  case '[':
    addToken(TokenType::LeftBracket, "[");
    break;
  case ']':
    addToken(TokenType::RightBracket, "]");
    break;
  case '^':
    addToken(TokenType::Caret, "^");
    break;
  case '=':
    addToken(TokenType::Equal, "=");
    break;
  case '>':
    if (match('='))
      addToken(TokenType::GreaterEqual, ">=");
    else
      addToken(TokenType::Greater, ">");
    break;
  case '<':
    if (match('=')) {
      addToken(TokenType::LessEqual, "<=");
    } else if (match('>')) {
      addToken(TokenType::Less, "<");
      addToken(TokenType::Greater, ">");
    } else {
      addToken(TokenType::Less, "<");
    }
    break;
  case '\'':
    addToken(TokenType::Identifier, "'");
    break;
  default:
    if (std::isdigit(static_cast<unsigned char>(c))) {
      scanNumber();
    } else if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
      scanIdentifier();
    } else {
      // Treat unknown characters as identifiers to match naive behaviour.
      addToken(TokenType::Identifier, std::string_view{&c, 1});
    }
    break;
  }
}

void Lexer::scanIdentifier() {
  while (std::isalnum(static_cast<unsigned char>(peek())) || peek() == '_')
    advance();

  std::string_view text = m_source.substr(m_start, m_current - m_start);

  auto it = KEYWORDS.find(text);
  if (it != KEYWORDS.end()) {
    addToken(it->second, text);
  } else {
    addToken(TokenType::Identifier, text);
  }
}

void Lexer::scanNumber() {
  while (std::isdigit(static_cast<unsigned char>(peek())))
    advance();

  std::string_view text = m_source.substr(m_start, m_current - m_start);
  addToken(TokenType::Number, text);
}

void Lexer::scanString() {
  while (!isAtEnd() && peek() != '\'')
    advance();

  if (!isAtEnd())
    advance(); // Closing quote

  std::string_view text = m_source.substr(m_start + 1, m_current - m_start - 2);
  addToken(TokenType::String, text);
}

bool Lexer::match(char expected) {
  if (isAtEnd())
    return false;
  if (m_source[m_current] != expected)
    return false;
  ++m_current;
  return true;
}

bool Lexer::isAtEnd() const { return m_current >= m_source.size(); }

char Lexer::advance() { return m_source[m_current++]; }

char Lexer::peek() const {
  if (isAtEnd())
    return '\0';
  return m_source[m_current];
}

char Lexer::peekNext() const {
  if (m_current + 1 >= m_source.size())
    return '\0';
  return m_source[m_current + 1];
}

void Lexer::addToken(TokenType type, std::string_view lexeme) {
  Token tok{};
  tok.type = type;
  tok.lexeme = std::string(lexeme);
  tok.line = m_line;
  tok.column = m_start;
  m_tokens.push_back(std::move(tok));
}

} // namespace pascal
