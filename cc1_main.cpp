
#include <vector>
#include <string>
#include <memory>

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

using namespace std;
using namespace llvm;
using namespace clang;

unique_ptr<CodeGenAction> makeAction(CompilerInstance &Compiler){
  if(Compiler.getFrontendOpts().ProgramAction == frontend::EmitLLVM){
    return make_unique<EmitLLVMAction>();
  } else {
    return make_unique<EmitObjAction>();
  }
}

bool runInvocation(
    std::unique_ptr<CompilerInvocation> Invocation, FileManager *Files,
    DiagnosticConsumer *DiagConsumer) {

  std::shared_ptr<PCHContainerOperations> PCHContainerOps =
                     std::make_shared<PCHContainerOperations>();

  // Create a compiler instance to handle the actual work.
  CompilerInstance Compiler(std::move(PCHContainerOps));
  Compiler.setInvocation(std::move(Invocation));
  Compiler.setFileManager(Files);
  Compiler.createDiagnostics(DiagConsumer, /*ShouldOwnClient=*/false);
  Compiler.createSourceManager(*Files);

  unique_ptr<CodeGenAction> Action = makeAction(Compiler);
  const bool Success = Compiler.ExecuteAction(*Action);
 
  Files->clearStatCache();
  return Success;
}

void cc1_main(
  const char *BinaryName, FileManager *Files, 
  DiagnosticsEngine* Diagnostics, const opt::ArgStringList &Args){
 
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmPrinters();
  llvm::InitializeAllAsmParsers();

  std::unique_ptr<CompilerInvocation> Invocation(
      tooling::newInvocation(Diagnostics, Args, BinaryName));

  CodeGenOptions &CodeGenOpts = Invocation->getCodeGenOpts();
  CodeGenOpts.OptimizationLevel = std::max(CodeGenOpts.OptimizationLevel, (unsigned)1);
  LangOptions *LangOpts = Invocation->getLangOpts();
  LangOpts->Optimize = std::max(LangOpts->Optimize, (unsigned)1);

  runInvocation(std::move(Invocation), Files, nullptr);
}
