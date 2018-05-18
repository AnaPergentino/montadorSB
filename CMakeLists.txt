cmake_minimum_required(VERSION 3.8)
project(T1_SB)

set(CMAKE_CXX_STANDARD 11)
if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG=1)
endif()

set(CMAKE_C_FLAGS_DEBUG '-debug')
set(CMAKE_C_FLAGS '-debug')
set(SOURCE_FILES main.cpp #[[Tokenizer.cpp Tokenizer.h]] CommandOperations.cpp CommandOperations.h LabelParse.cpp LabelParse.h MacroLibrary.cpp MacroLibrary.h InstructionInfo.cpp InstructionInfo.h)
add_executable(T1_SB ${SOURCE_FILES})