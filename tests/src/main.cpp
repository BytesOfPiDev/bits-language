#include <iostream>

#include "gtest/gtest.h"

#include "chunk.hpp"

TEST(ChunkTest, WriteConstant_WrittenWithCorrectValues)
{
    bits::Chunk chunk{};
    constexpr int testLineNumber = 1337;
    chunk.Write(bits::OpCode::OpConstant, testLineNumber);
    EXPECT_EQ(chunk.m_byteCode.size(), 1);
    EXPECT_EQ(chunk.m_constants.size(), 0); // We didn't write a constant, just the instruction
    EXPECT_EQ(chunk.m_lines.size(), 1);
    EXPECT_EQ(chunk.m_lines[0], testLineNumber);
    EXPECT_EQ(chunk.m_byteCode[0], bits::OpCode::OpConstant);
}

auto main(int argc, char **argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
