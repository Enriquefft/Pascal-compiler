#ifndef PASCAL_COMPILER_MEMORY_HPP
#define PASCAL_COMPILER_MEMORY_HPP

namespace pascal {

class MemoryManager {
public:
  void allocate();
  void deallocate();
};

} // namespace pascal

#endif // PASCAL_COMPILER_MEMORY_HPP
