#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
  struct DemqaPass : public FunctionPass {
    static char ID;
    DemqaPass() : FunctionPass(ID) {}

    bool runOnFunction(Function &F) override {

      errs() << "\n"
                "------------FUNCTION START------------\n";
      errs() << "NAME: " << F.getName() << '\n';
      errs() << "------------FUNCTION BOODY------------\n\n";

      for (auto& B: F) {
          errs() << B.getName() << '\n';
          for (auto& I: B) {
              errs() << I << '\n';
          }
      }

      errs() << "\n"
                "-------------FUNCTION END-------------\n";

      return false;
    }
  };
}

char DemqaPass::ID = 0;
static RegisterPass<DemqaPass> X("demqa", "Simple Demqa Pass");
