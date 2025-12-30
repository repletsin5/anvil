#pragma once

#include <vector>
#include <memory>
#include <ir/type.hpp>

namespace anvil::ir
{
    class Type;

    class Context
    {
    public:
        Context() = default;

        Type *getVoidType();
        Type *getIntType(unsigned bits)
        {
            for (auto &t : types_)
                if (t->getKind() == Type::Kind::Integer && t->getBitWidth() == bits)
                    return t.get();
            types_.emplace_back(new Type(Type::Kind::Integer, bits));
            return types_.back().get();
        }

    private:
        std::vector<std::unique_ptr<Type>> types_;
    };
}