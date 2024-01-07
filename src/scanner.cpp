#include "common.hpp"

#include <cstdio>
#include <iterator>
#include <string>

#include "scanner.hpp"

namespace bits
{
struct Scanner
{
    //! Points to the beginning of the current identifier.
    char const *m_start = nullptr;
    //! Points to the current character being looked at.
    char const *m_current = nullptr;
    int m_line = 0;
};

Scanner scanner;

void InitScanner(std::string_view const source)
{
    scanner.m_start = source.begin();
    scanner.m_current = source.begin();
    scanner.m_line = 1;
}

static constexpr auto IsAlpha(char c) -> bool
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

static constexpr auto IsDigit(char c) -> bool
{
    return c >= '0' && c <= '9';
}

auto IsAtEnd() -> bool
{
    return *scanner.m_current == '\0';
}

auto Advance() -> char
{
    // Go to the next character. Remember, the current charater is the one being looked at, but has not yet been
    // consumed for usage.
    scanner.m_current++;
    // We return the previous character, which is now considered consumed
    return scanner.m_current[-1];
}

static auto Peek() -> char
{
    return *scanner.m_current;
}

auto PeekNext() -> char
{
    if (IsAtEnd())
    {
        return '\0';
    }

    return scanner.m_current[1];
}

auto Match(char expected) -> bool
{
    if (IsAtEnd())
    {
        return false;
    }

    if (*scanner.m_current != expected)
    {
        return false;
    }

    // The current character is considered consumed, so we go to the next character being looked at.
    scanner.m_current++;
    return true;
}

auto MakeToken(TokenType tokenType) -> Token
{
    Token token;
    token.m_type = tokenType;
    token.m_start = scanner.m_start;
    token.m_length = static_cast<int>(scanner.m_current - scanner.m_start);
    token.m_line = scanner.m_line;

    return token;
}

auto ErrorToken(std::string_view errorMessage) -> Token
{
    Token token;
    token.m_type = TokenType::Error;
    //! @todo Need to check if it returns the same address as using a const char*
    token.m_start = errorMessage.data();
    token.m_length = static_cast<int>(errorMessage.length());
    token.m_line = scanner.m_line;

    return token;
}

void SkipWhitespace()
{
    while (true)
    {
        char c = Peek();
        switch (c)
        {
        case ' ':
            [[fallthrough]];
        case '\r':
            [[fallthrough]];
        case '\t':
            Advance();
            break;
        case '\n':
            // It's a newline character, so increment line number.
            scanner.m_line++;
            Advance();
            break;
        case '/':
            if (PeekNext() == '/')
            {
                // A comment goes until the end of the line.
                while (Peek() != '\n' && !IsAtEnd())
                {
                    Advance();
                }
            }
            else
            {
                return;
            }
            break;
        default:
            return;
        }
    }
}

static TokenType CheckKeyword(int start, int length, std::string_view test, TokenType type)
{

    return TokenType::Identifier;
}

static auto IdentifierType() -> TokenType
{
    switch (scanner.m_start[0])
    {
    case 'a':
        return CheckKeyword(1, 2, "nd", TokenType::And);
    case 'c':
        return CheckKeyword(1, 4, "lass", TokenType::Class);
    case 'e':
        return CheckKeyword(1, 3, "lse", TokenType::Else);
    case 'f':
        if (scanner.m_current - scanner.m_start > 1)
        {
            switch (scanner.m_start[1])
            {
            case 'a':
                return CheckKeyword(2, 3, "lse", TokenType::False);
            case 'o':
                return CheckKeyword(2, 1, "r", TokenType::For);
            case 'u':
                return CheckKeyword(2, 1, "n", TokenType::Fun);
            }
        }
        break;
    case 'i':
        return CheckKeyword(1, 1, "f", TokenType::If);
    case 'n':
        return CheckKeyword(1, 2, "il", TokenType::Nil);
    case 'o':
        return CheckKeyword(1, 1, "r", TokenType::Or);
    case 'p':
        return CheckKeyword(1, 4, "rint", TokenType::Print);
    case 'r':
        return CheckKeyword(1, 5, "eturn", TokenType::Return);
    case 's':
        return CheckKeyword(1, 4, "uper", TokenType::Super);
    case 't':
        if (scanner.m_current - scanner.m_start > 1)
        {
            switch (scanner.m_start[1])
            {
            case 'h':
                return CheckKeyword(2, 2, "is", TokenType::This);
            case 'r':
                return CheckKeyword(2, 2, "ue", TokenType::True);
            }
        }
        break;
    case 'v':
        return CheckKeyword(1, 2, "ar", TokenType::Var);
    case 'w':
        return CheckKeyword(1, 4, "hile", TokenType::While);
    }

    return TokenType::Identifier;
}

static auto Identifier() -> Token
{
    while (IsAlpha(Peek()) || IsDigit(Peek()))
    {
        Advance();
    }

    return MakeToken(IdentifierType());
}

static auto Number() -> Token
{
    while (IsDigit(Peek()))
    {
        Advance();
    }

    // Look for a fractional part.
    if (Peek() == '.' && IsDigit(PeekNext()))
    {
        // Consume the "." (period)
        Advance();

        while (IsDigit(Peek()))
        {
            Advance();
        }
    }

    return MakeToken(TokenType::Number);
}

static auto string() -> Token
{
    while (Peek() != '"' && !IsAtEnd())
    {
        if (Peek() == '\n')
        {
            scanner.m_line++;
        }

        Advance();
    }

    if (IsAtEnd())
    {
        return ErrorToken("Unterminated string.");
    }

    Advance();
    return MakeToken(TokenType::String);
}

auto ScanToken() -> Token
{
    SkipWhitespace();
    // Each call scans a complete token, guaranteeing that we will always be at the start of a new token upon entry.
    scanner.m_start = scanner.m_current;
    if (IsAtEnd())
    {
        return MakeToken(TokenType::EndOfFile);
    }

    char c = Advance();
    if (IsAlpha(c))
    {
        return Identifier();
    }

    if (IsDigit(c))
    {
        return Number();
    }

    switch (c)
    {
    case '(':
        return MakeToken(TokenType::LeftParen);
    case ')':
        return MakeToken(TokenType::RightParen);
    case '{':
        return MakeToken(TokenType::LeftBrace);
    case '}':
        return MakeToken(TokenType::RightBrace);
    case ';':
        return MakeToken(TokenType::Semicolon);
    case ',':
        return MakeToken(TokenType::Comma);
    case '.':
        return MakeToken(TokenType::Dot);
    case '-':
        return MakeToken(TokenType::Minus);
    case '+':
        return MakeToken(TokenType::Plus);
    case '/':
        return MakeToken(TokenType::Slash);
    case '*':
        return MakeToken(TokenType::Star);
    case '!':
        return MakeToken(Match('=') ? TokenType::BangEqual : TokenType::Bang);
    case '=':
        return MakeToken(Match('=') ? TokenType::EqualEqual : TokenType::Equal);
    case '<':
        return MakeToken(Match('=') ? TokenType::LessEqual : TokenType::Less);
    case '>':
        return MakeToken(Match('=') ? TokenType::GreaterEqual : TokenType::Greater);
    case '"':
        return string();
    }

    // If we've made it this far, then we've encountered something we don't know how to handle.
    return ErrorToken("Unexpected character.");
}
} // namespace bits
