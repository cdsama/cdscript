// Copyright (c) 2019 chendi
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <vector>
#include <array>
#include <list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <sstream>
#include "catch2_ext.hpp"
#include "serialize.hpp"

using namespace cd::serialize;

struct Types
{
    bool b = false;
    int8_t i8 = 0;
    int16_t i16 = 0;
    int32_t i32 = 0;
    int64_t i64 = 0;
    uint8_t u8 = 0;
    uint16_t u16 = 0;
    uint32_t u32 = 0;
    uint64_t u64 = 0;

    friend bool operator==(const Types &lhs, const Types &rhs)
    {
        return ((lhs.b == rhs.b) &&
                (lhs.i8 == rhs.i8) &&
                (lhs.i16 == rhs.i16) &&
                (lhs.i32 == rhs.i32) &&
                (lhs.i64 == rhs.i64) &&
                (lhs.u8 == rhs.u8) &&
                (lhs.u16 == rhs.u16) &&
                (lhs.u32 == rhs.u32) &&
                (lhs.u64 == rhs.u64));
    }

    template <typename Archive>
    friend Archive &operator<<(Archive &ar, Types &t)
    {
        ar << t.b << t.i8 << t.i16 << t.i32 << t.i64 << t.u8 << t.u16 << t.u32 << t.u64;
        return ar;
    }
};

TEST_CASE("Serialize-Basic", "[core][serialize]")
{
    Types v1 = {false, -1, -2, -3, -4, 1, 2, 3, 4};
    std::stringstream ss;
    Archive<Writer> ar(ss);
    ar << v1;
    decltype(v1) v2;
    Archive<Reader> ar2(ss);
    ar2 << v2;
    CHECK(v1 == v2);
}

struct Strings
{
    std::string s1;
    std::wstring s2;
    std::u16string s3;
    std::u32string s4;
    friend bool operator==(const Strings &lhs, const Strings &rhs)
    {
        return ((lhs.s1 == rhs.s1) && (lhs.s2 == rhs.s2) && (lhs.s3 == rhs.s3) && (lhs.s4 == rhs.s4));
    }
    template <typename Archive>
    friend Archive &operator<<(Archive &ar, Strings &s)
    {
        ar << s.s1 << s.s2 << s.s3 << s.s4;
        return ar;
    }
};

TEST_CASE("Serialize-String", "[core][serialize]")
{
    {
        Strings v1 = {"Hello World!", L"你好世界！", u"ハローワールド", U"전 세계 여러분 안녕하세요"};
        std::stringstream ss;
        Archive<Writer> ar(ss);
        ar << v1;
        Archive<Reader> ar2(ss);
        decltype(v1) v2;
        ar2 << v2;
        CHECK(v1 == v2);
    }
}

TEST_CASE("Serialize-std-vector", "[core][serialize]")
{
    {
        std::vector<bool> v1 = {true, false, true, true, false};
        std::stringstream ss;
        Archive<Writer> ar(ss);
        ar << v1;
        Archive<Reader> ar2(ss);
        decltype(v1) v2;
        ar2 << v2;
        CHECK(v1 == v2);
    }
    {
        std::vector<int32_t> v1 = {1, 2, 3, 4, 5};
        std::stringstream ss;
        Archive<Writer> ar(ss);
        ar << v1;
        Archive<Reader> ar2(ss);
        decltype(v1) v2;
        ar2 << v2;
        CHECK(v1 == v2);
    }
    {
        std::vector<std::string> v1 = {"H", "E", "L", "L", "O", "world"};
        std::stringstream ss;
        Archive<Writer> ar(ss);
        ar << v1;
        Archive<Reader> ar2(ss);
        decltype(v1) v2;
        ar2 << v2;
        CHECK(v1 == v2);
    }
}

