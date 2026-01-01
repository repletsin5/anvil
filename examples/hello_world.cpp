#include <anvil.hpp> 
#include <fstream>

using namespace anvil::ir;

int main()
{
    Context ctx;
    Module mod("hello_world");

    auto mainFn = std::make_unique<Function>(ctx.getInt32Ty(), "main");
    auto entry = std::make_unique<BasicBlock>("entry");

    IRBuilder builder(&ctx, entry.get());

    auto zero = std::make_unique<ConstantInt>(ctx.getInt32Ty(), 0);
    builder.CreateRet(zero.get());

    mainFn->addBlock(std::move(entry));
    mod.addFunction(std::move(mainFn));

    std::ofstream out(mod.name() + ".ll");
    mod.print(out);
}