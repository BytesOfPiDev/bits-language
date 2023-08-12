#pragma once

#include "common.hpp"
#include "value.hpp"

#include <cstdint>
#include <vector>

namespace bits
{

//! @brief The supported bytecode instructions.
enum class OpCode : uint8_t
{
    OpConstant,
    OpAdd,
    OpSubtract,
    OpMultiply,
    OpDivide,
    OpNegate,
    OpReturn,
};

//! Stores a series of btyecode instructions.
//! @note Not every element is an instruction (@see OpCode), some are data/operands.
using ByteCodeContainer = std::vector<OpCode>;

struct Chunk
{
    Chunk();
    Chunk(const Chunk &) = default;
    Chunk(Chunk &&) = default;
    auto operator=(const Chunk &) -> Chunk & = default;
    auto operator=(Chunk &&) -> Chunk & = default;
    ~Chunk() = default;

    //! @brief Write a new instruction to the chunk.
    //! @parameter opCode The instruction to write.
    //! @line The source code line associated with this instruction
    inline void Write(OpCode opCode, int line)
    {
        m_byteCode.push_back(opCode);
        m_lines.push_back(line);
    }

    //! @brief Write a byte to the chunk.
    //! @parameter opCode The byte to write.
    //! @line The source code line associated with this instruction
    inline void Write(uint8_t byte, int line)
    {
        Write(static_cast<OpCode>(byte), line);
    }

    //! @brief Add a new constant to the chunk.
    //! @parameter value The value of the constant.
    //! @return The index of the new constant.
    auto AddConstant(Value value) -> int;

    ByteCodeContainer m_byteCode{};
    ValueContainer m_constants{};
    std::vector<int> m_lines{};
};

} // namespace bits
