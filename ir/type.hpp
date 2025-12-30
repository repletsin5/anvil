#pragma once

#include <ostream>
#include <cstdint>

namespace anvil::ir
{
    class Context;

    class Type
    {
        friend class Context;

    public:
        enum class Kind
        {
            Void,
            Integer,
            Pointer,
            Function,
            Struct,
            Array,
            Vector,
            Half,
            Float,
            Double
        };

        Kind getKind() const { return kind_; }
        bool isVoidTy() const { return kind_ == Kind::Void; }
        bool isIntegerTy() const { return kind_ == Kind::Integer; }
        bool isIntegerTy(unsigned bits) const { return isIntegerTy() && bitWidth_ == bits; }
        bool isPointerTy() const { return kind_ == Kind::Pointer; }
        bool isFunctionTy() const { return kind_ == Kind::Function; }
        bool isStructTy() const { return kind_ == Kind::Struct; }
        bool isArrayTy() const { return kind_ == Kind::Array; }
        bool isVectorTy() const { return kind_ == Kind::Vector; }

        bool isHalfTy() const { return kind_ == Kind::Half; }
        bool isFloatTy() const { return kind_ == Kind::Float; }
        bool isDoubleTy() const { return kind_ == Kind::Double; }

        bool isIEEELikeFPTy() const { return isHalfTy() || isFloatTy() || isDoubleTy(); }
        bool isFloatingPointTy() const { return isIEEELikeFPTy(); }

        unsigned getBitWidth() const { return bitWidth_; }
        Type *getElementType() const { return elementType_; }
        Type *getReturnType() const { return returnType_; }
        const std::vector<Type *> &getParamTypes() const { return paramTypes_; }
        const std::vector<Type *> &getStructElements() const { return structElements_; }
        unsigned getNumElements() const { return numElements_; }

        void print(std::ostream &os) const
        {
            switch (kind_)
            {
            case Kind::Void:
                os << "void";
                break;
            case Kind::Integer:
                os << "i" << bitWidth_;
                break;
            case Kind::Half:
                os << "half";
                break;
            case Kind::Float:
                os << "float";
                break;
            case Kind::Double:
                os << "double";
                break;
            case Kind::Pointer:
                elementType_->print(os);
                os << "*";
                break;
            case Kind::Array:
                os << "[" << numElements_ << " x ";
                elementType_->print(os);
                os << "]";
                break;
            case Kind::Vector:
                os << "<" << numElements_ << " x ";
                elementType_->print(os);
                os << ">";
                break;
            case Kind::Struct:
                os << "{ ";
                for (size_t i = 0; i < structElements_.size(); ++i)
                {
                    if (i)
                        os << ", ";
                    structElements_[i]->print(os);
                }
                os << " }";
                break;
            case Kind::Function:
                returnType_->print(os);
                os << "(";
                for (size_t i = 0; i < paramTypes_.size(); ++i)
                {
                    if (i)
                        os << ", ";
                    paramTypes_[i]->print(os);
                }
                os << ")";
                break;
            }
        }

    private:
        explicit Type(Kind k, unsigned bits = 0) : kind_(k), bitWidth_(bits) {}
        explicit Type(Type *elem) : kind_(Kind::Pointer), elementType_(elem) {}
        explicit Type(Type *ret, const std::vector<Type *> &params)
            : kind_(Kind::Function), returnType_(ret), paramTypes_(params) {}
        explicit Type(const std::vector<Type *> &elems)
            : kind_(Kind::Struct), structElements_(elems) {}
        explicit Type(Type *elem, unsigned num, Kind k)
            : kind_(k), elementType_(elem), numElements_(num) {}

        Kind kind_;
        unsigned bitWidth_ = 0;
        Type *elementType_ = nullptr;
        unsigned numElements_ = 0;
        Type *returnType_ = nullptr;
        std::vector<Type *> paramTypes_;
        std::vector<Type *> structElements_;
    };
}