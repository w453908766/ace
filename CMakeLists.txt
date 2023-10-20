
cmake_minimum_required(VERSION 3.2)

PROJECT(aceBuilder)
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

set(CMAKE_CXX_COMPILER g++)

add_compile_options(
  -std=c++14   
  -fno-rtti 
  -D_GNU_SOURCE 
  -D__STDC_CONSTANT_MACROS 
  -D__STDC_FORMAT_MACROS 
  -D__STDC_LIMIT_MACROS
  -DLLVM_DISABLE_ABI_BREAKING_CHECKS_ENFORCING
  -fPIC -fno-exceptions -g -Og
)

include_directories(/home/wang/Pictures/clang+llvm-15.0.7/include)
link_directories(/home/wang/Pictures/clang+llvm-15.0.7/lib)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(LIB_TYPE STATIC)
else()
  set(LIB_TYPE SHARED)
endif()

add_executable(aceBuilder main.cpp cc1_main.cpp PushArgs.cpp)



target_link_libraries(aceBuilder
  LLVMX86TargetMCA
  LLVMX86Disassembler
  LLVMX86AsmParser
  LLVMX86CodeGen
  LLVMX86Desc
  LLVMX86Info
)

target_link_libraries(aceBuilder
  clangBasic
  clangDriver
  clangFrontend
  clangFrontendTool
  clangSerialization
  clangTooling
  clangCodeGen
  clangExtractAPI
  clangRewriteFrontend
  clangARCMigrate
  clangStaticAnalyzerFrontend
  clangStaticAnalyzerCheckers
  clangStaticAnalyzerCore
  clangCrossTU
  clangIndex
  clangFrontend
  clangDriver
  clangParse
  clangSerialization
  clangSema
  clangAnalysis
  clangASTMatchers
  clangEdit
  clangSupport
  clangFormat
  clangToolingInclusions
  clangAST
  clangToolingCore
  clangRewrite
  clangLex
  clangBasic
 
  LLVMAnalysis
  LLVMCodeGen
  LLVMCore
  LLVMipo
  LLVMAggressiveInstCombine
  LLVMInstCombine
  LLVMInstrumentation
  LLVMMC
  LLVMMCParser
  LLVMObjCARCOpts
  LLVMOption
  LLVMScalarOpts
  LLVMSupport
  LLVMTransformUtils
  LLVMVectorize
  LLVMAsmPrinter
  LLVMGlobalISel
  LLVMSelectionDAG
  LLVMCFGuard
  LLVMMCDisassembler
  LLVMCoverage
  LLVMLTO
  LLVMCodeGen
  LLVMExtensions
  LLVMPasses
  LLVMObjCARCOpts
  LLVMTarget
  LLVMCoroutines
  LLVMipo
  LLVMInstrumentation
  LLVMVectorize
  LLVMBitWriter
  LLVMIRReader
  LLVMAsmParser
  LLVMLinker
  LLVMWindowsDriver
  LLVMOption
  LLVMFrontendOpenMP
  LLVMScalarOpts
  LLVMAggressiveInstCombine
  LLVMInstCombine
  LLVMTransformUtils
  LLVMAnalysis
  LLVMProfileData
  LLVMSymbolize
  LLVMDebugInfoDWARF
  LLVMDebugInfoPDB
  LLVMDebugInfoMSF
  LLVMObject
  LLVMMCParser
  LLVMMC
  LLVMDebugInfoCodeView
  LLVMBitReader
  LLVMTextAPI
  LLVMCore
  LLVMBinaryFormat
  LLVMRemarks
  LLVMBitstreamReader
  LLVMSupport
  LLVMDemangle
  LLVMMIRParser
  
  rt
  dl
  pthread
  m
  z
  tinfo
)