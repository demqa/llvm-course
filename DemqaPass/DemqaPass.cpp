#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include <ios>
#include <llvm/ADT/StringRef.h>
#include <ostream>
#include <fstream>
#include <system_error>

using namespace llvm;

const char dumpFilename[] = "dump";

namespace {
  struct DemqaPass : public FunctionPass {
private:
    std::error_code error_code;
    llvm::raw_fd_ostream dumpFile{dumpFilename, error_code};

public:
    static char ID;
    DemqaPass() : FunctionPass(ID) {
        dumpFile << "digraph structs {\n";
        dumpFile << "node [color=black, shape=box, style=\"filled\"]\n";
        dumpFile << "rankdir=UD;\n";
    }

   ~DemqaPass() {
       dumpFile << "}\n";
       dumpFile.close();

       system((std::string("dot ") + dumpFilename + " -T png -o " + dumpFilename + ".png").c_str());
       system((std::string("gwenview ") + dumpFilename + ".png").c_str());
    }

    bool runOnFunction(Function &F) override {
      static int launch = 0;

      dumpFile << "func"; dumpFile.write_hex(size_t(&F)); dumpFile << " ";
      dumpFile << "[fillcolor=\"white\"," << "label=\"\n";
      F.print(dumpFile);
      dumpFile << "\"" << "];\n";

      // Dump function uses
      for (auto &U : F.uses()) {
          User *user = U.getUser();
          // outs() << "[DOT] " << (uint64_t)(&F) << " -> " << (uint64_t)user << "\n";
          outs() << "User:  " << (uint64_t)user;
          user->print(llvm::outs(), true);
          outs() << "\n";
      }

      for (auto &B : F) {
          for (auto &I : B) {
              // Dump Instructions
              outs() << "Instruction: " << (uint64_t)(&I) << "\n";
              I.print(outs(), true);
              outs() << "\n";

              // Dump instruction uses
              for (auto &U : I.uses()) {
                  User *user = U.getUser();
                  // outs() << "[DOT] " << (uint64_t)(&I) << " -> " << (uint64_t)user << "\n     ";
                  outs() << "  User:  " << (uint64_t)user << "\n";
                  // user->print(outs(), true);
                  // outs() << "\n";
              }
          }
      }

      outs() << "\n"
                "-------------FUNCTION END-------------\n";

      return false;
    }
  };
}

char DemqaPass::ID = 0;
static RegisterPass<DemqaPass> X("demqa", "Simple Demqa Pass");
