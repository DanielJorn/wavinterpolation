#include<iostream>
#include<vector>
#include<fstream>
#include<string>

using namespace std;

class AudioFile {
public:
#pragma pack(push, 1)
    typedef struct riffHeader {
        int8_t chunkID[4];
        int32_t chunkSize;
        int8_t format[4];
    }riffHeader;
#pragma pack(pop)
#pragma pack(push, 2)
    typedef struct subChunk1 {
        int8_t subchunk1ID[4];
        int32_t subchunk1Size;
        int16_t audioFormat;
        int16_t numChannels;
        int32_t sampleRate;
        int32_t byteRate;
        int16_t blockAlign;
        int16_t bitsPerSample;
    } subChunk1;
#pragma pack(pop)
#pragma pack(push, 3)
    typedef struct subChunk2 {
        int8_t subchunk2ID[4];
        int32_t subchunk2Size;
    }subChunk2;
#pragma pack(pop)

    riffHeader header;
    subChunk1 firstSubChunk;
    subChunk2 secondSubChunk;
    int16_t *data;
};