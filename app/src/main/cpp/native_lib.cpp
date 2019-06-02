#include <jni.h>
#include "include/android_log_print.h"
#include<string.h>

//c++代码，调用了c语言库的函数，因此链接的时候找不到,需要加上extern "C"声明
extern "C" {
#include "algorithm/algorithm.h"
#include "include/sub.h"
#include "helloworld/helloworld.h"
}
extern "C"

#include<math.h>

//在这里定义JavaVM不知道会不会出问题
JavaVM *javaVM;

//Java层目录
const char *const kJNI = "com/jni/app/JniCls";

static struct native_t {
    jclass clazz;
    jmethodID onCalledByNative;
    jfieldID mObject;
} gPaJniClassInfo;


//JNIEXPORT和JNICALL都是JNI关键字，表示此函数是被JNI调用的
//函数命名规则为：Java_类全名_方法名。它们之间用 _(下划线) 连接

/************************获取JNI中字符串******************/
JNIEXPORT jstring JNICALL
Java_com_jni_app_JniCls_nativeGetStrFromJNI(JNIEnv *env, jobject /* this */) {
    const char *hello = "Hello World ,I come from JNI!";
    LOGV("nativeGetHelloWorld, %s", hello);
    jdouble base = 4;
    jint exponent = 3;
    jdouble result = power(base, exponent);//次方计算,64
    jdouble output = add(result, 120.0);//加法计算，184
    jdouble output2 = sub(output, 12.0);//减法计算，172
    hello_world(hello);
    LOGV("nativeGetHelloWorld, %g %d is %g\n", base, exponent, output2);
    jstring jstr = env->NewStringUTF(hello);
    return jstr;
}

/**************************************JNI中设置Java对象内部变量***************************************/
JNIEXPORT void JNICALL
Java_com_jni_app_JniCls_nativeSetJObjectValueInNative(JNIEnv *env, jclass clazz,
                                                      jobject obj) {
    //clazz是Java层调用native方法的类对象，并不是obj的class类
    jclass clazz_ = env->GetObjectClass(obj);
    //查找class中属性，最后一个参数是变量签名(int 的变量签名是"I",float 的变量签名是"F"...)
    jfieldID fid_int = env->GetFieldID(clazz_, "jni_value_int", "I");
    jfieldID fid_float = env->GetFieldID(clazz_, "jni_value_float", "F");
    jfieldID fid_boolean = env->GetFieldID(clazz_, "jni_value_boolean", "Z");
    jfieldID fid_byte = env->GetFieldID(clazz_, "jni_value_byte", "B");
    jfieldID fid_char = env->GetFieldID(clazz_, "jni_value_char", "C");
    jfieldID fid_string = env->GetFieldID(clazz_, "jni_value_string",
                                          "Ljava/lang/String;");////String对象其签名类型需要分号;
    //获取属性值
    jint value_int = env->GetIntField(obj, fid_int);
    jfloat value_float = env->GetFloatField(obj, fid_float);
    jboolean value_boolean = env->GetBooleanField(obj, fid_boolean);
    jbyte value_byte = env->GetByteField(obj, fid_byte);
    jchar value_char = env->GetCharField(obj, fid_char);
    //获取Java层字符串类型属性值
    jstring value_string = (jstring) env->GetObjectField(obj, fid_string);
    //设置Java层对象的属性值
    jint number = env->GetIntField(obj, fid_int);
    //加10
    env->SetIntField(obj, fid_int, value_int + 10);
    env->SetFloatField(obj, fid_float, value_float + 10);
    //字节取反
    env->SetByteField(obj, fid_byte, 0x10);
    //布尔值取反
    env->SetBooleanField(obj, fid_boolean, (jboolean) (!value_boolean));
    //变大写？出现啊x乱码
    env->SetCharField(obj, fid_char, (jchar) (value_char));
    //jstring拷贝到jchar字符串指针，注释掉的是第一种，const，不可变，对应String不可变。
    //const jchar *bestring = env->GetStringChars(value_string, NULL);
    //获取jstring长度
    jsize len = env->GetStringLength(value_string);
    //下面使用第二种，新创建内存
    jchar *bestring = new jchar[len + 1];
    //设置已\0结尾，否则会出现乱码
    bestring[len] = L'\0';
    //初始化字符串指针
    env->GetStringRegion(value_string, 0, len, bestring);
    //env->ReleaseStringChars(value_string,bestring);
    //jchar字符串指针指向字符串翻转
    for (jint i = 0, j = len - 1; i < j; i++, j--) {
        jchar temp = bestring[i];
        bestring[i] = bestring[j];
        bestring[j] = temp;
    }
    for (jsize i = 0; i < len; i++) {
        LOGV("value from Java function,%d,%c", len, bestring[i]);
    }
    //反转后获取新的字符串
    jstring newstring = env->NewString(bestring, len);
    delete[] bestring;
    //新字符串设置到Java层变量中
    env->SetObjectField(obj, fid_string, newstring);

};

