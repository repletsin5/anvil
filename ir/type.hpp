#pragma once

#include <ostream>
#include <cstdint>

namespace anvil::ir
{
    class Context;

    class Type
    {
        friend class Context;

    public:
        enum class Kind
        {
            Void,
            Integer,
            Pointer,
            Function
        };

        Kind getKind() const { return kind_; }
        unsigned getBitWidth() const { return bitWidth_; }
        void print(std::ostream &os) const
        {
            switch (kind_)
            {
            case Kind::Void:
                os << "void";
                break;
            case Kind::Integer:
                os << "i" << bitWidth_;
                break;
            default:
                os << "<type>";
                break;
            }
        }

    protected:
        explicit Type(Kind k, unsigned bits = 0)
            : kind_(k), bitWidth_(bits) {}

    private:
        Kind kind_;
        unsigned bitWidth_;
    };
}