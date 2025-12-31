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
            if (cond_)
            {
                os << "br i1 ";
                cond_->printAsOperand(os);
                os << ", label %" << thenBB_->getName()
                   << ", label %" << elseBB_->getName();
            }
            else if (branchTarget_)
            {
                os << "br label %" << branchTarget_->getName();
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

        if (opcode_ == Opcode::PHI)
        {
            os << "phi ";
            type_->print(os);
            os << " ";
            for (size_t i = 0; i < phiValues_.size(); ++i)
            {
                if (i > 0)
                    os << ", ";
                os << "[";
                phiValues_[i]->printAsOperand(os);
                os << ", %";
                os << phiBlocks_[i]->getName();
                os << "]";
            }
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
