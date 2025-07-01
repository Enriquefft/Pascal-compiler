#include "visitors/memory.hpp"
#include <cstdlib>

namespace pascal {

void MemoryManager::allocate(std::size_t bytes) {
  m_block = std::malloc(bytes);
}

void MemoryManager::deallocate() {
  std::free(m_block);
  m_block = nullptr;
}

} // namespace pascal
