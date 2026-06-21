cmake_minimum_required(VERSION 3.16)

if(NOT COMMAND UT_addEachCXX_asTestExe)
    message(FATAL_ERROR "UT_addEachCXX_asTestExe() must be defined by the parent CMakeLists.txt")
endif()

UT_addEachCXX_asTestExe()
