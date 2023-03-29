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

    std::vector<std::pair<User *, User *>> storage;

public:
    static char ID;
    DemqaPass() : FunctionPass(ID) {
        dumpFile << "digraph structs {\n";
        dumpFile << "node [color=white, shape=box, style=\"filled\"]\n";
        dumpFile << "edge [style=bold, constraint=false]\n";
        dumpFile << "rankdir=UD;\n";
    }

    ~DemqaPass() {
        for (auto x: storage) {
            printUserName(x.first); dumpFile << " -> "; printUserName(x.second); dumpFile << ";\n";
        }

        dumpFile << "}\n";
        dumpFile.close();

        system((std::string("dot ") + dumpFilename + " -T png -o " + dumpFilename + ".png").c_str());
        system((std::string("gwenview ") + dumpFilename + ".png").c_str());
     }

    void printUserName(User* u) {
        dumpFile << "func"; dumpFile.write_hex(size_t(u)); dumpFile << " ";
    }

    bool runOnFunction(Function &F) override {
      static int launch = 0;

      // Algorithm of printing static infomation of call graph:
      //
      // every function prints their body, that way that every instrunction will have
      // their own block in dot file (because only instrunction can be a user of function),
      // and users of instruction are instructions too.
      //
      // it will be function dumpFunction.

      // printUserName(&F);
      // dumpFile << "[fillcolor=\"white\"," << "label=\"";
      // // F.print(dumpFile);
      // dumpFile << F.getName();
      // dumpFile << "\"];\n";

      // Dump function uses
      // for (auto &U : F.uses()) {
      //     User *user = U.getUser();

      //     storage.push_back({user, &F});
      // }

      outs() << "In a function called " << F.getName() << " at " << uint64_t(&F) << "\n";
      F.print(llvm::outs());
      outs() << "\n";

      // Dump function uses
      for (auto &U : F.uses()) {
          User *user = U.getUser();
          outs() << "[DOT] " << (uint64_t)user << " -> " << (uint64_t)(&F) << "\n";
          // ----------this user is call instruction---------
          outs() << "    User:  " << (uint64_t)user;
          user->print(llvm::outs(), true);
          outs() << "\n";
      }


      for (auto &B : F) {
          for (auto &I : B) {
              // Dump Instructions
              // outs() << "Instruction: " << (uint64_t)(&I) << "\n";
              // I.print(outs(), true);
              // outs() << "\n";

              // Dump instruction uses
              // for (auto &U : I.uses()) {
              //     User *user = U.getUser();
                  // outs() << "[DOT] " << (uint64_t)(&I) << " -> " << (uint64_t)user << "\n     ";
                  // outs() << "  User:  " << (uint64_t)user << "\n";
                  // user->print(outs(), true);
                  // outs() << "\n";
              // }
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
