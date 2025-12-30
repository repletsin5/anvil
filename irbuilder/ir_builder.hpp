#pragma once

#include <ir/instruction.hpp>
#include <ir/basic_block.hpp>

namespace anvil::ir
{

    class IRBuilder
    {
    public:
        explicit IRBuilder(BasicBlock *bb) : block_(bb) {}

        Instruction *CreateAdd(Value *lhs, Value *rhs)
        {
            auto *inst = new Instruction(lhs->getType(), Instruction::Opcode::Add, {lhs, rhs});
            block_->addInstruction(inst);
            return inst;
        }

        Instruction *CreateRet(Value *val)
        {
            auto *inst = new Instruction(val->getType(), Instruction::Opcode::Ret, {val});
            block_->addInstruction(inst);
            return inst;
        }

    private:
        BasicBlock *block_;
    };

}
