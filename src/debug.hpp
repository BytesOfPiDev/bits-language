#pragma once

#include "chunk.hpp"
#include <string_view>

namespace bits
{

//! @brief Helper function that takes a chunk and prints out a human readable presentation of the entire chunk.
void DisassembleChunk(bits::Chunk &chunk, std::string_view name);
//! @brief Helper function that prints out a human readable presentation of a single instruction.
auto DisassembleInstruction(bits::Chunk &chunk, int offset) -> int;

} // namespace bits
