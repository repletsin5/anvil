#pragma once

#include <ostream>
#include <ir/helpers.hpp>
#include <ir/value.hpp>

namespace anvil::ir
{

    class BasicBlock;

    class Instruction : public Value
    {
    public:
        enum class Opcode
        {
            Ret,
            Br,
            Switch,
            IndirectBr,
            Invoke,
            CallBr,
            Resume,
            CatchSwitch,
            CatchRet,
            CleanupRet,
            Unreachable,
            FNeg,
            Add,
            FAdd,
            Sub,
            FSub,
            Mul,
            FMul,
            UDiv,
            SDiv,
            FDiv,
            URem,
            SRem,
            FRem,
            Shl,
            LShr,
            AShr,
            And,
            Or,
            Xor,
            ExtractElement,
            InsertElement,
            ShuffleVector,
            ExtractValue,
            InsertValue,
            Alloca,
            Load,
            Store,
            Fence,
            CmpXchg,
            AtomicRMW,
            GetElementPtr,
            Trunc,
            ZExt,
            SExt,
            FPTrunc,
            FPExt,
            FPToUI,
            FPToSI,
            UIToFP,
            SIToFP,
            PtrToInt,
            PtrToAddr,
            IntToPtr,
            BitCast,
            AddrSpaceCast,
            ICmp,
            FCmp,
            PHI,
            Select,
            Freeze,
            Call,
            VAArg,
            LandingPad,
            CatchPad,
            CleanupPad
        };

        enum class ICmpPredicate
        {
            EQ,
            NE,
            UGT,
            UGE,
            ULT,
            ULE,
            SGT,
            SGE,
            SLT,
            SLE
        };

        Instruction(Type *type, Opcode op, const std::vector<Value *> &operands = {})
            : Value(type), opcode_(op), operands_(operands), id_(-1) {}
        Instruction(BasicBlock *target)
            : Value(nullptr), opcode_(Opcode::Br), branchTarget_(target), id_(-1) {}
        Instruction(Value *cond, BasicBlock *thenBB, BasicBlock *elseBB)
            : Value(nullptr), opcode_(Opcode::Br), cond_(cond), thenBB_(thenBB), elseBB_(elseBB), id_(-1) {}

        void printTerminator(std::ostream &os) const;
        void printOtherOps(std::ostream &os) const;

        void setId(unsigned id)
        {
            id_ = id;
        }
        unsigned getId() const noexcept
        {
            return id_;
        }

        void setPredicate(ICmpPredicate pred)
        {
            icmpPred_ = pred;
        }

        ICmpPredicate getPredicate() const
        {
            return icmpPred_;
        }

        Opcode getOpcode() const noexcept
        {
            return opcode_;
        }

        const std::vector<Value *> &getOperands() const noexcept
        {
            return operands_;
        }

        void addIncoming(Value *val, BasicBlock *bb)
        {
            phiValues_.push_back(val);
            phiBlocks_.push_back(bb);
        }

        const std::vector<Value *> &getIncomingValues() const
        {
            return phiValues_;
        }

        const std::vector<BasicBlock *> &getIncomingBlocks() const
        {
            return phiBlocks_;
        }

        BasicBlock *getBranchTarget() const
        {
            return branchTarget_;
        }

        BasicBlock *getThenBB() const
        {
            return thenBB_;
        }

        BasicBlock *getElseBB() const
        {
            return elseBB_;
        }

