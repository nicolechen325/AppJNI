package com.jni.app;
/**
 * Created by CHENGUANG491 on 2018/8/14.
 */

public class PAJni {

    public static final String TAG = "PaJni";

    static {
        System.loadLibrary("chen_native_libs");
    }

    //获取JNI中字符串
    public native String nativeGetStrFromJNI();

    //JNI中设置Java对象内部变量
    public native void nativeSetJObjectValueInNative(JNISetObj obj);

    //回调Java方法
    public native void nativeRetriveJava(JNISetObj obj);

    //参数与返回包括字节数组
    public native byte[] nativeParameterArray(byte[] bytes);


}
