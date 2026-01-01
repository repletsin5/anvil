#pragma once

#include <vector>
#include <memory>
#include <ostream>
#include <cassert>

#include <ir/value.hpp>
#include <ir/instruction.hpp>
#include <ir/types/type.hpp>

namespace anvil::ir
{
    class Function;

    class BasicBlock final : public Value
    {
    public:
        explicit BasicBlock(Type *labelTy,
                            std::string name = {},
                            Function *parent = nullptr)
            : Value(labelTy, std::move(name)), parent_(parent) {}

        Function *getParent() const noexcept
        {
            return parent_;
        }

        void setParent(Function *F) noexcept
        {
            parent_ = F;
        }

        void addInstruction(std::unique_ptr<Instruction> inst)
        {
            assert(!hasTerminator() && "Cannot add instruction after terminator");
            instructions_.push_back(std::move(inst));
        }

        void setTerminator(std::unique_ptr<Instruction> term)
        {
            assert(term->isTerminator() && "Instruction is not a terminator");
            assert(!hasTerminator() && "BasicBlock already has a terminator");
            terminator_ = std::move(term);
        }

        bool hasTerminator() const noexcept { return terminator_ != nullptr; }

        const std::vector<std::unique_ptr<Instruction>> &getInstructions() const noexcept
        {
            return instructions_;
        }

        Instruction *getTerminator() const noexcept
        {
            return terminator_.get();
        }

        void print(std::ostream &os) const override
        {
            unsigned dummyId = 0;
            print(os, dummyId);
        }

        void print(std::ostream &os, unsigned &nextId) const
        {
            os << name_ << ":\n";

            for (const auto &inst : instructions_)
            {
                if (inst->getType() != nullptr)
                    inst->setId(nextId++);

                os << "  ";
                inst->print(os);
                os << "\n";
            }

            if (terminator_)
            {
                os << "  ";
                terminator_->print(os);
                os << "\n";
            }
        }

    private:
        Function *parent_ = nullptr;
        std::vector<std::unique_ptr<Instruction>> instructions_;
        std::unique_ptr<Instruction> terminator_;
    };
}
