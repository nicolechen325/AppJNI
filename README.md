### JNI

##### External Tool配置
* **javah** 一键根据java文件内部native方法生成h头文件。

**命令目录**
$JDKPath$\bin\javah

**参数信息**
-classpath $ModuleFileDir$/src/main/java/ -jni -d $ModuleFileDir$/src/main/cpp/ $FileClass$

**工作目录**
$FileDir$

头文件名 包名(下划线)+类名.
头文件定义了Java层native方法的声明，
函数名 Java_包名(下划线)+类名_xxx方法名。


