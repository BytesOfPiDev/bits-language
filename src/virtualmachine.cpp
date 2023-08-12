#include "virtualmachine.hpp"
#include "chunk.hpp"
#include "common.hpp"
#include "debug.hpp"
#include <algorithm>
#include <iostream>

namespace bits
{
VirtualMachine VM; // NOLINT

template <OpCode op>
concept IsBinaryOp =
    op == OpCode::OpAdd || op == OpCode::OpSubtract || op == OpCode::OpMultiply || op == OpCode::OpDivide;

template <OpCode opCode>
constexpr void PerformBinaryOperation()
    requires(IsBinaryOp<opCode>)
{
    Value rightValue = VM.Pop();
    Value leftValue = VM.Pop();

    Value result;
    if constexpr (opCode == OpCode::OpAdd)
    {
        result = leftValue + rightValue;
    }
    else if constexpr (opCode == OpCode::OpSubtract)
    {
        result = leftValue - rightValue;
    }
    else if constexpr (opCode == OpCode::OpMultiply)
    {
        result = leftValue * rightValue;
    }
    else if constexpr (opCode == OpCode::OpDivide)
    {
        result = leftValue / rightValue;
    }

    std::cout << std::endl << "BinaryOp Result: " << result << std::endl;
    VM.Push(result);
}

VirtualMachine::VirtualMachine() : m_stackTop(m_stack.data())
{
}

auto VirtualMachine::Run() -> InterpretResult
{
    auto const readByteFunc = []() -> OpCode { return (*VM.m_ip++); }; // NOLINT
    auto readConstantFunc = [&readByteFunc]() -> Value {
        return VM.m_chunk->m_constants[static_cast<int>(readByteFunc())];
    };
    while (true)
    {
        if constexpr (DebugTraceExecution)
        {
            std::cout << "\nStack: ";
            std::for_each(VM.m_stack.begin(), VM.m_stackTop, [](Value value) {
                std::cout << "[";
                PrintValue(value);
                std::cout << "]";
            });
            std::cout << std::endl;
            DisassembleInstruction(*VM.m_chunk, static_cast<int>(VM.m_ip - &VM.m_chunk->m_byteCode.front()));
        }

        OpCode instruction;
        switch (instruction = readByteFunc())
        {
        case OpCode::OpConstant: {
            Value constant = readConstantFunc();
            VM.Push(constant);
            break;
        }
        case OpCode::OpAdd: {
            PerformBinaryOperation<OpCode::OpAdd>();
            break;
        }
        case OpCode::OpSubtract: {
            PerformBinaryOperation<OpCode::OpSubtract>();
            break;
        }
        case OpCode::OpMultiply: {
            PerformBinaryOperation<OpCode::OpMultiply>();
            break;
        }
        case OpCode::OpDivide: {
            PerformBinaryOperation<OpCode::OpDivide>();
            break;
        }
        case OpCode::OpNegate: {
            VM.Push(-VM.Pop());
            break;
        }
        case OpCode::OpReturn: {
            PrintValue(VM.Pop());
            std::cout << std::endl;
            return InterpretResult::InterpretOk;
        }
        }
    }
}

auto VirtualMachine::GetVM() -> VirtualMachine &
{
    return VM;
}

void VirtualMachine::ResetStack()
{
    VM.m_stackTop = VM.m_stack.begin();
}

auto VirtualMachine::Interpret(Chunk *chunk) -> InterpretResult
{
    std::cout << "Interpretting...\n";
    m_chunk = chunk;
    m_ip = &m_chunk->m_byteCode.front();

    return Run();
}

void VirtualMachine::Push(Value value)
{
    *m_stackTop = value;
    ++m_stackTop;
}

auto VirtualMachine::Pop() -> Value
{
    --m_stackTop;
    return *m_stackTop;
}

} // namespace bits
