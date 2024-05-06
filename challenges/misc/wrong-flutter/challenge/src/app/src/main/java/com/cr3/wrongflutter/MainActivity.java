package com.cr3.wrongflutter;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import io.flutter.embedding.engine.FlutterJNI;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        FlutterJNI f = new FlutterJNI(this);
    }
}