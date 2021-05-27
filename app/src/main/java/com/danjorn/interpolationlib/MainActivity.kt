package com.danjorn.interpolationlib

import android.content.Context
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import com.danjorn.wavinterpolation.interpolateWav
import java.io.File

private const val TAG = "MainActivity"

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val input = assetToFile(this, "input.wav")
        interpolateWav(input, "$filesDir/output.wav", 2f)
    }

    private fun assetToFile(context: Context, fileName: String): String {
        context.assets.open(fileName).use { inStream ->
            val tempFileName = "tmp_$fileName"
            val fullPath = "${context.filesDir}/$tempFileName"
            val possibleFile = File(fullPath)

            return if (possibleFile.exists()){
                fullPath
            } else {
                context.openFileOutput(tempFileName, Context.MODE_PRIVATE).use { outStream ->
                    var bytesRead: Int
                    val buffer = ByteArray(4096)
                    while (inStream.read(buffer).also { bytesRead = it } != -1) {
                        outStream.write(buffer, 0, bytesRead)
                    }
                }
                fullPath
            }
        }
    }
}