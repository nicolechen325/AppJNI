package com.jni.app;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

public class SampleJNIActivity extends Activity implements View.OnClickListener {
    PAJni paJni;
    JNISetObj jniSetObj;

    private TextView tvJniGetStr;
    private TextView tvJniSetNativeValue;
    private TextView tvJniRetriveJava;
    private TextView tvJniGetArray;

    private TextView tvJniGetStrText;
    private TextView tvJniGetArrayText;
    private TextView tvJniSetNativeValueText;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_sample_jni);
        initView();
        jniSetObj = new JNISetObj();
        paJni = new PAJni();
    }

    private void initView() {

        tvJniGetStr = findViewById(R.id.tv_jni_get_str);
        tvJniSetNativeValue = findViewById(R.id.tv_jni_set_native_value);
        tvJniRetriveJava = findViewById(R.id.tv_jni_retrive_java);
        tvJniGetArray = findViewById(R.id.tv_jni_get_array);
        tvJniGetStrText = findViewById(R.id.tv_jni_get_str_text);
        tvJniGetArrayText = findViewById(R.id.tv_jni_get_array_text);
        tvJniSetNativeValueText = findViewById(R.id.tv_jni_set_native_value_text);

        tvJniGetStr.setOnClickListener(this);
        tvJniSetNativeValue.setOnClickListener(this);
        tvJniRetriveJava.setOnClickListener(this);
        tvJniGetArray.setOnClickListener(this);
    }


    @Override
    public void onClick(View v) {
        if (v == tvJniGetStr) {
            String str = paJni.nativeGetStrFromJNI();
            tvJniGetStrText.setText(str);
        } else if (v == tvJniSetNativeValue) {
            paJni.nativeSetJObjectValueInNative(jniSetObj);
            tvJniSetNativeValueText.setText(jniSetObj.getmSetByJNI_char() + "");
        } else if (v == tvJniRetriveJava) {
            paJni.nativeRetriveJava(jniSetObj);
        } else if (v == tvJniGetArray) {
            byte[] srcBytes = new byte[]{10, 12, 13, 14};
            String src = "";
            for (int i = 0; i < srcBytes.length; i++) {
                src = src + String.valueOf(0xff & srcBytes[i]) + ",";
            }
            byte[] relBytes = paJni.nativeParameterArray(srcBytes);
            String result = "";
            for (int i = 0; i < relBytes.length; i++) {
                result = result + String.valueOf(0xff & relBytes[i]) + ",";
            }
            StringBuilder sb = new StringBuilder();
            sb.append("JniSrcBytes:" + src);
            sb.append("\n");
            sb.append("JIN层返回字节数组,JniResultBytes:" + result);
            tvJniGetArrayText.setText(sb);
        }
    }

}