        void print(std::ostream &os) const override
        {

            if (!isTerminator())
                os << local(id_) << " = ";

            switch (opcode_)
            {
            case Opcode::Add:
            case Opcode::FAdd:
            case Opcode::Sub:
            case Opcode::FSub:
            case Opcode::Mul:
            case Opcode::FMul:
            case Opcode::UDiv:
            case Opcode::SDiv:
            case Opcode::FDiv:
            case Opcode::URem:
            case Opcode::SRem:
            case Opcode::FRem:
                printBinary(os);
                break;
            case Opcode::FNeg:
                printUnary(os, "fneg");
                break;
            case Opcode::Shl:
            case Opcode::LShr:
            case Opcode::AShr:
            case Opcode::And:
            case Opcode::Or:
            case Opcode::Xor:
                printBinary(os);
                break;
            case Opcode::ExtractElement:
            case Opcode::InsertElement:
            case Opcode::ShuffleVector:
                printVectorOps(os);
                break;
            case Opcode::ExtractValue:
            case Opcode::InsertValue:
                printAggregateOps(os);
                break;
            case Opcode::Alloca:
            case Opcode::Load:
            case Opcode::Store:
            case Opcode::Fence:
            case Opcode::CmpXchg:
            case Opcode::AtomicRMW:
            case Opcode::GetElementPtr:
                printMemoryOps(os);
                break;
            case Opcode::Trunc:
            case Opcode::ZExt:
            case Opcode::SExt:
            case Opcode::FPTrunc:
            case Opcode::FPExt:
            case Opcode::FPToUI:
            case Opcode::FPToSI:
            case Opcode::UIToFP:
            case Opcode::SIToFP:
            case Opcode::PtrToInt:
            case Opcode::PtrToAddr:
            case Opcode::IntToPtr:
            case Opcode::BitCast:
            case Opcode::AddrSpaceCast:
                printConversionOps(os);
                break;
            case Opcode::ICmp:
            case Opcode::FCmp:
            case Opcode::PHI:
            case Opcode::Select:
            case Opcode::Freeze:
            case Opcode::Call:
            case Opcode::VAArg:
            case Opcode::LandingPad:
            case Opcode::CatchPad:
            case Opcode::CleanupPad:
                printOtherOps(os);
                break;
            case Opcode::Ret:
            case Opcode::Br:
            case Opcode::Switch:
            case Opcode::IndirectBr:
            case Opcode::Invoke:
            case Opcode::CallBr:
            case Opcode::Resume:
            case Opcode::CatchSwitch:
            case Opcode::CatchRet:
            case Opcode::CleanupRet:
            case Opcode::Unreachable:
                printTerminator(os);
                break;
            }
        }

        void printAsOperand(std::ostream &os) const override
        {
            os << local(id_);
        }

    private:
        bool isTerminator() const
        {
            switch (opcode_)
            {
            case Opcode::Ret:
            case Opcode::Br:
            case Opcode::Switch:
            case Opcode::IndirectBr:
            case Opcode::Invoke:
            case Opcode::CallBr:
            case Opcode::Resume:
            case Opcode::CatchSwitch:
            case Opcode::CatchRet:
            case Opcode::CleanupRet:
            case Opcode::Unreachable:
                return true;
            default:
                return false;
            }
        }

        void printBinary(std::ostream &os) const
        {
            os << opcodeStr() << " " << getTypeStr() << " ";
            operands_[0]->printAsOperand(os);
            os << ", ";
            operands_[1]->printAsOperand(os);
        }

        void printUnary(std::ostream &os, const std::string &op) const
        {
            os << op << " " << getTypeStr() << " ";
            operands_[0]->printAsOperand(os);
        }

        void printVectorOps(std::ostream &os) const
        {
            os << opcodeStr() << " ";
            operands_[0]->getType()->print(os);
            os << " ";
            operands_[0]->printAsOperand(os);
            for (size_t i = 1; i < operands_.size(); ++i)
            {
                os << ", ";
                operands_[i]->getType()->print(os);
                os << " ";
                operands_[i]->printAsOperand(os);
            }
        }

        void printAggregateOps(std::ostream &os) const
        {
            os << opcodeStr() << " ";
            operands_[0]->getType()->print(os);
            os << " ";
            operands_[0]->printAsOperand(os);
            for (size_t i = 1; i < operands_.size(); ++i)
            {
                os << ", ";
                operands_[i]->getType()->print(os);
                os << " ";
                operands_[i]->printAsOperand(os);
            }
        }

