cmake_minimum_required(VERSION 3.4.1)

##用cmake命令将CMakeLists.txt文件转化为make所需要的makefile文件##

aux_source_directory(. dir_lib_srcs)                          #查找当前目录下的所有源文件,将所有源文件的文件名赋值给变量DIR_LIB_SRCS

add_library(sub SHARED ${dir_lib_srcs})                       # 生成动态链接库
