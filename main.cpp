
#include <string>
#include <memory>

#include "clang/Driver/Driver.h"
#include "clang/Driver/Compilation.h"
#include "clang/Driver/Job.h"

#include "clang/Tooling/Tooling.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/DiagnosticIDs.h"
#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/FileSystemOptions.h"
#include "clang/Basic/LLVM.h"
#include "clang/Frontend/ASTUnit.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/CompilerInvocation.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/CodeGen/CodeGenAction.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/CodeGen/BackendUtil.h"
#include "clang/Frontend/FrontendOptions.h"
#include "clang/Frontend/MultiplexConsumer.h"

#include "llvm/Option/ArgList.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/InitLLVM.h"

using namespace std;
using namespace llvm;
using namespace clang;

void cc1_main(
  const char *BinaryName, FileManager *Files, 
  DiagnosticsEngine* Diagnostics, const opt::ArgStringList &Args);
 
static driver::Driver *
newDriver(DiagnosticsEngine *Diagnostics, const char *BinaryName,
          IntrusiveRefCntPtr<llvm::vfs::FileSystem> VFS){
  driver::Driver *CompilerDriver =
      new driver::Driver(BinaryName, llvm::sys::getDefaultTargetTriple(),
                         *Diagnostics, "clang LLVM compiler", std::move(VFS));
  CompilerDriver->setTitle("clang_based_tool");
  return CompilerDriver;
}

void ExecuteJobs(
  const char *BinaryName, FileManager *Files, 
  DiagnosticsEngine* Diagnostics, const driver::JobList &Jobs){
  for (driver::Command Job : Jobs) {
//    Job.Print(llvm::outs(), "\n", false);
    const opt::ArgStringList &Args = Job.getArguments();

    if(!StringRef(Job.getExecutable()).equals(BinaryName)){
      std::string Error;
      bool ExecutionFailed;
      std::vector<Optional<StringRef>> Redirects;
      int Res = Job.Execute(Redirects, &Error, &ExecutionFailed);
    } else if(Args.size()>=1 && StringRef(Args[0]).equals("-cc1")) {
      cc1_main(BinaryName, Files, Diagnostics, Args);
    } else {
      llvm_unreachable("never handle tool");
    }
  }
}

bool run(SmallVector<const char *> &Argv){
  const char *BinaryName = Argv[0];

  IntrusiveRefCntPtr<FileManager> Files(
      new FileManager(FileSystemOptions()));

  IntrusiveRefCntPtr<DiagnosticOptions> DiagOpts = CreateAndPopulateDiagOpts(Argv);
  TextDiagnosticPrinter DiagnosticPrinter(llvm::errs(), DiagOpts.get());

  IntrusiveRefCntPtr<DiagnosticsEngine> Diagnostics =
      CompilerInstance::createDiagnostics(
        DiagOpts.get(), &DiagnosticPrinter, false);

  const std::unique_ptr<driver::Driver> Driver(
      newDriver(Diagnostics.get(), BinaryName, &Files->getVirtualFileSystem()));

  if (!Files->getFileSystemOpts().WorkingDir.empty())
    Driver->setCheckInputsExist(false);
  const std::unique_ptr<driver::Compilation> Compilation(
      Driver->BuildCompilation(Argv));

  if (!Compilation)
    return false;

  ExecuteJobs(BinaryName, Files.get(), Diagnostics.get(), Compilation->getJobs());
  return true;
}

void pushArgs(SmallVector<const char *>& Args, int argc, const char **argv);

int main(int argc, const char **argv){
  InitLLVM X(argc, argv);
  SmallVector<const char *> Args;
  pushArgs(Args, argc, argv);
  run(Args);
  return 0;
}