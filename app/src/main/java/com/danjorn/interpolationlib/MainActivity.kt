package com.danjorn.interpolationlib

import android.content.Context
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.danjorn.wavinterpolation.interpolateWav
import java.io.File

private const val TAG = "MainActivity"

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
    }
}