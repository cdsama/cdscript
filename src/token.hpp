// Copyright (c) 2018 chendi
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once
#include <any>
#include "cdscript.hpp"
namespace cdscript
{
using token_t = int32_t;
struct Token
{
    enum Type
    {
        First = 256,
        Integer,
        Number,
        String,
        Identifier,
        Comment,
        EndOfFile,
    };
    token_t type = 0;
    size_t line = 0;
    size_t column = 0;
    std::any value;
};

} // namespace cdscript