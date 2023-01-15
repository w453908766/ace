
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

SmallString<128> plugin;

void pushArgs(SmallVector<const char *>& Args, int argc, const char **argv){
  for (int i = 0; i < argc; i++){
    Args.push_back(argv[i]);
  }

//  SmallString<128> pluginPath(argv[0]);
//  sys::path::remove_filename(pluginPath);
//  sys::path::append(pluginPath, "libExetrackBuilder.so");
//  plugin.append("-fpass-plugin=");
//  plugin.append(pluginPath);
//  Args.push_back(plugin.c_str());
}

