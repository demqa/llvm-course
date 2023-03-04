add_llvm_library( LLVMOurPass MODULE
  DemqaPass.cpp

  DEPENDS
  PLUGIN_TOOL
  opt
  )
