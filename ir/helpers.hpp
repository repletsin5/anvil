#pragma once

#include <string>

namespace anvil::ir
{
    inline std::string local(unsigned id) { 
        return "%" + std::to_string(id); 
    }

    inline std::string global(const std::string &name) { 
        return "@" + name; 
    }
}
