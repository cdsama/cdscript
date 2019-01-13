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
#include <deque>
#include <stack>
#include <sstream>
#include "catch2_ext.hpp"
#include "serialize.hpp"

using namespace cd;
using namespace serialize;

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
    Archive<Reader> arr(ss);
    arr << v2;
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
        Archive<Reader> arr(ss);
        decltype(v1) v2;
        arr << v2;
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
        Archive<Reader> arr(ss);
        decltype(v1) v2;
        arr << v2;
        CHECK(v1 == v2);
    }
    {
        std::vector<int32_t> v1 = {1, 2, 3, 4, 5};
        std::stringstream ss;
        Archive<Writer> ar(ss);
        ar << v1;
        Archive<Reader> arr(ss);
        decltype(v1) v2;
        arr << v2;
        CHECK(v1 == v2);
    }
    {
        std::vector<std::string> v1 = {"H", "E", "L", "L", "O", "world"};
        std::stringstream ss;
        Archive<Writer> ar(ss);
        ar << v1;
        Archive<Reader> arr(ss);
        decltype(v1) v2;
        arr << v2;
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
        Archive<Reader> arr(ss);
        decltype(v1) v2;
        arr << v2;
        CHECK(v1 == v2);
    }
    {
        std::array<std::string, 6> v1 = {"H", "E", "L", "L", "O", "world"};
        std::stringstream ss;
        Archive<Writer> ar(ss);
        ar << v1;
        Archive<Reader> arr(ss);
        decltype(v1) v2;
        arr << v2;
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
        Archive<Reader> arr(ss);
        decltype(v1) v2;
        arr << v2;
        CHECK(v1 == v2);
    }
    {
        std::list<std::string> v1 = {"H", "E", "L", "L", "O", "world"};
        std::stringstream ss;
        Archive<Writer> ar(ss);
        ar << v1;
        Archive<Reader> arr(ss);
        decltype(v1) v2;
        arr << v2;
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
        Archive<Reader> arr(ss);
        decltype(v1) v2;
        arr << v2;
        CHECK(v1 == v2);
    }
    {
        std::set<std::string> v1 = {"A", "B", "C", "D", "E"};
        std::stringstream ss;
        Archive<Writer> ar(ss);
        ar << v1;
        Archive<Reader> arr(ss);
        decltype(v1) v2;
        arr << v2;
        CHECK(v1 == v2);
    }
    {
        std::unordered_set<int32_t> v1 = {1, 2, 3, 4, 5};
        std::stringstream ss;
        Archive<Writer> ar(ss);
        ar << v1;
        Archive<Reader> arr(ss);
        decltype(v1) v2;
        arr << v2;
        CHECK(v1 == v2);
    }
    {
        std::unordered_set<std::string> v1 = {"A", "B", "C", "D", "E"};
        std::stringstream ss;
        Archive<Writer> ar(ss);
        ar << v1;
        Archive<Reader> arr(ss);
        decltype(v1) v2;
        arr << v2;
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
        Archive<Reader> arr(ss);
        decltype(v1) v2;
        arr << v2;
        CHECK(v1 == v2);
    }
    {
        std::unordered_map<std::string, int32_t> v1 = {{"Alice", 1}, {"Bob", 2}, {"Cindy", 3}, {"David", 4}, {"Ella", 5}};
        std::stringstream ss;
        Archive<Writer> ar(ss);
        ar << v1;
        Archive<Reader> arr(ss);
        decltype(v1) v2;
        arr << v2;
        CHECK(v1 == v2);
    }
    {
        std::unordered_multimap<std::string, int32_t> v1 = {{"Alice", 1}, {"Bob", 2}, {"Bob", 2}, {"Cindy", 3}, {"David", 4}, {"Ella", 5}};
        std::stringstream ss;
        Archive<Writer> ar(ss);
        ar << v1;
        Archive<Reader> arr(ss);
        decltype(v1) v2;
        arr << v2;
        CHECK(v1 == v2);
    }
}

TEST_CASE("Serialize-std-deque", "[core][serialize]")
{
    {
        std::deque<int32_t> v1 = {1, 2, 3, 4, 5};
        std::stringstream ss;
        Archive<Writer> ar(ss);
        ar << v1;
        Archive<Reader> arr(ss);
        decltype(v1) v2;
        arr << v2;
        CHECK(v1 == v2);
    }
}