/*********************************************回调Java方法***************************************/
JNIEXPORT void JNICALL
Java_com_jni_app_JniCls_nativeRetriveJava(JNIEnv *env, jclass clazz, jobject obj) {
    const char *beGoPaJava = "i will go Java";
    jstring beSetString = env->NewStringUTF(beGoPaJava);
    //获取Jave层对象的class的实例
    jclass clazz_ = env->GetObjectClass(obj);
    //查找Java层中该Class中的方法
    jmethodID methodId_ = env->GetMethodID(clazz_, "onCallByNative",
                                           "(ILjava/lang/String;)Ljava/lang/String;");
    //调用Java层方法，传给Java层参数，调用可变参数这个方法
    //Java层返回String类型
    jstring name_str = reinterpret_cast<jstring>(env->CallObjectMethod(obj,
                                                                       methodId_,
                                                                       23, beSetString));
    char buf[128];
    const char *str = env->GetStringUTFChars(name_str, 0);
    //打印从Java层返回jstring转换的char*
    LOGV("value from Java function,%s", str);
}

/**************************************参数与返回包括字节数组********************************/
JNIEXPORT jbyteArray JNICALL
Java_com_jni_app_JniCls_nativeParameterArray(JNIEnv *env, jobject obj,
                                             jbyteArray jbyteArray1) {
    //将Java层基本类型数组转换到C++数组。获取jbyteArray对象的指针
    jbyte *byte_array = env->GetByteArrayElements(jbyteArray1, 0);
    //获取数组长度
    jsize len = env->GetArrayLength(jbyteArray1);
    //新建一个jbytearray数组
    jbyteArray jbyte_array_new = env->NewByteArray(len);
    jbyte *byte_array_new = env->GetByteArrayElements(jbyte_array_new, NULL);
    //传入数组偶数item存储新数组中
    jint count = 0;
    for (jsize j = 0; j < len; j++) {
        if (j % 2 == 0) {
            byte_array_new[count++] = byte_array[j];
        }
    }
    LOGV("value from Java function,%s", "c" + byte_array_new[0]);
    for (int i = 0; i < len; i++) {
        //int value = receivedbyte[i] & 0xFF;
        //取反,例如，0x00001100,取反后为0x11110011
        jbyte jbyte = (~byte_array[i]);
        env->SetByteArrayRegion(jbyteArray1, i, 1, &jbyte);
    }
    return jbyteArray1;
}

//动态注册
JNINativeMethod gMethods[] = {
        {"nativeGetStrFromJNI",           "()Ljava/lang/String;",            (void *) Java_com_jni_app_JniCls_nativeGetStrFromJNI},
        {"nativeSetJObjectValueInNative", "(Lcom/jni/app/JniTransmisObj;)V", (void *) Java_com_jni_app_JniCls_nativeSetJObjectValueInNative},
        {"nativeRetriveJava",             "(Lcom/jni/app/JniTransmisObj;)V", (void *) Java_com_jni_app_JniCls_nativeRetriveJava},
        {"nativeParameterArray",          "([B)[B",                          (void *) Java_com_jni_app_JniCls_nativeParameterArray},
};

JNIEnv *JNU_GetEnv() {
    JNIEnv *env;
    javaVM->GetEnv((void **) &env, JNI_VERSION_1_4);
    return env;
}

//在执行方法system.loadLibrary()时调用
//通过该方法告诉VM,当前so库使用的JNI版本，进行数据初始化，对java类调用的native函数注册
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    javaVM = vm;
    JNIEnv *env;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return -1;
    }
    jclass clz = env->FindClass(kJNI);

    env->RegisterNatives(clz, gMethods, sizeof(gMethods) / sizeof(gMethods[0]));
    return JNI_VERSION_1_4;
}