#include <jni.h>
#include "include/android_log_print.h"

//c++代码，调用了c语言库的函数，因此链接的时候找不到,需要加上extern "C"声明
extern "C" {
#include "algorithm/algorithm.h"
#include "include/sub.h"
#include "helloworld/helloworld.h"
}
extern "C"

#include<math.h>

JavaVM *javaVM;////在这里定义JavaVM不知道会不会出问题
const char *const kJNI = "com/jni/app/PAJni";//Java层目录

static struct native_t {
    jclass clazz;
    jmethodID onCalledByNative;
    jfieldID mObject;
} gPaJniClassInfo;


//JNIEXPORT和JNICALL都是JNI关键字，表示此函数是被JNI调用的
//函数命名规则为：Java_类全名_方法名。它们之间用 _(下划线) 连接

/************************获取JNI中字符串******************/
JNIEXPORT jstring JNICALL
Java_com_jni_app_PAJni_nativeGetStrFromJNI(JNIEnv *env, jobject /* this */) {
    const char *hello = "Hello World ,I come from JNI!";
    LOGV("nativeGetHelloWorld, %s", hello);
    double base = 4;
    int exponent = 3;
    double result = power(base, exponent);//次方计算,64
    double output = add(result, 120.0);//加法计算，184
    double output2 = sub(output, 12.0);//减法计算，172
    hello_world(hello);
    LOGV("nativeGetHelloWorld, %g %d is %g\n", base, exponent, output2);
    return env->NewStringUTF(hello);
}

/**************************************JNI中设置Java对象内部变量***************************************/
JNIEXPORT void JNICALL
Java_com_jni_app_PAJni_nativeSetJObjectValueInNative(JNIEnv *env, jclass clazz,
                                                     jobject obj) {
    jclass clz_jinsetobj = env->GetObjectClass(obj);
    //得到class中的某个变量，最后一个参数是变量签名(int 的变量签名是"I",float 的变量签名是"F"...)
    jfieldID fid_int = env->GetFieldID(clz_jinsetobj, "mSetByJNI_int", "I");
    jfieldID fid_float = env->GetFieldID(clz_jinsetobj, "mSetByJNI_float", "F");
    jfieldID fid_boolean = env->GetFieldID(clz_jinsetobj, "mSetByJNI_boolean", "Z");
    jfieldID fid_byte = env->GetFieldID(clz_jinsetobj, "mSetByJNI_byte", "B");
    jfieldID fid_char = env->GetFieldID(clz_jinsetobj, "mSetByJNI_char", "C");
    jfieldID fid_string = env->GetFieldID(clz_jinsetobj, "mSetByJNI_string",
                                          "Ljava/lang/String;");////String对象其签名类型需要分号;
    const int valSetInt = 251;
    float valSetFloat = (float) sqrt(64.0);
    jboolean beboolean = (jboolean) true;
    const jbyte valSetByt = 0x79;
    jchar beSetChar = (jchar) 'a';
    const char *bestring = "IWillgoJava";
    jstring beSetString = env->NewStringUTF(bestring);

    env->SetIntField(obj, fid_int, valSetInt);
    env->SetFloatField(obj, fid_float, valSetFloat);
    env->SetByteField(obj, fid_byte, valSetByt);
    env->SetBooleanField(obj, fid_boolean, beboolean);
    env->SetCharField(obj, fid_char, beSetChar);
    env->SetObjectField(obj, fid_string, beSetString);
};

/*********************************************回调Java方法***************************************/
JNIEXPORT void JNICALL
Java_com_jni_app_PAJni_nativeRetriveJava(JNIEnv *env, jclass clazz, jobject obj) {
    const char *beGoPaJava = "i will go Java";
    jstring beSetString = env->NewStringUTF(beGoPaJava);
    jclass clazz_ = env->GetObjectClass(obj);
    jmethodID methodId_ = env->GetMethodID(clazz_, "onCalledByNative",
                                           "(ILjava/lang/String;)Ljava/lang/String;");
    jstring name_str = reinterpret_cast<jstring>(env->CallObjectMethod(obj,
                                                                       methodId_,
                                                                       23, beSetString));
    char buf[128];
    const char *str = env->GetStringUTFChars(name_str, 0);
    LOGV("value from Java function,%s", str);//打印从Java层返回jstring转换的char*
}

/**************************************参数与返回包括字节数组********************************/
JNIEXPORT jbyteArray JNICALL
Java_com_jni_app_PAJni_nativeParameterArray(JNIEnv *env, jobject obj,
                                            jbyteArray jbyteArray1) {
    jbyte *receivedbyte = env->GetByteArrayElements(jbyteArray1, 0);
    jsize jsize1 = env->GetArrayLength(jbyteArray1);
    for (int i = 0; i < jsize1; i++) {
        //int value = receivedbyte[i] & 0xFF;
        jbyte jbyte = (~receivedbyte[i]);//取反,例如，0x00001100,取反后为0x11110011
        env->SetByteArrayRegion(jbyteArray1, i, 1, &jbyte);
    }
    return jbyteArray1;
}

//动态注册
JNINativeMethod gMethods[] = {
        {"nativeGetStrFromJNI",           "()Ljava/lang/String;",       (void *) Java_com_jni_app_PAJni_nativeGetStrFromJNI},
        {"nativeSetJObjectValueInNative", "(Lcom/jni/app/JNISetObj;)V", (void *) Java_com_jni_app_PAJni_nativeSetJObjectValueInNative},
        {"nativeRetriveJava",             "(Lcom/jni/app/JNISetObj;)V", (void *) Java_com_jni_app_PAJni_nativeRetriveJava},
        {"nativeParameterArray",          "([B)[B",                     (void *) Java_com_jni_app_PAJni_nativeParameterArray},
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