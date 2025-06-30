#ifndef PASCAL_COMPILER_MEMORY_HPP
#define PASCAL_COMPILER_MEMORY_HPP

#include <cstddef>

namespace pascal {

class MemoryManager {
public:
  void allocate(std::size_t bytes);
  void deallocate();

private:
  void *m_block{nullptr};
};

} // namespace pascal

#endif // PASCAL_COMPILER_MEMORY_HPP
