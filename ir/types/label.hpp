#pragma once
#include <ostream>
#include <ir/types/type.hpp>

namespace anvil::ir
{
    class LabelType final : public Type
    {
    public:
        LabelType() : Type(Kind::Label) {}
        void print(std::ostream &os) const override
        {
            os << "label";
        }
    };
}