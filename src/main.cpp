#include "chunk.hpp"
#include "common.hpp"
#include "debug.hpp"
#include "virtualmachine.hpp"

#include <iostream>

auto main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[]) -> int
{
    using namespace bits;

    // Some manually created bytecode to verify basic functionality during implementation.
    constexpr int fakeLineNumber = 123;
    Chunk chunk{};
    size_t constant = chunk.AddConstant(1.2);
    chunk.Write(OpCode::OpConstant, fakeLineNumber);
    chunk.Write(constant, fakeLineNumber);

    constant = chunk.AddConstant(3.4);
    chunk.Write(OpCode::OpConstant, fakeLineNumber);
    chunk.Write(constant, fakeLineNumber);

    chunk.Write(OpCode::OpAdd, fakeLineNumber);

    constant = chunk.AddConstant(5.6);
    chunk.Write(OpCode::OpConstant, fakeLineNumber);
    chunk.Write(constant, fakeLineNumber);

    chunk.Write(OpCode::OpDivide, fakeLineNumber);

    chunk.Write(OpCode::OpNegate, fakeLineNumber);
    chunk.Write(OpCode::OpReturn, fakeLineNumber);

    DisassembleChunk(chunk, "test chunk");
    VirtualMachine::GetVM().Interpret(&chunk);

    return 0;
}
