package com.pa.chen.appjni;

import android.util.Log;

public class JNISetObj {

    private int mSetByJNI_int;
    private float mSetByJNI_float;
    private boolean mSetByJNI_boolean;
    private byte mSetByJNI_byte;
    private char mSetByJNI_char;
    private String mSetByJNI_string;

    public JNISetObj() {
    }

    public int getmSetByJNI_int() {
        return mSetByJNI_int;
    }

    public void setmSetByJNI_int(int mSetByJNI_int) {
        this.mSetByJNI_int = mSetByJNI_int;
    }

    public float getmSetByJNI_float() {
        return mSetByJNI_float;
    }

    public void setmSetByJNI_float(float mSetByJNI_float) {
        this.mSetByJNI_float = mSetByJNI_float;
    }

    public boolean ismSetByJNI_boolean() {
        return mSetByJNI_boolean;
    }

    public void setmSetByJNI_boolean(boolean mSetByJNI_boolean) {
        this.mSetByJNI_boolean = mSetByJNI_boolean;
    }

    public byte getmSetByJNI_byte() {
        return mSetByJNI_byte;
    }

    public void setmSetByJNI_byte(byte mSetByJNI_byte) {
        this.mSetByJNI_byte = mSetByJNI_byte;
    }

    public char getmSetByJNI_char() {
        return mSetByJNI_char;
    }

    public void setmSetByJNI_char(char mSetByJNI_char) {
        this.mSetByJNI_char = mSetByJNI_char;
    }

    public String getmSetByJNI_string() {
        return mSetByJNI_string;
    }

    public void setmSetByJNI_string(String mSetByJNI_string) {
        this.mSetByJNI_string = mSetByJNI_string;
    }

    public String onCalledByNative(int code, String value) {
        String retrive = value + code;
        Log.d("onCalledByNative", "onCalledByNative:" + value);
        return retrive;
    }

}
