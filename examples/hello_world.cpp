#include <anvil.hpp>
#include <fstream>

using namespace anvil::ir;

int main()
{
    Context ctx;
    Module mod("hello_world");

    FunctionType *funcTy = ctx.getFunctionTy(ctx.getInt32Ty(), {});
    
    auto mainFn = std::make_unique<Function>(funcTy, "main");
    auto entry = std::make_unique<BasicBlock>(ctx.getLabelTy(), "entry", mainFn.get());

    IRBuilder builder(&ctx, entry.get());

    auto zero = std::make_unique<ConstantInt>(ctx.getInt32Ty(), 0);
    builder.CreateRet(zero.get());

    mainFn->addBlock(std::move(entry));
    mod.addFunction(std::move(mainFn));
 
    std::ofstream out(mod.name() + ".ll");
    mod.print(out);
}