#include "chunk.hpp"

#include <cstdlib>
#include <limits>

namespace bits
{
Chunk::Chunk() = default;

auto Chunk::AddConstant(Value value) -> int
{
    m_constants.push_back(value);

    [[likely]] if (static_cast<int>(m_constants.size()) < std::numeric_limits<int>::max())
    {
        return static_cast<int>(m_constants.size()) - 1;
    }

    // An overflow is considered catastrophic, there's no point in trying to recover.
    [[unlikely]] exit(EXIT_FAILURE);
}

} // namespace bits
