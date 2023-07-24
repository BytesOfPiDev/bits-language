#include "chunk.hpp"
#include "common.hpp"
#include "debug.hpp"

#include <iostream>

auto main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[]) -> int
{
    using namespace bits;

    // Some manually created bytecode to verify basic functionality during implementation.
    Chunk chunk{};
    size_t constant = chunk.AddConstant(1.2);
    chunk.Write(bits::OpCode::OpConstant, 123);
    chunk.Write(constant, 123);
    chunk.Write(bits::OpCode::OpReturn, 123);

    DisassembleChunk(chunk, "test chunk");

    std::cout << "Hai!\n";

    return 0;
}
