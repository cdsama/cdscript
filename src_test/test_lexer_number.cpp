#include "catch2_ext.hpp"
#include "lexer.hpp"
using namespace cdscript;

template <typename T>
Token MockNumberToken(T t)
{
    Token token;
    token.type = Token::Number;
    token.line = 0;
    token.column = 0;
    Token::NumberValue value;
    value.number = t;
    value.type = Token::NumberType<T>::value;
    token.value = value;
    return token;
}

TEST_CASE("Lexer-Number-TokenType", "[core][lexer][number]")
{
    {
        Token token = MockNumberToken((int32_t)1);
        CHECK(token.number().as<int32_t>() == (int32_t)1);
        token = MockNumberToken((int32_t)1);
        CHECK_THROWS_MATCHES(token.number().as<int8_t>(), Token::NumberTypeError, WhatEquals("expected <int8_t> but real type is <int32_t>"));
        token = MockNumberToken((int32_t)1);
        CHECK_THROWS_MATCHES(token.number().as<int16_t>(), Token::NumberTypeError, WhatEquals("expected <int16_t> but real type is <int32_t>"));
        token = MockNumberToken((int32_t)1);
        CHECK_THROWS_MATCHES(token.number().as<int64_t>(), Token::NumberTypeError, WhatEquals("expected <int64_t> but real type is <int32_t>"));
        token = MockNumberToken((int32_t)1);
        CHECK_THROWS_MATCHES(token.number().as<uint8_t>(), Token::NumberTypeError, WhatEquals("expected <uint8_t> but real type is <int32_t>"));
        token = MockNumberToken((int32_t)1);
        CHECK_THROWS_MATCHES(token.number().as<uint16_t>(), Token::NumberTypeError, WhatEquals("expected <uint16_t> but real type is <int32_t>"));
        token = MockNumberToken((int32_t)1);
        CHECK_THROWS_MATCHES(token.number().as<uint32_t>(), Token::NumberTypeError, WhatEquals("expected <uint32_t> but real type is <int32_t>"));
        token = MockNumberToken((int32_t)1);
        CHECK_THROWS_MATCHES(token.number().as<uint64_t>(), Token::NumberTypeError, WhatEquals("expected <uint64_t> but real type is <int32_t>"));
        token = MockNumberToken((int32_t)1);
        CHECK_THROWS_MATCHES(token.number().as<float>(), Token::NumberTypeError, WhatEquals("expected <float> but real type is <int32_t>"));
        token = MockNumberToken((int32_t)1);
        CHECK_THROWS_MATCHES(token.number().as<double>(), Token::NumberTypeError, WhatEquals("expected <double> but real type is <int32_t>"));

        token = MockNumberToken((int8_t)1);
        CHECK_THROWS_MATCHES(token.number().as<int32_t>(), Token::NumberTypeError, WhatEquals("expected <int32_t> but real type is <int8_t>"));
        token = MockNumberToken((int16_t)1);
        CHECK_THROWS_MATCHES(token.number().as<int32_t>(), Token::NumberTypeError, WhatEquals("expected <int32_t> but real type is <int16_t>"));
        token = MockNumberToken((int64_t)1);
        CHECK_THROWS_MATCHES(token.number().as<int32_t>(), Token::NumberTypeError, WhatEquals("expected <int32_t> but real type is <int64_t>"));
        token = MockNumberToken((uint8_t)1);
        CHECK_THROWS_MATCHES(token.number().as<int32_t>(), Token::NumberTypeError, WhatEquals("expected <int32_t> but real type is <uint8_t>"));
        token = MockNumberToken((uint16_t)1);
        CHECK_THROWS_MATCHES(token.number().as<int32_t>(), Token::NumberTypeError, WhatEquals("expected <int32_t> but real type is <uint16_t>"));
        token = MockNumberToken((uint32_t)1);
        CHECK_THROWS_MATCHES(token.number().as<int32_t>(), Token::NumberTypeError, WhatEquals("expected <int32_t> but real type is <uint32_t>"));
        token = MockNumberToken((uint64_t)1);
        CHECK_THROWS_MATCHES(token.number().as<int32_t>(), Token::NumberTypeError, WhatEquals("expected <int32_t> but real type is <uint64_t>"));
        token = MockNumberToken(1.0f);
        CHECK_THROWS_MATCHES(token.number().as<int32_t>(), Token::NumberTypeError, WhatEquals("expected <int32_t> but real type is <float>"));
        token = MockNumberToken(1.0);
        CHECK_THROWS_MATCHES(token.number().as<int32_t>(), Token::NumberTypeError, WhatEquals("expected <int32_t> but real type is <double>"));
    }
}

