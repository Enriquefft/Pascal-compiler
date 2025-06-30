#ifndef PASCAL_COMPILER_EXECUTOR_HPP
#define PASCAL_COMPILER_EXECUTOR_HPP

#include <string>
#include <string_view>

namespace pascal {

class Executor {
public:
  [[nodiscard]] std::string run(std::string_view asm_code);
};

} // namespace pascal

#endif // PASCAL_COMPILER_EXECUTOR_HPP
