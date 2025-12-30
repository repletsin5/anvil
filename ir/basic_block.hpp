#pragma once
#include <vector>
#include <string>
#include <ir/instruction.hpp>

namespace anvil::ir
{

    class BasicBlock
    {
    public:
        explicit BasicBlock(std::string name) : name_(name) {}

        void addInstruction(Instruction *inst) { instructions_.push_back(inst); }

        void print(std::ostream &os) const
        {
            os << name_ << ":\n";
            for (auto *inst : instructions_)
                inst->print(os);
        }

    private:
        std::string name_;
        std::vector<Instruction *> instructions_;
    };

}
