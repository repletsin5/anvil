#pragma once

#include <vector>
#include <ir/instruction.hpp>
#include <ir/basic_block.hpp>
#include <ir/context.hpp>

namespace anvil::ir
{

    class IRBuilder
    {
    public:
        IRBuilder(Context *ctx, BasicBlock *bb)
            : ctx_(ctx), block_(bb) {}

        Instruction *CreateAdd(Value *lhs, Value *rhs)
        {
            auto inst = std::make_unique<Instruction>(lhs->getType(), Instruction::Opcode::Add, std::vector<Value *>{lhs, rhs});
            Instruction *ptr = inst.get();
            block_->addInstruction(std::move(inst));
            return ptr;
        }

        Instruction *CreateICmp(Instruction::ICmpPredicate pred, Value *lhs, Value *rhs)
        {
            auto inst = std::make_unique<Instruction>(ctx_->getInt1Ty(), Instruction::Opcode::ICmp, std::vector<Value *>{lhs, rhs});
            inst->setPredicate(pred);
            Instruction *ptr = inst.get();
            block_->addInstruction(std::move(inst));
            return ptr;
        }

        Instruction *CreateBr(BasicBlock *dest)
        {
            auto inst = std::make_unique<Instruction>(dest);
            Instruction *ptr = inst.get();
            block_->addInstruction(std::move(inst));
            return ptr;
        }

        Instruction *CreateCondBr(Value *cond, BasicBlock *thenBB, BasicBlock *elseBB)
        {
            auto inst = std::make_unique<Instruction>(cond, thenBB, elseBB);
            Instruction *ptr = inst.get();
            block_->addInstruction(std::move(inst));
            return ptr;
        }

        Instruction *CreatePHI(Type *type)
        {
            auto inst = std::make_unique<Instruction>(type, Instruction::Opcode::PHI);
            Instruction *ptr = inst.get();
            block_->addInstruction(std::move(inst));
            return ptr;
        }

        void AddIncoming(Instruction *phi, Value *val, BasicBlock *bb)
        {
            phi->addIncoming(val, bb);
        }

        Instruction *CreateRet(Value *val)
        {
            auto inst = std::make_unique<Instruction>(val->getType(), Instruction::Opcode::Ret, std::vector<Value *>{val});
            Instruction *ptr = inst.get();
            block_->addInstruction(std::move(inst));
            return ptr;
        }

        Instruction *CreateGEP(Value *base, const std::vector<Value *> &indices)
        {
            std::vector<Value *> ops;
            ops.push_back(base);
            ops.insert(ops.end(), indices.begin(), indices.end());
            Type *elemTy = base->getType()->getElementType();
            Type *ptrTy = ctx_->getPointerType(elemTy);
            auto inst = std::make_unique<Instruction>(ptrTy, Instruction::Opcode::GetElementPtr, ops);
            Instruction *ptr = inst.get();
            block_->addInstruction(std::move(inst));
            return ptr;
        }

        void SetInsertPoint(BasicBlock *bb)
        {
            block_ = bb;
        }

    private:
        Context *ctx_;
        BasicBlock *block_;
    };
}