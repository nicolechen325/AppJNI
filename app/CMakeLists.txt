cmake_minimum_required(VERSION 3.4.1)
################用cmake命令将CMakeLists.txt文件转化为make所需要的makefile文件#####################
project(chen_native_libs)                                                     #定义工程名称


message(STATUS "Project: ${PROJECT_NAME}")                                #打印相关消息消息
message(STATUS "Project Directory: ${PROJECT_SOURCE_DIR}")                #打印相关消息消息
message(STATUS "Project: Message,消息输出！")                              #打印相关消息消息

set(CMAKE_BUILE_TYPE DEBUG)                                               #指定编译类型，debug 版会生成相关调试信息
set(CMAKE_VERBOSE_MAKEFILE on)                                            #要显示执行构建过程中详细的信息(比如为了得到更详细的出错信息)

aux_source_directory(./src/main/cpp/ DIR_SRCS)                            #查找./src/main/cpp/目录下的所有源文件,将所有源文件的文件名赋值给变量DIR_SRCS
add_subdirectory(./src/main/cpp/algorithm)                                #添加子目录
add_subdirectory(./src/main/cpp/helloworld)                               #添加子目录
#add_subdirectory(./src/main/cpp/subtraction)                             #本来是添加子目录,后期生成so后，单独将so拿出来，放到jniLibs中，就不用这里的源码了

set(SRC_LIST ${DIR_SRCS})                                                 #设置源码目录SRC_LIST

add_library(${PROJECT_NAME} SHARED ${SRC_LIST})                           #生成链接库,动态库,根据当前SRC_LIST目录下的源码

find_library(chen_log-lib log)                                            ####加上这一句，否则找不到undefined reference to '__android_log_print'

target_link_libraries(${PROJECT_NAME}  ${chen_log-lib})                   #添加链接库
target_link_libraries(${PROJECT_NAME}  algorithm)                         #添加一个自己写的算法链接库
target_link_libraries(${PROJECT_NAME}  helloworld)                        #添加链接库

add_library(sub SHARED IMPORTED)                                          #so链接库,动态库,是引入类型imported
set_target_properties(sub PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libsub.so)
target_link_libraries(${PROJECT_NAME}  sub)                               #添加链接库，这个是自己编译好的so文件,直接用,就没有源码了，只有include头文件，其他源码生成的链接库

#set(distribution_DIR ${CMAKE_SOURCE_DIR}/libBuild)                       #CMAKE_SOURCE_DIR是Cmakelist.txt所在的路径
#add_library(libcgbinder SHARED IMPORTED)#暂时注释,
#set_target_properties(libcgbinder  PROPERTIES IMPORTED_LOCATION ${distribution_DIR}/android/lib/libbinder.so)#暂时注释
#add_library(libcgutils SHARED IMPORTED)#暂时注释
#set_target_properties(libcgutils  PROPERTIES IMPORTED_LOCATION ${distribution_DIR}/android/lib/libutils.so)#暂时注释,,#设置输出的名称,设置动态库的版本和API版本


#include_directories(src/jni)
#链接第三方库的头文件
#target_include_directories(${PROJECT_NAME} PRIVATE
#       ${distribution_DIR}/android/include/frameworks/native/include
#      ${distribution_DIR}/android/include/system/core/include)#暂时注释
#add_link_library生成(可执行文件或库)所需要的库，比如我们用到了libm.so（命名规则：lib+name+.so），就添加该库的名称，这里增加了log,现__android_log_print
#target_link_libraries(${PROJECT_NAME} libcgbinder libcgutils ${log-lib})#暂时注释

##############用cmake命令将CMakeLists.txt文件转化为make所需要的makefile文件#######################

###############################以下是参考学习####################################

#add_library(native-lib SHARED src/main/cpp/native-lib.cpp )#系统自带
#target_link_libraries( native-lib ${log-lib} )#系统自带
#指定生成目标，可执行
#add_executable(Xxx main.cc)
#生成链接库,动态库或静态库,,,Xxx代表生成的库的名字,SHARED代表是动态库,DIR_LIB_SRCS是存放源码的目录
#add_library (Xxx [SHARED | STATIC | MODULE] ${DIR_LIB_SRCS})
#本项目包含一个子目录Xxx，这样Xxx目录下的CMakeLists.txt文件和源代码也会被处理。
#add_subdirectory(Xxx)
#find_library() 查找库所在目录


