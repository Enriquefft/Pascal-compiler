#include "visitors/codegen.hpp"
#include "parser/ast.hpp"

namespace pascal {

std::string CodeGenerator::generate(const AST & /*ast*/) {
  m_output.clear();
  emit("section .text");
  return m_output;
}

void CodeGenerator::emit(const std::string &text) { m_output += text; }

} // namespace pascal
