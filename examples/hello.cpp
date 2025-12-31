#include <anvil.hpp>
#include <fstream>

using namespace anvil::ir;

int main()
{
    Context ctx;
    Module mod("nested_match");

    auto mainFn = std::make_unique<Function>(ctx.getInt32Ty(), "main");

    auto entry = std::make_unique<BasicBlock>("entry");
    IRBuilder builder(&ctx, entry.get());

    auto x = std::make_unique<ConstantInt>(ctx.getInt32Ty(), 1);
    auto y = std::make_unique<ConstantInt>(ctx.getInt32Ty(), 2);

    auto one = std::make_unique<ConstantInt>(ctx.getInt32Ty(), 1);
    auto two = std::make_unique<ConstantInt>(ctx.getInt32Ty(), 2);

    Instruction* cmpX =
        builder.CreateICmp(Instruction::ICmpPredicate::EQ, x.get(), one.get());

    auto x_then = std::make_unique<BasicBlock>("x.then");
    auto x_else = std::make_unique<BasicBlock>("x.else");
    auto x_merge = std::make_unique<BasicBlock>("x.merge");

    builder.CreateCondBr(cmpX, x_then.get(), x_else.get());

    IRBuilder xThenBuilder(&ctx, x_then.get());

    Instruction* cmpY =
        xThenBuilder.CreateICmp(Instruction::ICmpPredicate::EQ, y.get(), two.get());

    auto y_then = std::make_unique<BasicBlock>("y.then");
    auto y_else = std::make_unique<BasicBlock>("y.else");
    auto y_merge = std::make_unique<BasicBlock>("y.merge");

    xThenBuilder.CreateCondBr(cmpY, y_then.get(), y_else.get());

    IRBuilder yThenBuilder(&ctx, y_then.get());
    auto v10 = std::make_unique<ConstantInt>(ctx.getInt32Ty(), 10);
    yThenBuilder.CreateBr(y_merge.get());

    IRBuilder yElseBuilder(&ctx, y_else.get());
    auto v20 = std::make_unique<ConstantInt>(ctx.getInt32Ty(), 20);
    yElseBuilder.CreateBr(y_merge.get());

    IRBuilder yMergeBuilder(&ctx, y_merge.get());
    Instruction* yPhi = yMergeBuilder.CreatePHI(ctx.getInt32Ty());
    yPhi->addIncoming(v10.get(), y_then.get());
    yPhi->addIncoming(v20.get(), y_else.get());
    yMergeBuilder.CreateBr(x_merge.get());

    IRBuilder xElseBuilder(&ctx, x_else.get());
    auto v30 = std::make_unique<ConstantInt>(ctx.getInt32Ty(), 30);
    xElseBuilder.CreateBr(x_merge.get());

    IRBuilder xMergeBuilder(&ctx, x_merge.get());
    Instruction* xPhi = xMergeBuilder.CreatePHI(ctx.getInt32Ty());
    xPhi->addIncoming(yPhi, y_merge.get());
    xPhi->addIncoming(v30.get(), x_else.get());
    xMergeBuilder.CreateRet(xPhi);

    mainFn->addBlock(std::move(entry));
    mainFn->addBlock(std::move(x_then));
    mainFn->addBlock(std::move(x_else));
    mainFn->addBlock(std::move(y_then));
    mainFn->addBlock(std::move(y_else));
    mainFn->addBlock(std::move(y_merge));
    mainFn->addBlock(std::move(x_merge));

    mod.addFunction(std::move(mainFn));

    std::ofstream out(mod.name() + ".ll");
    mod.print(out);
}
