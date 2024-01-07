#pragma once

#include "chunk.hpp"
#include "value.hpp"

#include <array>
#include <string_view>

namespace bits
{

static constexpr int MaxStackSize = 256;

enum class InterpretResult
{
    InterpretOk,
    InterpretCompileError,
    InterpretRuntimeError
};

class VirtualMachine
{
  public:
    VirtualMachine();
    VirtualMachine(VirtualMachine const &) = default;
    VirtualMachine(VirtualMachine &&) = delete;
    auto operator=(VirtualMachine const &) -> VirtualMachine & = default;
    auto operator=(VirtualMachine &&) -> VirtualMachine & = delete;
    ~VirtualMachine() = default;

    auto Interpret(std::string_view source) -> InterpretResult;

    static auto GetVM() -> VirtualMachine &;
    static void ResetStack();

    void Push(Value value);
    auto Pop() -> Value;

  private:
    static auto Run() -> InterpretResult;

    // Points to the chunk of bytecode being interpreted.
    Chunk *m_chunk = nullptr;
    // Always points to the next instruction.
    OpCode *m_ip = nullptr;
    // An array of values on the VM's stack.
    std::array<Value, MaxStackSize> m_stack{};
    // Points one element past the top of the stack; e.g. the first available free element.
    Value *m_stackTop = nullptr;
};

} // namespace bits
