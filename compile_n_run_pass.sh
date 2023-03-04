# This file must be located it llvm build directory
# Build directory must contain opt & clang

name="${1%.*}"

./bin/clang -c -emit-llvm $1
./bin/opt -enable-new-pm=0 -load lib/LLVMOurPass.so -demqa < $name.bc > /dev/null

