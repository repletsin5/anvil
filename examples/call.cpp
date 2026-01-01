//! Currently doesn't work

#include <anvil.hpp>
#include <fstream>

using namespace anvil::ir;

int main()
{
    Context ctx;
    Module mod("call");

    Type *i32 = ctx.getInt32Ty();

    auto addFn = std::make_unique<Function>(i32, "add", std::vector<Type *>{i32, i32});
    Function *addFnRaw = addFn.get();

    auto addEntry = std::make_unique<BasicBlock>("entry");
    IRBuilder addBuilder(&ctx, addEntry.get());

    Parameter *a = addFnRaw->getParam(0);
    Parameter *b = addFnRaw->getParam(1);

    Instruction *sum = addBuilder.CreateAdd(a, b);
    addBuilder.CreateRet(sum);

    addFn->addBlock(std::move(addEntry));
    mod.addFunction(std::move(addFn));

    auto mainFn = std::make_unique<Function>(i32, "main");
    auto entry = std::make_unique<BasicBlock>("entry");
    IRBuilder builder(&ctx, entry.get());

    auto x = std::make_unique<ConstantInt>(i32, 1);
    auto y = std::make_unique<ConstantInt>(i32, 2);

    auto one = std::make_unique<ConstantInt>(i32, 1);
    auto two = std::make_unique<ConstantInt>(i32, 2);

    Instruction *cmpX = builder.CreateICmp(Instruction::ICmpPredicate::EQ, x.get(), one.get());

    auto x_then = std::make_unique<BasicBlock>("x.then");
    auto x_else = std::make_unique<BasicBlock>("x.else");
    auto x_merge = std::make_unique<BasicBlock>("x.merge");

    builder.CreateCondBr(cmpX, x_then.get(), x_else.get());

    IRBuilder xThenBuilder(&ctx, x_then.get());
    Instruction *cmpY = xThenBuilder.CreateICmp(Instruction::ICmpPredicate::EQ, y.get(), two.get());

    auto y_then = std::make_unique<BasicBlock>("y.then");
    auto y_else = std::make_unique<BasicBlock>("y.else");
    auto y_merge = std::make_unique<BasicBlock>("y.merge");

    xThenBuilder.CreateCondBr(cmpY, y_then.get(), y_else.get());

    IRBuilder yThenBuilder(&ctx, y_then.get());
    Instruction *callAdd1 = yThenBuilder.CreateCall(addFnRaw, {x.get(), y.get()});
    yThenBuilder.CreateBr(y_merge.get());

    IRBuilder yElseBuilder(&ctx, y_else.get());
    auto v10 = std::make_unique<ConstantInt>(i32, 10);
    Instruction *callAdd2 = yElseBuilder.CreateCall(addFnRaw, {x.get(), v10.get()});
    yElseBuilder.CreateBr(y_merge.get());

    IRBuilder yMergeBuilder(&ctx, y_merge.get());
    Instruction *yPhi = yMergeBuilder.CreatePHI(i32);
    yPhi->addIncoming(callAdd1, y_then.get());
    yPhi->addIncoming(callAdd2, y_else.get());
    yMergeBuilder.CreateBr(x_merge.get());

    IRBuilder xElseBuilder(&ctx, x_else.get());
    auto v20 = std::make_unique<ConstantInt>(i32, 20);
    Instruction *callAdd3 = xElseBuilder.CreateCall(addFnRaw, {y.get(), v20.get()});
    xElseBuilder.CreateBr(x_merge.get());

    IRBuilder xMergeBuilder(&ctx, x_merge.get());
    Instruction *xPhi = xMergeBuilder.CreatePHI(i32);
    xPhi->addIncoming(yPhi, y_merge.get());
    xPhi->addIncoming(callAdd3, x_else.get());
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
