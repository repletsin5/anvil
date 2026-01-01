#pragma once

#include <ir/instruction.hpp>
#include <ir/basic_block.hpp>

namespace anvil::ir
{
    inline void Instruction::printTerminator(std::ostream &os) const
    {
        switch (opcode_)
        {
        case Opcode::Br:
            if (operands_.size() == 1)
            {
                os << "br label %" << static_cast<BasicBlock *>(operands_[0])->getName();
            }
            else if (operands_.size() == 3)
            {
                os << "br i1 ";
                operands_[0]->printAsOperand(os);
                os << ", label %" << static_cast<BasicBlock *>(operands_[1])->getName();
                os << ", label %" << static_cast<BasicBlock *>(operands_[2])->getName();
            }
            else
            {
                os << "br";
            }
            break;

        case Opcode::Ret:
            if (!operands_.empty())
            {
                os << "ret ";
                operands_[0]->getType()->print(os);
                os << " ";
                operands_[0]->printAsOperand(os);
            }
            else
            {
                os << "ret void";
            }
            break;

        default:
            os << "unknown terminator";
            break;
        }
    }

    inline void Instruction::printOtherOps(std::ostream &os) const
    {
        if (opcode_ == Opcode::PHI)
        {
            os << "phi ";
            type_->print(os);
            os << " ";
            for (size_t i = 0; i < incoming_.size(); ++i)
            {
                if (i > 0)
                    os << ", ";
                os << "[";
                incoming_[i].first->printAsOperand(os);
                os << ", %";
                os << incoming_[i].second->getName();
                os << "]";
            }
            return;
        }

        if (opcode_ == Opcode::ICmp)
        {
            os << "icmp " << icmpPredStr() << " ";
            operands_[0]->getType()->print(os);
            os << " ";
            operands_[0]->printAsOperand(os);
            os << ", ";
            operands_[1]->printAsOperand(os);
            return;
        }

        if (operands_.size() == 2)
        {
            os << opcodeStr() << " ";
            operands_[0]->getType()->print(os);
            os << " ";
            operands_[0]->printAsOperand(os);
            os << ", ";
            operands_[1]->printAsOperand(os);
            return;
        }

        os << opcodeStr() << " ";
        for (size_t i = 0; i < operands_.size(); ++i)
        {
            if (i > 0)
                os << ", ";
            operands_[i]->getType()->print(os);
            os << " ";
            operands_[i]->printAsOperand(os);
        }
    }
}
