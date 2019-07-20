
#include "catch2_ext.hpp"
#include "lexer.hpp"
using namespace cd;
using namespace script;

TEST_CASE("NumberValue-Get-Set-Type", "[core][lexer][number]")
{
    NumberValue v;
    v.set((int32_t)1);
    CHECK(v.get<int32_t>() == (int32_t)1);
    v.set((int32_t)1);
    CHECK_THROWS_MATCHES(v.get<int8_t>(), NumberTypeError, WhatEquals("expected <int8_t> but real type is <int32_t>"));
    v.set((int32_t)1);
    CHECK_THROWS_MATCHES(v.get<int16_t>(), NumberTypeError, WhatEquals("expected <int16_t> but real type is <int32_t>"));
    v.set((int32_t)1);
    CHECK_THROWS_MATCHES(v.get<int64_t>(), NumberTypeError, WhatEquals("expected <int64_t> but real type is <int32_t>"));
    v.set((int32_t)1);
    CHECK_THROWS_MATCHES(v.get<uint8_t>(), NumberTypeError, WhatEquals("expected <uint8_t> but real type is <int32_t>"));
    v.set((int32_t)1);
    CHECK_THROWS_MATCHES(v.get<uint16_t>(), NumberTypeError, WhatEquals("expected <uint16_t> but real type is <int32_t>"));
    v.set((int32_t)1);
    CHECK_THROWS_MATCHES(v.get<uint32_t>(), NumberTypeError, WhatEquals("expected <uint32_t> but real type is <int32_t>"));
    v.set((int32_t)1);
    CHECK_THROWS_MATCHES(v.get<uint64_t>(), NumberTypeError, WhatEquals("expected <uint64_t> but real type is <int32_t>"));
    v.set((int32_t)1);
    CHECK_THROWS_MATCHES(v.get<float>(), NumberTypeError, WhatEquals("expected <float> but real type is <int32_t>"));
    v.set((int32_t)1);
    CHECK_THROWS_MATCHES(v.get<double>(), NumberTypeError, WhatEquals("expected <double> but real type is <int32_t>"));

    v.set((int8_t)1);
    CHECK_THROWS_MATCHES(v.get<int32_t>(), NumberTypeError, WhatEquals("expected <int32_t> but real type is <int8_t>"));
    v.set((int16_t)1);
    CHECK_THROWS_MATCHES(v.get<int32_t>(), NumberTypeError, WhatEquals("expected <int32_t> but real type is <int16_t>"));
    v.set((int64_t)1);
    CHECK_THROWS_MATCHES(v.get<int32_t>(), NumberTypeError, WhatEquals("expected <int32_t> but real type is <int64_t>"));
    v.set((uint8_t)1);
    CHECK_THROWS_MATCHES(v.get<int32_t>(), NumberTypeError, WhatEquals("expected <int32_t> but real type is <uint8_t>"));
    v.set((uint16_t)1);
    CHECK_THROWS_MATCHES(v.get<int32_t>(), NumberTypeError, WhatEquals("expected <int32_t> but real type is <uint16_t>"));
    v.set((uint32_t)1);
    CHECK_THROWS_MATCHES(v.get<int32_t>(), NumberTypeError, WhatEquals("expected <int32_t> but real type is <uint32_t>"));
    v.set((uint64_t)1);
    CHECK_THROWS_MATCHES(v.get<int32_t>(), NumberTypeError, WhatEquals("expected <int32_t> but real type is <uint64_t>"));
    v.set(1.0f);
    CHECK_THROWS_MATCHES(v.get<int32_t>(), NumberTypeError, WhatEquals("expected <int32_t> but real type is <float>"));
    v.set(1.0);
    CHECK_THROWS_MATCHES(v.get<int32_t>(), NumberTypeError, WhatEquals("expected <int32_t> but real type is <double>"));
}

TEST_CASE("NumberValue-Type-Info", "[core][lexer][number]")
{
    {
        NumberValue v;
        v.set(1);
        CHECK(v.is_integer() == true);
        CHECK(v.is_signed() == true);
        v.set(1.0f);
        CHECK(v.is_integer() == false);
        v.set((uint8_t)0);
        CHECK(v.is_signed() == false);
        CHECK(v.byte_size() == 1);
        v.set(1.0);
        CHECK(v.is_integer() == false);
        CHECK(v.is_signed() == true);
        CHECK(v.byte_size() == 8);
    }
}

TEST_CASE("NumberValue-Type-Cast", "[core][lexer][number]")
{
    {
        NumberValue v;
        v.set(1);
        CHECK(v.cast_to<float>() == 1.0f);
        v.set(1.0);
        CHECK(v.cast_to<int32_t>() == 1);
        CHECK(v.cast_to<int8_t>() == 1);
        v.set(2.5);
        CHECK(v.cast_to<int32_t>() == 2);
        v.set(2.1);
        CHECK(v.cast_to<double>() == 2.1);
        CHECK(v.cast_to<float>() == 2.1f);
        CHECK(v.cast_to<int8_t>() == (int8_t)2);
        CHECK(v.cast_to<int16_t>() == (int16_t)2);
        CHECK(v.cast_to<int32_t>() == (int32_t)2);
        CHECK(v.cast_to<int64_t>() == (int64_t)2);
        CHECK(v.cast_to<uint8_t>() == (uint8_t)2);
        CHECK(v.cast_to<uint16_t>() == (uint16_t)2);
        CHECK(v.cast_to<uint32_t>() == (uint32_t)2);
        CHECK(v.cast_to<uint64_t>() == (uint64_t)2);
    }
}