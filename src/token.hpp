// Copyright (c) 2018 chendi
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once
#include <any>
#include <limits>
#include <string>
#include <map>
#include "cdscript.hpp"
namespace cdscript
{
using token_t = int32_t;
struct Token
{
    enum Type
    {
        First = 256,
        Number,
        String,
        Identifier,
        Comment,
        EndOfFile,
    };

    template <typename T>
    struct SupportedNumberType
    {
        static constexpr bool value = false;
    };

#define SupportNumberType(__typename__)                    \
    template <>                                            \
    struct SupportedNumberType<__typename__>               \
    {                                                      \
        static constexpr bool value = true;                \
        static constexpr const char *name = #__typename__; \
    };

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

    template <typename T>
    struct NumberType<T, typename std::enable_if<SupportedNumberType<T>::value>::type>
    {
        enum
        {
            value = (std::numeric_limits<T>::digits << 2) + (std::numeric_limits<T>::is_integer << 1) + std::numeric_limits<T>::is_signed,
        };

        static T get(std::any &number, int32_t real)
        {
            try
            {
                return std::any_cast<T>(number);
            }
            catch (std::bad_any_cast e)
            {
                throw NumberTypeError(SupportedNumberType<T>::name, NumberTypeMap[real]);
            }
        }
    };
#define REGIST_NUMBER_TYPE_NAME(__typename__)                                    \
    {                                                                            \
        NumberType<__typename__>::value, SupportedNumberType<__typename__>::name \
    }

    inline static std::map<int32_t, const char *> NumberTypeMap = {
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
        int32_t type;
        std::any number;
        template <typename T>
        T value()
        {
            return NumberType<T>::get(number, type);
        }
    };

    token_t type = 0;
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

} // namespace cdscript