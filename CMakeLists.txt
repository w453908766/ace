
cmake_minimum_required(VERSION 3.2)

PROJECT(aceBuilder)
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

set(CMAKE_CXX_COMPILER g++)

INCLUDE_DIRECTORIES(include)
INCLUDE_DIRECTORIES(/usr/local/include)

add_compile_options(
    -std=c++14   
    -fno-rtti 
    -D_GNU_SOURCE 
    -D__STDC_CONSTANT_MACROS 
    -D__STDC_FORMAT_MACROS 
    -D__STDC_LIMIT_MACROS
    -fPIC -fno-exceptions -g -Og
)


link_directories(/home/wang/Pictures/Serialization/build)

add_executable(aceBuilder main.cpp cc1_main.cpp PushArgs.cpp)

target_link_libraries(aceBuilder)

target_link_libraries(aceBuilder
    clangAnalysis
    clangLex
    clangAST 
    clangBasic
    clangFrontend 
    clangTooling 
    clangDriver
    clangCodeGen
    clangSerialization
 

    LLVMSupport 
    LLVMOption
    LLVMCore

    LLVMAnalysis
    LLVMBitReader
    LLVMBitWriter
    LLVMCore
    LLVMCoroutines
    LLVMCoverage
    LLVMDemangle
    LLVMExtensions
    LLVMFrontendOpenMP
    LLVMipo
    LLVMIRReader
    LLVMAggressiveInstCombine
    LLVMInstCombine
    LLVMInstrumentation
    LLVMLTO
    LLVMLinker
    LLVMMC
    LLVMObjCARCOpts
    LLVMObject
    LLVMPasses
    LLVMProfileData
    LLVMRemarks
    LLVMScalarOpts
    LLVMSupport
    LLVMTarget
    LLVMTransformUtils

    LLVMX86TargetMCA 
    LLVMX86Disassembler 
    LLVMX86AsmParser 
    LLVMX86CodeGen 
    LLVMX86Desc 
    LLVMX86Info 
)