TEST_CASE("Serialize-std-stack", "[core][serialize]")
{
    {
        std::stack<int32_t> v1;
        auto data = {1, 2, 3, 4, 5};
        for (auto &&i : data)
        {
            v1.push(i);
        }
        std::stringstream ss;
        Archive<Writer> ar(ss);
        ar << v1;
        Archive<Reader> arr(ss);
        decltype(v1) v2;
        arr << v2;
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
    Archive<Reader> arr(ss);
    int i = 0;
    CHECK_THROWS_MATCHES(ar << i, Exception, WhatEquals("Failed to write 4 bytes to output stream! Wrote 0"));
    CHECK_THROWS_MATCHES(arr << i, Exception, WhatEquals("Failed to read 4 bytes from input stream! Read 0"));
}

class NoDefaultConstructor
{
    int i;

  public:
    NoDefaultConstructor(int _i)
        : i(_i)
    {
    }
    int j = 0;
    int GetI()
    {
        return i;
    }

    template <typename Archive>
    friend Archive &operator<<(Archive &ar, NoDefaultConstructor &s)
    {
        ar << s.i << s.j;
        return ar;
    }
    template <typename Archive>
    friend Archive &operator<<(Archive &ar, Constructor<NoDefaultConstructor> &constructor)
    {
        int i;
        ar << i;
        constructor(i);
        ar << constructor->j;
        return ar;
    }
};

TEST_CASE("Serialize-NoDefaultConstructor", "[core][serialize]")
{
    {
        std::stringstream ss;
        Archive<Writer> ar(ss);
        NoDefaultConstructor v1 = {5};
        v1.j = 3;
        ar << v1;
        ar << v1;
        ar << v1;
        ar << v1;
        Archive<Reader> arr(ss);
        Constructor<NoDefaultConstructor> v2;
        arr << v2;
        CHECK(v1.GetI() == v2.get_unique()->GetI());
        arr << v2;
        CHECK(v1.j == v2.get_shared()->j);
        arr << v2;
        arr << v2;
        // decltype(v1) *v4;
        // arr << v4; // compile error
    }
}

struct A
{
    int a = 0;
    virtual ~A() {}
    virtual std::string Type()
    {
        return "A";
    }
    template <typename Archive>
    friend Archive &operator<<(Archive &ar, A &v)
    {
        ar << v.a;
        return ar;
    }
};

struct B : A
{
    int b = 0;
    virtual std::string Type()
    {
        return "B";
    }
    template <typename Archive>
    friend Archive &operator<<(Archive &ar, B &v)
    {
        ar << v.a << v.b;
        return ar;
    }
};

struct C : A
{
    int c = 0;
    C(int i)
    {
        c = i;
    }
    virtual std::string Type()
    {
        return "C";
    }
    template <typename Archive>
    friend Archive &operator<<(Archive &ar, C &v)
    {
        ar << v.a << v.c;
        return ar;
    }
    template <typename Archive>
    friend Archive &operator<<(Archive &ar, Constructor<C> &constructor)
    {
        int c;
        int a;
        ar << a << c;
        constructor(c);
        constructor->a = a;
        return ar;
    }
};

struct D : B
{
    int d = 0;
    virtual std::string Type()
    {
        return "D";
    }
    template <typename Archive>
    friend Archive &operator<<(Archive &ar, D &v)
    {
        ar << v.a << v.b << v.d;
        return ar;
    }
};

REGIST_TYPE(A);
REGIST_TYPE(B);
REGIST_TYPE(C);
REGIST_TYPE(D);

struct E
{
    int e = 0;
    template <typename Archive>
    friend Archive &operator<<(Archive &ar, E &v)
    {
        ar << v.e;
        return ar;
    }
};

struct F
{
    int f = 0;
    F(int i)
    {
        f = i;
    }
    template <typename Archive>
    friend Archive &operator<<(Archive &ar, F &v)
    {
        ar << v.f;
        return ar;
    }
    template <typename Archive>
    friend Archive &operator<<(Archive &ar, Constructor<F> &constructor)
    {
        int f;
        ar << f;
        constructor(f);
        return ar;
    }
};

TEST_CASE("Serialize-polymorphic", "[core][serialize]")
{
    {
        std::unique_ptr<A> a = std::make_unique<A>();
        std::unique_ptr<A> b = std::make_unique<B>();
        std::unique_ptr<A> c = std::make_unique<C>(3);
        std::unique_ptr<A> d = std::make_unique<D>();
        std::unique_ptr<A> e;
        std::stringstream ss;
        Archive<Writer> ar(ss);
        ar << a;
        ar << b;
        ar << c;
        ar << d;
        ar << e;
        Archive<Reader> arr(ss);
        std::unique_ptr<A> aa;
        std::unique_ptr<A> bb;
        std::unique_ptr<A> cc;
        std::unique_ptr<A> dd;
        std::unique_ptr<A> ee;
        arr << aa;
        arr << bb;
        arr << cc;
        arr << dd;
        arr << ee;
        REQUIRE(aa != nullptr);
        CHECK(aa->Type() == "A");
        REQUIRE(bb != nullptr);
        CHECK(bb->Type() == "B");
        REQUIRE(cc != nullptr);
        CHECK(cc->Type() == "C");
        CHECK(static_cast<C *>(c.get())->c == static_cast<C *>(cc.get())->c);
        REQUIRE(dd != nullptr);
        CHECK(dd->Type() == "D");
        CHECK(ee == nullptr);
    }
    {
        std::unique_ptr<E> e = std::make_unique<E>();
        e->e = 3;
        std::unique_ptr<E> f;
        std::stringstream ss;
        Archive<Writer> ar(ss);
        ar << e;
        ar << f;
        Archive<Reader> arr(ss);
        std::unique_ptr<E> ee;
        std::unique_ptr<E> ff;
        arr << ee;
        arr << ff;
        REQUIRE(ee != nullptr);
        CHECK(e->e == ee->e);
        CHECK(ff == nullptr);
    }
    {
        std::unique_ptr<F> f = std::make_unique<F>(3);
        std::stringstream ss;
        Archive<Writer> ar(ss);
        ar << f;
        Archive<Reader> arr(ss);
        std::unique_ptr<F> ff;
        arr << ff;
        REQUIRE(ff != nullptr);
        CHECK(f->f == ff->f);
    }
}