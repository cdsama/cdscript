// Copyright (c) 2019 chendi
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once
#include <any>
#include <limits>
#include <map>
#include <string>
#include "cdscript.hpp"
namespace cd::script
{
using token_t = int32_t;

template <typename T>
struct SupportedNumberType
{
    static constexpr bool value = false;
};

#define SupportNumberType(__TYPE_NAME__)                    \
    template <>                                             \
    struct SupportedNumberType<__TYPE_NAME__>               \
    {                                                       \
        static constexpr bool value = true;                 \
        static constexpr const char *name = #__TYPE_NAME__; \
    }

SupportNumberType(int8_t);
SupportNumberType(int16_t);
SupportNumberType(int32_t);
SupportNumberType(int64_t);
SupportNumberType(uint8_t);
SupportNumberType(uint16_t);
SupportNumberType(uint32_t);
SupportNumberType(uint64_t);
SupportNumberType(float);
SupportNumberType(double);

template <typename T, typename Enable = void>
struct NumberType
{
};

class NumberTypeError : public std::exception
{
  public:
    NumberTypeError(const char *expected, const char *real) noexcept
    {
        ss.append("expected <");
        ss.append(expected);
        ss.append("> but real type is <");
        ss.append(real);
        ss.append(">");
    }

    virtual const char *what() const noexcept override
    {
        return ss.c_str();
    }
    std::string ss;
};

template <typename Td, typename Ts>
static inline Td data_cast(const Ts &v)
{
    return *reinterpret_cast<const Td *>(&v);
}

using type_value_t = uint8_t;
using number_data_t = int64_t;

template <typename T>
struct NumberType<T, typename std::enable_if<SupportedNumberType<T>::value>::type>
{
    static constexpr type_value_t value = (sizeof(T) << 2) + (std::numeric_limits<T>::is_signed << 1) + std::numeric_limits<T>::is_integer;
};
#define REGIST_NUMBER_TYPE_NAME(__typename__)                                    \
    {                                                                            \
        NumberType<__typename__>::value, SupportedNumberType<__typename__>::name \
    }

inline static std::map<type_value_t, const char *> NumberTypeMap = {
    REGIST_NUMBER_TYPE_NAME(int8_t),
    REGIST_NUMBER_TYPE_NAME(int16_t),
    REGIST_NUMBER_TYPE_NAME(int32_t),
    REGIST_NUMBER_TYPE_NAME(int64_t),
    REGIST_NUMBER_TYPE_NAME(uint8_t),
    REGIST_NUMBER_TYPE_NAME(uint16_t),
    REGIST_NUMBER_TYPE_NAME(uint32_t),
    REGIST_NUMBER_TYPE_NAME(uint64_t),
    REGIST_NUMBER_TYPE_NAME(float),
    REGIST_NUMBER_TYPE_NAME(double),
};

struct NumberValue
{
    type_value_t type;
    number_data_t number;
    template <typename T>
    T get()
    {
        if (NumberType<T>::value == type)
        {
            return data_cast<T>(number);
        }
        else
        {
            throw NumberTypeError(NumberTypeMap[NumberType<T>::value], NumberTypeMap[type]);
        }
    }

#define CAST_TO_TYPE_CASE_RETURN(__TYPE__) case NumberType<__TYPE__>::value: \
                return static_cast<T>(data_cast<__TYPE__>(number))

    template <typename T>
    T cast_to()
    {
        if (NumberType<T>::value == type)
        {
            return data_cast<T>(number);
        }
        else
        {
            switch (type)
            {
                CAST_TO_TYPE_CASE_RETURN(int8_t);
                CAST_TO_TYPE_CASE_RETURN(int16_t);
                CAST_TO_TYPE_CASE_RETURN(int32_t);
                CAST_TO_TYPE_CASE_RETURN(uint8_t);
                CAST_TO_TYPE_CASE_RETURN(uint16_t);
                CAST_TO_TYPE_CASE_RETURN(uint32_t);
                CAST_TO_TYPE_CASE_RETURN(uint64_t);
                CAST_TO_TYPE_CASE_RETURN(float);
                CAST_TO_TYPE_CASE_RETURN(double);
            default:
                return static_cast<T>(number);
            }
        }
    }

    bool is_integer()
    {
        return type & 1;
    }

    bool is_signed()
    {
        return type & (1 << 1);
    }

    int32_t byte_size()
    {
        return type >> 2;
    }

    template <typename T>
    void set(const T &v)
    {
        type = NumberType<T>::value;
        number = data_cast<number_data_t>(v);
    }
};

struct Token
{
    enum Type
    {
        Null = 256,
        True,
        False,
        Number,
        String,
        Identifier,
        VarArg,
        Comment,
        And,
        Or,
        Equal,
        NotEqual,
        LessEqual,
        GreatEqual,
        PlusEqual,
        PlusPlus,
        MinusEqual,
        MinusMinus,
        MultiplyEqual,
        DivideEqual,
        ModuloEqual,
        AndEqual,
        OrEqual,
        ExclusiveOrEqual,
        LeftShift,
        RightShift,
        Concat,
        If,
        Else,
        For,
        While,
        In,
        Break,
        Continue,
        Return,
        Function,
        Try,
        Catch,
        Throw,
        Class,
        Interface,
        Is,
        Object,
        This,
        Super,
        Any,
        EndOfFile,
    };

    token_t type = EndOfFile;
    size_t line = 0;
    size_t column = 0;
    std::any value;

    inline std::string &str()
    {
        return std::any_cast<std::string &>(value);
    }

    inline NumberValue &number()
    {
        return std::any_cast<NumberValue &>(value);
    }
};

}  // namespace cd::script