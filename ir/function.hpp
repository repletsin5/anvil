#pragma once

#include <ir/basic_block.hpp>
#include <ir/type.hpp>
#include <ir/value.hpp>

namespace anvil::ir
{

    class Function : public Value
    {
    public:
        Function(Type *retType, std::string name, std::vector<Type *> params = {})
            : Value(nullptr, std::move(name)), retType_(retType), paramTypes_(std::move(params)) {}

        void addBlock(std::unique_ptr<BasicBlock> bb) { blocks_.push_back(std::move(bb)); }

        void print(std::ostream &os) const override
        {
            os << "define ";
            retType_->print(os);
            os << " " << global(name_) << "(";
            for (size_t i = 0; i < paramTypes_.size(); ++i)
            {
                if (i)
                    os << ", ";
                paramTypes_[i]->print(os);
            }
            os << ") {\n";
            for (const auto &bb : blocks_)
            {
                bb->print(os);
            }
            os << "}\n";
        }

    private:
        Type *retType_;
        std::vector<Type *> paramTypes_;
        std::vector<std::unique_ptr<BasicBlock>> blocks_;
    };
}
