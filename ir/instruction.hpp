#pragma once
#include <ostream>
#include <ir/value.hpp>

namespace anvil::ir
{

    class Instruction : public Value
    {
    public:
        enum class Opcode
        {
            Add,
            Ret
        };

        Instruction(Type *type, Opcode op)
            : Value(type), opcode_(op), id_(nextId_++) {}

        Opcode getOpcode() const { return opcode_; }
        unsigned getId() const { return id_; }

        void print(std::ostream &os) const
        {
            os << "%" << id_ << " = ";
            switch (opcode_)
            {
            case Opcode::Add:
                os << "add ";
                break;
            case Opcode::Ret:
                os << "ret ";
                break;
            }
            type_->print(os);
            os << "\n";
        }

    private:
        Opcode opcode_;
        unsigned id_;
        inline static unsigned nextId_ = 0;
    };

}