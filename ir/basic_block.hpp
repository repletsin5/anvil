#pragma once

#include <vector>
#include <string>
#include <ir/instruction.hpp>

namespace anvil::ir
{

    class BasicBlock : public Value
    {
    public:
        BasicBlock(const std::string &name) : Value(nullptr), name_(name) {}

        const std::string &getName() const
        {
            return name_;
        }

        void addInstruction(std::unique_ptr<Instruction> inst)
        {
            instructions_.push_back(std::move(inst));
        }

        const std::vector<std::unique_ptr<Instruction>> &getInstructions() const
        {
            return instructions_;
        }

        void print(std::ostream &os) const override
        {
            os << name_ << ":\n";
            for (const auto &inst : instructions_)
            {
                os << "  ";
                inst->print(os);
                os << "\n";
            }
        }

    private:
        std::string name_;
        std::vector<std::unique_ptr<Instruction>> instructions_;
    };
}
