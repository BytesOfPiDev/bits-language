#include "chunk.hpp"
#include "common.hpp"
#include "debug.hpp"
#include "virtualmachine.hpp"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "sysexits.hpp"

static void Repl()
{
    constexpr auto MaxCharactersPerLine = 1024;

    std::array<char, MaxCharactersPerLine> line{};
    while (true)
    {
        std::cout << "bits-lang > ";

        if (fgets(line.data(), line.size(), stdin))
        {
            std::cout << '\n';
            break;
        }
        bits::VirtualMachine::GetVM().Interpret(line.data());
    }
}

static auto ReadFile(std::string_view path) -> std::string
{
    auto fileStream = std::ifstream{path.data(), std::fstream::in | std::fstream::binary};
    if (!fileStream)
    {
        std::cerr << "Could not open file " << path << "." << '\n';
        exit(ExitCode_IOError);
    }

    std::string buffer{std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>()};

    if (buffer.empty())
    {
        std::cerr << "Found no data in file " << path << "." << '\n';
    }

    return std::move(buffer);
}

static void RunFile(std::string_view path)
{
    std::string source = ReadFile(path);
    bits::InterpretResult result = bits::VirtualMachine::GetVM().Interpret(source.c_str());
    source.clear();

    if (result == bits::InterpretResult::InterpretCompileError)
    {
        exit(EX_DATAERR);
    }

    if (result == bits::InterpretResult::InterpretRuntimeError)
    {
        exit(EX_SOFTWARE);
    }
}

auto main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[]) -> int
{
    using namespace bits;

    if (argc == 1)
    {
        Repl();
    }
    else if (argc == 2)
    {
        RunFile(argv[1]);
    }
    else
    {
        std::cerr << "Usage: bits [path]" << std::endl;
        exit(64);
    }

    return 0;
}
