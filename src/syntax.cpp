// Copyright (c) 2018 chendi
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "syntax.hpp"
#include "visitor.hpp"

namespace cdscript
{
#define IMPL_VISIT_FUNC(__CLASS_NAME__) \
    void __CLASS_NAME__::Visit(Visitor *visitor, std::any &data) { visitor->Visit(this, data); }

IMPL_VISIT_FUNC(BinaryExpress)

} // namespace cdscript
