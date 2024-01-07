#pragma once

#include <string_view>
namespace bits
{
enum class TokenType
{
    // Single-character tokens.
    LeftParen,  //! (
    RightParen, //! )
    LeftBrace,  //! [
    RightBrace, //! ]
    Comma,      //! ,
    Dot,        //! .
    Minus,      //! -
    Plus,       //! +
    Semicolon,  //! ;
    Slash,      //! /
    Star,       //! *
    // One or two character tokens.
    Bang,
    BangEqual,
    Equal,
    EqualEqual,
    Greater,
    GreaterEqual,
    Less,
    LessEqual,
    // Literals
    Identifier,
    String,
    Number,
    // Keywords
    And,
    Class,
    Else,
    False,
    Fun,
    For,
    If,
    Nil,
    Or,
    Print,
    Return,
    Super,
    This,
    True,
    Var,
    While,

    Error,
    EndOfFile
};

struct Token
{
    TokenType m_type{};
    char const *m_start = nullptr;
    int m_length = 0;
    int m_line = 0;
};

void InitScanner(std::string_view const source);
auto ScanToken() -> Token;
} // namespace bits
