#pragma once

#include <ir/function.hpp>

namespace anvil::ir
{

    class Module
    {
    public:
        explicit Module(std::string name)
            : name_(std::move(name)) {}

        void addFunction(Function *fn)
        {
            functions_.push_back(fn);
        }

        void print(std::ostream &os) const
        {
            os << "; ModuleID = '" << name_ << "'\n";
            for (auto *fn : functions_)
                fn->print(os);
        }

    private:
        std::string name_;
        std::vector<Function *> functions_;
    };
}
