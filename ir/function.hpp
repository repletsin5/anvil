#pragma once

#include <ir/basic_block.hpp>

namespace anvil::ir
{

    class Function : public Value
    {
    public:
        using Value::Value;

        void addBlock(BasicBlock *bb)
        {
            blocks_.push_back(bb);
        }

        void print(std::ostream &os) const
        {
            os << "define ";
            type_->print(os);
            os << " @" << name_ << "() {\n";
            for (auto *bb : blocks_)
                bb->print(os);
            os << "}\n";
        }

    private:
        std::vector<BasicBlock *> blocks_;
    };
}
