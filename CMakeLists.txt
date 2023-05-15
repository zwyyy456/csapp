cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_C_STANDARD 17)
project(asms VERSION 0.1.0)

include(CTest)
enable_testing()

# find_package(Threads REQUIRED) # 查找 pthread 对应的库

include_directories(inc) # 添加头文件文件夹 inc

aux_source_directory(src/common Com)
aux_source_directory(src/hardware/cpu Cpu)
aux_source_directory(src/hardware/memory Mem)

# 将这些源文件编译成一个函数
add_executable(asms main_hardware.c ${SOURCES} ${Com} ${Cpu} ${Mem})

# target_link_libraries(asms Threads::Threads)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)




