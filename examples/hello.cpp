#include <anvil.hpp>
#include <fstream>

using namespace anvil::ir;

int main()
{
    Context ctx;
    Module mod("test");

    auto *i32 = ctx.getIntType(32);
    auto *fn = new Function(i32, "main");

    BasicBlock *entry = new BasicBlock("entry");
    IRBuilder builder(entry);

    auto *const1 = new ConstantInt(i32, 42);
    auto *const2 = new ConstantInt(i32, 58);

    Instruction *value = builder.CreateAdd(const1, const2);
    builder.CreateRet(value);

    fn->addBlock(entry);
    mod.addFunction(fn);

    std::ofstream out(mod.name() + ".ll");
    if (!out)
        return 1;

    mod.print(out);

    return 0;
}