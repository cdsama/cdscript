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
        Token token = MockNumberToken(1i32);
        CHECK(token.number().value<int32_t>() == 1i32);
        token = MockNumberToken(1i32);
        CHECK_THROWS_MATCHES(token.number().value<int8_t>(), Token::NumberTypeError, WhatEquals("expected <int8_t> but real type is <int32_t>"));
        token = MockNumberToken(1i32);
        CHECK_THROWS_MATCHES(token.number().value<int16_t>(), Token::NumberTypeError, WhatEquals("expected <int16_t> but real type is <int32_t>"));
        token = MockNumberToken(1i32);
        CHECK_THROWS_MATCHES(token.number().value<int64_t>(), Token::NumberTypeError, WhatEquals("expected <int64_t> but real type is <int32_t>"));
        token = MockNumberToken(1i32);
        CHECK_THROWS_MATCHES(token.number().value<uint8_t>(), Token::NumberTypeError, WhatEquals("expected <uint8_t> but real type is <int32_t>"));
        token = MockNumberToken(1i32);
        CHECK_THROWS_MATCHES(token.number().value<uint16_t>(), Token::NumberTypeError, WhatEquals("expected <uint16_t> but real type is <int32_t>"));
        token = MockNumberToken(1i32);
        CHECK_THROWS_MATCHES(token.number().value<uint32_t>(), Token::NumberTypeError, WhatEquals("expected <uint32_t> but real type is <int32_t>"));
        token = MockNumberToken(1i32);
        CHECK_THROWS_MATCHES(token.number().value<uint64_t>(), Token::NumberTypeError, WhatEquals("expected <uint64_t> but real type is <int32_t>"));
        token = MockNumberToken(1i32);
        CHECK_THROWS_MATCHES(token.number().value<float>(), Token::NumberTypeError, WhatEquals("expected <float> but real type is <int32_t>"));
        token = MockNumberToken(1i32);
        CHECK_THROWS_MATCHES(token.number().value<double>(), Token::NumberTypeError, WhatEquals("expected <double> but real type is <int32_t>"));

        token = MockNumberToken((int8_t)1i8);
        CHECK_THROWS_MATCHES(token.number().value<int32_t>(), Token::NumberTypeError, WhatEquals("expected <int32_t> but real type is <int8_t>"));
        token = MockNumberToken(1i16);
        CHECK_THROWS_MATCHES(token.number().value<int32_t>(), Token::NumberTypeError, WhatEquals("expected <int32_t> but real type is <int16_t>"));
        token = MockNumberToken(1i64);
        CHECK_THROWS_MATCHES(token.number().value<int32_t>(), Token::NumberTypeError, WhatEquals("expected <int32_t> but real type is <int64_t>"));
        token = MockNumberToken(1ui8);
        CHECK_THROWS_MATCHES(token.number().value<int32_t>(), Token::NumberTypeError, WhatEquals("expected <int32_t> but real type is <uint8_t>"));
        token = MockNumberToken(1ui16);
        CHECK_THROWS_MATCHES(token.number().value<int32_t>(), Token::NumberTypeError, WhatEquals("expected <int32_t> but real type is <uint16_t>"));
        token = MockNumberToken(1ui32);
        CHECK_THROWS_MATCHES(token.number().value<int32_t>(), Token::NumberTypeError, WhatEquals("expected <int32_t> but real type is <uint32_t>"));
        token = MockNumberToken(1ui64);
        CHECK_THROWS_MATCHES(token.number().value<int32_t>(), Token::NumberTypeError, WhatEquals("expected <int32_t> but real type is <uint64_t>"));
        token = MockNumberToken(1.0f);
        CHECK_THROWS_MATCHES(token.number().value<int32_t>(), Token::NumberTypeError, WhatEquals("expected <int32_t> but real type is <float>"));
        token = MockNumberToken(1.0);
        CHECK_THROWS_MATCHES(token.number().value<int32_t>(), Token::NumberTypeError, WhatEquals("expected <int32_t> but real type is <double>"));
    }
}

TEST_CASE("Lexer-Number-Int", "[core][lexer][number]")
{
    {
        std::istringstream code("1");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().value<int64_t>() == 1LL);
    }
}