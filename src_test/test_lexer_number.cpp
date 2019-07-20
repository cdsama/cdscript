// Copyright (c) 2019 chendi
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "catch2_ext.hpp"
#include "lexer.hpp"
using namespace cd;
using namespace script;

TEST_CASE("Lexer-Number-Bin", "[core][lexer][number]")
{
    {
        std::istringstream code("0b0 0b1 0b00011 0B1001 0B111111111111111111111111111111111 0b1111111111111111111111111111111111111111111111111111111111111111");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int32_t>() == 0);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int32_t>() == 1);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int32_t>() == 3);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int32_t>() == 9);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int64_t>() == 8589934591);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int64_t>() == std::numeric_limits<int64_t>::max());
    }
}

TEST_CASE("Lexer-Number-Bin-Postfix", "[core][lexer][number]")
{
    {
        std::istringstream code("0B1001i8 0B1001i16 0B1001i32 0B1001i64 0B1001u8 0B1001u16 0B1001u32 0B1001u64 0b1111111111111111111111111111111111111111111111111111111111111111u64");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int8_t>() == 9);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int16_t>() == 9);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int32_t>() == 9);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int64_t>() == 9);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<uint8_t>() == 9);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<uint16_t>() == 9);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<uint32_t>() == 9);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<uint64_t>() == 9);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<uint64_t>() == std::numeric_limits<uint64_t>::max());
    }
}

TEST_CASE("Lexer-Number-Bin-Exceptions", "[core][lexer][number]")
{
    {
        std::istringstream code("0b");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("expect digit after binary number literal prefix at line:1 column:2"));
    }
    {
        std::istringstream code("0bi8");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("expect digit after binary number literal prefix at line:1 column:3"));
    }
    {
        std::istringstream code("0b2");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("unexpected digit '2' in binary number literal at line:1 column:3"));
    }
    {
        std::istringstream code("0b1x");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("unexpected character 'x' after binary number literal at line:1 column:4"));
    }
    {
        std::istringstream code("0b1001.101");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("unexpected '.' in binary number literal at line:1 column:7"));
    }
    {
        std::istringstream code("0b100111011i8");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("binary number literal is out of range at line:1 column:13"));
    }
    {
        std::istringstream code("0b11111111111111111111111111111111111111111111111111111111111111111");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("binary number literal is out of range at line:1 column:67"));
    }
    {
        std::istringstream code("0b100111011i88");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("unexpected postfix character '8' after number literal at line:1 column:14"));
    }
    {
        std::istringstream code("0b10011101_i8");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("unexpected character '_' after binary number literal at line:1 column:11"));
    }
    {
        std::istringstream code("0b100111011i65");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("unexpected postfix bit after number literal at line:1 column:14"));
    }
    {
        std::istringstream code("0b100111011i64_");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("unexpected postfix character '_' after number literal at line:1 column:15"));
    }
}

TEST_CASE("Lexer-Number-Oct", "[core][lexer][number]")
{
    {
        std::istringstream code("000 001 0777 077777 0777777777 077777777777 0777777777777777777777 01777777777777777777777");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int32_t>() == 0);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int32_t>() == 1);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int32_t>() == 511);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int32_t>() == 32767);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int32_t>() == 134217727);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int64_t>() == 8589934591);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int64_t>() == std::numeric_limits<int64_t>::max());
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<uint64_t>() == std::numeric_limits<uint64_t>::max());
    }
}
TEST_CASE("Lexer-Number-Oct-Postfix", "[core][lexer][number]")
{
    {
        std::istringstream code("011i8 011i16 011i32 011i64 011u8 011u16 011u32 011u64 01777777777777777777777u64");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int8_t>() == 9);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int16_t>() == 9);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int32_t>() == 9);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int64_t>() == 9);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<uint8_t>() == 9);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<uint16_t>() == 9);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<uint32_t>() == 9);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<uint64_t>() == 9);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<uint64_t>() == std::numeric_limits<uint64_t>::max());
    }
}

TEST_CASE("Lexer-Number-Oct-Exceptions", "[core][lexer][number]")
{
    {
        std::istringstream code("09");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("unexpected digit '9' in octal number literal at line:1 column:2"));
    }
    {
        std::istringstream code("03x");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("unexpected character 'x' after octal number literal at line:1 column:3"));
    }
    {
        std::istringstream code("02.3");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("unexpected '.' in octal number literal at line:1 column:3"));
    }
    {
        std::istringstream code("077777i8");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("octal number literal is out of range at line:1 column:8"));
    }
    {
        std::istringstream code("0777777777777777777777777777777777777777777");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("octal number literal is out of range at line:1 column:43"));
    }
    {
        std::istringstream code("0777777777777777777777777777777777777777777i64");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("octal number literal is out of range at line:1 column:46"));
    }
    {
        std::istringstream code("0777777777777777777777777777777777777777777u64");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("octal number literal is out of range at line:1 column:46"));
    }
}

TEST_CASE("Lexer-Number-Dec", "[core][lexer][number]")
{
    {
        std::istringstream code("0 1 55555 77777777 2147483647 9223372036854775807");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int32_t>() == 0);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int32_t>() == 1);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int32_t>() == 55555);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int32_t>() == 77777777);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int32_t>() == std::numeric_limits<int32_t>::max());
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int64_t>() == std::numeric_limits<int64_t>::max());
    }
}