        void printMemoryOps(std::ostream &os) const
        {
            if (opcode_ == Opcode::GetElementPtr)
            {
                Type *elemTy = operands_[0]->getType()->getElementType();
                os << "getelementptr ";
                elemTy->print(os);
                os << ", ";
                operands_[0]->getType()->print(os);
                os << " ";
                operands_[0]->printAsOperand(os);
                for (size_t i = 1; i < operands_.size(); ++i)
                {
                    os << ", ";
                    operands_[i]->getType()->print(os);
                    os << " ";
                    operands_[i]->printAsOperand(os);
                }
            }
            else
            {
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

        void printConversionOps(std::ostream &os) const
        {
            os << opcodeStr() << " ";
            operands_[0]->getType()->print(os);
            os << " ";
            operands_[0]->printAsOperand(os);
            os << " to ";
            type_->print(os);
        }

        std::string getTypeStr() const
        {
            if (type_->isIntegerTy())
                return "i" + std::to_string(type_->getBitWidth());
            if (type_->isHalfTy())
                return "half";
            if (type_->isFloatTy())
                return "float";
            if (type_->isDoubleTy())
                return "double";
            return "unknown";
        }

        std::string opcodeStr() const
        {
            switch (opcode_)
            {
            case Opcode::Add:
                return "add";
            case Opcode::FAdd:
                return "fadd";
            case Opcode::Sub:
                return "sub";
            case Opcode::FSub:
                return "fsub";
            case Opcode::Mul:
                return "mul";
            case Opcode::FMul:
                return "fmul";
            case Opcode::UDiv:
                return "udiv";
            case Opcode::SDiv:
                return "sdiv";
            case Opcode::FDiv:
                return "fdiv";
            case Opcode::URem:
                return "urem";
            case Opcode::SRem:
                return "srem";
            case Opcode::FRem:
                return "frem";
            case Opcode::Shl:
                return "shl";
            case Opcode::LShr:
                return "lshr";
            case Opcode::AShr:
                return "ashr";
            case Opcode::And:
                return "and";
            case Opcode::Or:
                return "or";
            case Opcode::Xor:
                return "xor";
            case Opcode::FNeg:
                return "fneg";
            case Opcode::ExtractElement:
                return "extractelement";
            case Opcode::InsertElement:
                return "insertelement";
            case Opcode::ShuffleVector:
                return "shufflevector";
            case Opcode::ExtractValue:
                return "extractvalue";
            case Opcode::InsertValue:
                return "insertvalue";
            case Opcode::Alloca:
                return "alloca";
            case Opcode::Load:
                return "load";
            case Opcode::Store:
                return "store";
            case Opcode::Fence:
                return "fence";
            case Opcode::CmpXchg:
                return "cmpxchg";
            case Opcode::AtomicRMW:
                return "atomicrmw";
            case Opcode::GetElementPtr:
                return "getelementptr";
            case Opcode::Trunc:
                return "trunc";
            case Opcode::ZExt:
                return "zext";
            case Opcode::SExt:
                return "sext";
            case Opcode::FPTrunc:
                return "fptrunc";
            case Opcode::FPExt:
                return "fpext";
            case Opcode::FPToUI:
                return "fptoui";
            case Opcode::FPToSI:
                return "fptosi";
            case Opcode::UIToFP:
                return "uitofp";
            case Opcode::SIToFP:
                return "sitofp";
            case Opcode::PtrToInt:
                return "ptrtoint";
            case Opcode::PtrToAddr:
                return "ptrtoaddr";
            case Opcode::IntToPtr:
                return "inttoptr";
            case Opcode::BitCast:
                return "bitcast";
            case Opcode::AddrSpaceCast:
                return "addrspacecast";
            case Opcode::ICmp:
                return "icmp";
            case Opcode::FCmp:
                return "fcmp";
            case Opcode::PHI:
                return "phi";
            case Opcode::Select:
                return "select";
            case Opcode::Freeze:
                return "freeze";
            case Opcode::Call:
                return "call";
            case Opcode::VAArg:
                return "va_arg";
            case Opcode::LandingPad:
                return "landingpad";
            case Opcode::CatchPad:
                return "catchpad";
            case Opcode::CleanupPad:
                return "cleanuppad";
            case Opcode::Ret:
                return "ret";
            case Opcode::Br:
                return "br";
            case Opcode::Switch:
                return "switch";
            case Opcode::IndirectBr:
                return "indirectbr";
            case Opcode::Invoke:
                return "invoke";
            case Opcode::CallBr:
                return "callbr";
            case Opcode::Resume:
                return "resume";
            case Opcode::CatchSwitch:
                return "catchswitch";
            case Opcode::CatchRet:
                return "catchret";
            case Opcode::CleanupRet:
                return "cleanupret";
            case Opcode::Unreachable:
                return "unreachable";
            }
            return "unknown";
        }

        std::string icmpPredStr() const
        {
            switch (icmpPred_)
            {
            case ICmpPredicate::EQ:
                return "eq";
            case ICmpPredicate::NE:
                return "ne";
            case ICmpPredicate::UGT:
                return "ugt";
            case ICmpPredicate::UGE:
                return "uge";
            case ICmpPredicate::ULT:
                return "ult";
            case ICmpPredicate::ULE:
                return "ule";
            case ICmpPredicate::SGT:
                return "sgt";
            case ICmpPredicate::SGE:
                return "sge";
            case ICmpPredicate::SLT:
                return "slt";
            case ICmpPredicate::SLE:
                return "sle";
            }
            return "eq";
        }

        Opcode opcode_;
        std::vector<Value *> operands_;
        unsigned id_;
        inline static unsigned nextId_ = 0;
        ICmpPredicate icmpPred_;
        BasicBlock *branchTarget_ = nullptr;
        Value *cond_ = nullptr;
        BasicBlock *thenBB_ = nullptr;
        BasicBlock *elseBB_ = nullptr;
        std::vector<Value *> phiValues_;
        std::vector<BasicBlock *> phiBlocks_;
    };

}