cmake_minimum_required(VERSION 3.4.1)

add_library( # Library name
             interpolation-lib

             SHARED

             # Provides a relative path to your source file(s).
        src/main/cpp/interpolation-lib.cpp)