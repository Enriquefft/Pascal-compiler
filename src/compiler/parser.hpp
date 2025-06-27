#ifndef PASCAL_COMPILER_PARSER_HPP
#define PASCAL_COMPILER_PARSER_HPP

#include "tokens.hpp"
#include <vector>

namespace pascal {

class Parser {
public:
  explicit Parser(const std::vector<Token> &tokens);
  void parse();

private:
  const std::vector<Token> &m_tokens;
};

} // namespace pascal

#endif // PASCAL_COMPILER_PARSER_HPP
