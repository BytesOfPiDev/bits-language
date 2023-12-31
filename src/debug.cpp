#include "debug.hpp"

#include "chunk.hpp"
#include "value.hpp"

#include <format>
#include <iostream>

namespace bits
{

constexpr auto DefaultWidth = 16;

void DisassembleChunk(bits::Chunk &chunk, std::string_view name)
{
    std::cout << "== " << name << " ==" << '\n';

    for (int offset = 0; offset < chunk.m_byteCode.size();)
    {
        offset = DisassembleInstruction(chunk, offset);
    };
}

static auto ConstantInstruction(std::string_view name, bits::Chunk &chunk, int offset) -> int
{
    auto constant = static_cast<uint8_t>(chunk.m_byteCode[offset + 1]);
    std::cout << std::format("{:15} [{}] ", name.data(), constant);
    PrintValue(chunk.m_constants[constant]);
    return offset + 2;
}

static auto SimpleInstruction(std::string_view name, int offset)
{
    std::cout << name << '\n';
    return ++offset;
}

auto DisassembleInstruction(bits::Chunk &chunk, int offset) -> int
{
    std::cout << '\n' << std::format("{:0>4d} ", offset);

    if (offset > 0 && chunk.m_lines[offset - 1])
    {
        std::cout << "   | ";
    }
    else
    {
        std::cout << std::format("{:0>4d} ", chunk.m_lines[offset]);
    }

    OpCode const instruction = chunk.m_byteCode[offset];

    switch (instruction)
    {
    case OpCode::OpConstant:
        return ConstantInstruction("OP_CONSTANT", chunk, offset);
    case OpCode::OpAdd:
        return SimpleInstruction("OP_ADD", offset);
    case OpCode::OpSubtract:
        return SimpleInstruction("OP_SUBTRACT", offset);
    case OpCode::OpMultiply:
        return SimpleInstruction("OP_MULTIPLY", offset);
    case OpCode::OpDivide:
        return SimpleInstruction("OP_DIVIDE", offset);
    case OpCode::OpNegate:
        return SimpleInstruction("OP_NEGATE", offset);
    case OpCode::OpReturn:
        return SimpleInstruction("OP_RETURN", offset);
    default:
        std::cout << "Unknown opcode " << static_cast<uint8_t>(instruction) << '\n';
        return ++offset;
    }
}

} // namespace bits
