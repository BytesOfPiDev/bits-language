#include "compiler.hpp"

#include "common.hpp"
#include "compiler.hpp"
#include "scanner.hpp"

#include <cstdio>
#include <iomanip>
#include <iostream>

namespace bits
{
void Compile(std::string_view const source)
{
    InitScanner(source);
    int line = -1;
    while (true)
    {
        Token token = ScanToken();
        if (token.m_line != line)
        {
            std::cout << std::setprecision(4) << token.m_line;
            line = token.m_line;
        }
        else
        {
            std::cout << "   | ";
        }

        printf("%2d '%.*s'\n", static_cast<int>(token.m_type), token.m_length, token.m_start);

        if (token.m_type == TokenType::EndOfFile)
        {
            break;
        }
    }
}
} // namespace bits
