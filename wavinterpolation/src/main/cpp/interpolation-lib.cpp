#include <jni.h>
#include <unistd.h>
#include "AudioManager.h"

extern "C" JNIEXPORT void
Java_com_danjorn_wavinterpolation_WavIntrepolationKt_interpolate(JNIEnv *env, jclass,
                                                                 jstring input_path,
                                                                 jstring output_path,
                                                                 jfloat coefficient) {

    const char *inWavPath = env->GetStringUTFChars(input_path, nullptr);
    const char *outWavPath = env->GetStringUTFChars(output_path, nullptr);
    AudioManager manager{};
    manager.audioResize(inWavPath, outWavPath, coefficient);
}