TEST_CASE("Lexer-Number-Bin", "[core][lexer][number]")
{
    {
        std::istringstream code("0b0 0b1 0b00011 0B1001 0B111111111111111111111111111111111 0b1111111111111111111111111111111111111111111111111111111111111111");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().as<int32_t>() == 0);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().as<int32_t>() == 1);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().as<int32_t>() == 3);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().as<int32_t>() == 9);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().as<int64_t>() == 8589934591);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().as<int64_t>() == INT64_MAX);
    }
    {
        std::istringstream code("0B1001i8 0B1001i16 0B1001i32 0B1001i64 0B1001u8 0B1001u16 0B1001u32 0B1001u64 ");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().as<int8_t>() == 9);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().as<int16_t>() == 9);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().as<int32_t>() == 9);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().as<int64_t>() == 9);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().as<uint8_t>() == 9);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().as<uint16_t>() == 9);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().as<uint32_t>() == 9);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().as<uint64_t>() == 9);
    }
}

TEST_CASE("Lexer-Number-Bin-Exceptions", "[core][lexer][number]")
{
    {
        std::istringstream code("0b");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Lexer::ParseError, WhatEquals("expect digit after binary number literal prefix at line:1 column:2"));
    }
    {
        std::istringstream code("0bi8");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Lexer::ParseError, WhatEquals("expect digit after binary number literal prefix at line:1 column:3"));
    }
    {
        std::istringstream code("0b2");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Lexer::ParseError, WhatEquals("unexpected digit '2' in binary number literal at line:1 column:3"));
    }
    {
        std::istringstream code("0b1x");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Lexer::ParseError, WhatEquals("unexpected character 'x' after binary number literal at line:1 column:4"));
    }
    {
        std::istringstream code("0b1001.101");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Lexer::ParseError, WhatEquals("unexpected '.' in binary number literal at line:1 column:7"));
    }
    {
        std::istringstream code("0b100111011i8");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Lexer::ParseError, WhatEquals("binary number literal is out of range at line:1 column:13"));
    }
    {
        std::istringstream code("0b11111111111111111111111111111111111111111111111111111111111111111");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Lexer::ParseError, WhatEquals("binary number literal is out of range at line:1 column:67"));
    }
    {
        std::istringstream code("0b100111011i88");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Lexer::ParseError, WhatEquals("unexpected postfix character '8' after binary number literal at line:1 column:14"));
    }
    {
        std::istringstream code("0b10011101_i8");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Lexer::ParseError, WhatEquals("unexpected character '_' after binary number literal at line:1 column:11"));
    }
    {
        std::istringstream code("0b100111011i65");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Lexer::ParseError, WhatEquals("unexpected postfix bit after binary number literal at line:1 column:14"));
    }
    {
        std::istringstream code("0b100111011i64_");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Lexer::ParseError, WhatEquals("unexpected postfix character '_' after binary number literal at line:1 column:15"));
    }
}

TEST_CASE("Lexer-Number-Int", "[core][lexer][number]")
{
    {
        std::istringstream code("1");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().as<int32_t>() == 1);
    }
}