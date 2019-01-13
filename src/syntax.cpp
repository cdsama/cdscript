// Copyright (c) 2019 chendi
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "syntax.hpp"
#include "visitor.hpp"

namespace cd::script
{
#define IMPL_VISIT_FUNC(__CLASS_NAME__) \
    void __CLASS_NAME__::Visit(Visitor *visitor, std::any &data) { visitor->Visit(this, data); }

IMPL_VISIT_FUNC(BinaryExpression)
IMPL_VISIT_FUNC(LiteralValue)

}  // namespace cd::script