TEST_CASE("Lexer-Number-Dec-Postfix", "[core][lexer][number]")
{
    {
        std::istringstream code("0i8 1u8 22222i16 44444u16 77777777i32 2147483647i32 4294967295u32 9223372036854775807i64");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int8_t>() == 0);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<uint8_t>() == 1);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int16_t>() == 22222);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<uint16_t>() == 44444);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int32_t>() == 77777777);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int32_t>() == std::numeric_limits<int32_t>::max());
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<uint32_t>() == std::numeric_limits<uint32_t>::max());
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int64_t>() == std::numeric_limits<int64_t>::max());
    }
}

TEST_CASE("Lexer-Number-Hex", "[core][lexer][number]")
{
    {
        std::istringstream code("0x0 0x1 0xff 0xffff 0xefffffff 0x7fffffffffffffff 0xffffffffffffffff");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int32_t>() == 0);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int32_t>() == 1);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int32_t>() == 0xff);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int32_t>() == 0xffff);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int64_t>() == 0xefffffff);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int64_t>() == 0x7fffffffffffffff);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<uint64_t>() == 0xffffffffffffffff);
    }
}

TEST_CASE("Lexer-Number-Hex-Postfix", "[core][lexer][number]")
{
    {
        std::istringstream code("0xffu8 0xffffu16 0xffffffffu32 0xffffffffffffffffu64 0x7fi8 0x7fffi16 0x7fffffffi32 0x7fffffffffffffffi64");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<uint8_t>() == std::numeric_limits<uint8_t>::max());
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<uint16_t>() == std::numeric_limits<uint16_t>::max());
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<uint32_t>() == std::numeric_limits<uint32_t>::max());
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<uint64_t>() == std::numeric_limits<uint64_t>::max());
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int8_t>() == std::numeric_limits<int8_t>::max());
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int16_t>() == std::numeric_limits<int16_t>::max());
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int32_t>() == std::numeric_limits<int32_t>::max());
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<int64_t>() == std::numeric_limits<int64_t>::max());
    }
}

TEST_CASE("Lexer-Number-Float", "[core][lexer][number]")
{
    {
        std::istringstream code("0.1 1. 1.f 1.0 5.3 3.402823466e+38F 1.175494351e-38F 1.7976931348623158e+308 2.2250738585072014e-308 0x0.1 0x0.1p2");
        auto lexer = Lexer::GetLexer(code);
        auto token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<double>() == 0.1);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<double>() == 1.);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<float>() == 1.f);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<double>() == 1.0);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<double>() == 5.3);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<float>() == std::numeric_limits<float>::max());
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<float>() == std::numeric_limits<float>::min());
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<double>() == std::numeric_limits<double>::max());
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<double>() == std::numeric_limits<double>::min());
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<double>() == 1.0 / 16.0);
        token = lexer->GetToken();
        CHECK(token.type == Token::Number);
        CHECK(token.number().get<double>() == 1.0 / 16.0 * 2.0 * 2.0);
    }
}

TEST_CASE("Lexer-Number-Dec-Hex-Exceptions", "[core][lexer][number]")
{
    {
        std::istringstream code("0..9");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("multiple '.' in number literal at line:1 column:3"));
    }
    {
        std::istringstream code("0x0..9");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("multiple '.' in number literal at line:1 column:5"));
    }
    {
        std::istringstream code("0.1ex");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("expect exponent digit at line:1 column:5"));
    }
    {
        std::istringstream code("0x0.1px");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("expect exponent digit at line:1 column:7"));
    }
    {
        std::istringstream code("0x0.1p2fx");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("unexpected 'x' after number literal at line:1 column:9"));
    }
    {
        std::istringstream code("1f");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("unexpected 'f' after integer literal at line:1 column:2"));
    }
    {
        std::istringstream code("1.0i32");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("unexpected 'i' after float literal at line:1 column:4"));
    }
    {
        std::istringstream code("0x1.0u32");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("unexpected 'u' after float literal at line:1 column:6"));
    }

    {
        std::istringstream code("1.0x");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("unexpected 'x' after number literal at line:1 column:4"));
    }
    {
        std::istringstream code("0x1.0x");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("unexpected 'x' after number literal at line:1 column:6"));
    }
    {
        std::istringstream code("0xffi8");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("number literal is out of range at line:1 column:6"));
    }
    {
        std::istringstream code("0xffffffffffffffffffffffff");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("number literal is out of range at line:1 column:26"));
    }
    {
        std::istringstream code("3.812823466e+38F");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("number literal is out of range at line:1 column:16"));
    }
    {
        std::istringstream code("4e+380");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("number literal is out of range at line:1 column:6"));
    }
    {
        std::istringstream code("999999999999999999999i64");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("number literal is out of range at line:1 column:24"));
    }
    {
        std::istringstream code("999999999999999999999u64");
        auto lexer = Lexer::GetLexer(code);
        CHECK_THROWS_MATCHES(lexer->GetToken(), Exception, WhatEquals("number literal is out of range at line:1 column:24"));
    }
}