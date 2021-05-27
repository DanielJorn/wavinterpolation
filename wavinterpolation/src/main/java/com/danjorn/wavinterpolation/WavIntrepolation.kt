package com.danjorn.wavinterpolation

private const val TAG = "WavInterpolation"

fun interpolateWav(inputPath: String, outputPath: String, coefficient: Float) {
    System.loadLibrary("native-lib")
    interpolate(inputPath, outputPath, coefficient)
}

private external fun interpolate(inputPath: String, outputPath: String, coefficient: Float)