TEST_CASE("Serialize-std-array", "[core][serialize]")
{
    {
        std::array<int32_t, 5> v1 = {1, 2, 3, 4, 5};
        std::stringstream ss;
        Archive<Writer> ar(ss);
        ar << v1;
        Archive<Reader> ar2(ss);
        decltype(v1) v2;
        ar2 << v2;
        CHECK(v1 == v2);
    }
    {
        std::array<std::string, 6> v1 = {"H", "E", "L", "L", "O", "world"};
        std::stringstream ss;
        Archive<Writer> ar(ss);
        ar << v1;
        Archive<Reader> ar2(ss);
        decltype(v1) v2;
        ar2 << v2;
        CHECK(v1 == v2);
    }
}

TEST_CASE("Serialize-std-list", "[core][serialize]")
{
    {
        std::list<int32_t> v1 = {1, 2, 3, 4, 5};
        std::stringstream ss;
        Archive<Writer> ar(ss);
        ar << v1;
        Archive<Reader> ar2(ss);
        decltype(v1) v2;
        ar2 << v2;
        CHECK(v1 == v2);
    }
    {
        std::list<std::string> v1 = {"H", "E", "L", "L", "O", "world"};
        std::stringstream ss;
        Archive<Writer> ar(ss);
        ar << v1;
        Archive<Reader> ar2(ss);
        decltype(v1) v2;
        ar2 << v2;
        CHECK(v1 == v2);
    }
}

TEST_CASE("Serialize-std-set", "[core][serialize]")
{
    {
        std::set<int32_t> v1 = {1, 2, 3, 4, 5};
        std::stringstream ss;
        Archive<Writer> ar(ss);
        ar << v1;
        Archive<Reader> ar2(ss);
        decltype(v1) v2;
        ar2 << v2;
        CHECK(v1 == v2);
    }
    {
        std::unordered_set<int32_t> v1 = {1, 2, 3, 4, 5};
        std::stringstream ss;
        Archive<Writer> ar(ss);
        ar << v1;
        Archive<Reader> ar2(ss);
        decltype(v1) v2;
        ar2 << v2;
        CHECK(v1 == v2);
    }
}

TEST_CASE("Serialize-std-map", "[core][serialize]")
{
    {
        std::map<std::string, int32_t> v1 = {{"Alice", 1}, {"Bob", 2}, {"Cindy", 3}, {"David", 4}, {"Ella", 5}};
        std::stringstream ss;
        Archive<Writer> ar(ss);
        ar << v1;
        Archive<Reader> ar2(ss);
        decltype(v1) v2;
        ar2 << v2;
        CHECK(v1 == v2);
    }
    {
        std::unordered_map<std::string, int32_t> v1 = {{"Alice", 1}, {"Bob", 2}, {"Cindy", 3}, {"David", 4}, {"Ella", 5}};
        std::stringstream ss;
        Archive<Writer> ar(ss);
        ar << v1;
        Archive<Reader> ar2(ss);
        decltype(v1) v2;
        ar2 << v2;
        CHECK(v1 == v2);
    }
    {
        std::unordered_multimap<std::string, int32_t> v1 = {{"Alice", 1}, {"Bob", 2}, {"Bob", 2}, {"Cindy", 3}, {"David", 4}, {"Ella", 5}};
        std::stringstream ss;
        Archive<Writer> ar(ss);
        ar << v1;
        Archive<Reader> ar2(ss);
        decltype(v1) v2;
        ar2 << v2;
        CHECK(v1 == v2);
    }
}

struct sbt : std::basic_streambuf<std::iostream::char_type, std::iostream::traits_type>
{
};

TEST_CASE("Serialize-Exception", "[core][serialize]")
{
    sbt sb;
    std::iostream ss(&sb);
    Archive<Writer> ar(ss);
    Archive<Reader> ar2(ss);
    int i = 0;
    CHECK_THROWS_MATCHES(ar << i, Exception, WhatEquals("Failed to write 4 bytes to output stream! Wrote 0"));
    CHECK_THROWS_MATCHES(ar2 << i, Exception, WhatEquals("Failed to read 4 bytes from input stream! Read 0"));
}