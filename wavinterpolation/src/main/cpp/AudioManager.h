#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include <cmath>
#include "AudioFile.h"
#include <errno.h>

using namespace std;

class AudioManager {
private:
    void displayInfo(AudioFile file) {
        AudioFile::riffHeader header = file.header;
        AudioFile::subChunk1 firstSubChunk = file.firstSubChunk;
        AudioFile::subChunk2 secondSubChunk = file.secondSubChunk;

        cout << "chunkID: " << header.chunkID[0] << header.chunkID[1] << header.chunkID[2]
             << header.chunkID[3]
             << endl;
        cout << "chunkSize:" << header.chunkSize << endl;
        cout << "format: " << header.format[0] << header.format[1] << header.format[2]
             << header.format[3] << endl;
        cout << "subchunk1ID: " << firstSubChunk.subchunk1ID[0] << firstSubChunk.subchunk1ID[1]
             << firstSubChunk.subchunk1ID[2] << firstSubChunk.subchunk1ID[3] << endl;
        cout << "subchunk1Size: " << firstSubChunk.subchunk1Size << endl;
        cout << "audioFormat: " << firstSubChunk.audioFormat << endl;
        cout << "numChannels: " << firstSubChunk.numChannels << endl;
        cout << "sampleRate: " << firstSubChunk.sampleRate << endl;
        cout << "byteRate: " << firstSubChunk.byteRate << endl;
        cout << "blockAlign: " << firstSubChunk.blockAlign << endl;
        cout << "bitsPerSample: " << firstSubChunk.bitsPerSample << endl;
        cout << "subchunk2ID: " << secondSubChunk.subchunk2ID[0] << secondSubChunk.subchunk2ID[1]
             << secondSubChunk.subchunk2ID[2] << secondSubChunk.subchunk2ID[3] << endl;
        cout << "subchunk2Size: " << secondSubChunk.subchunk2Size << endl;
    }

    void displayDataLayer(AudioFile audioFile) {
        int numOfSamples = getNumberOfSamples(audioFile);

        for (int i = 0; i < numOfSamples; ++i) {
            cout << hex << (audioFile.data[i]) << " ";
        }
    }

    AudioFile convertToAudio(FILE *input) {
        AudioFile result{};

        fread(&result.header, sizeof(result.header), 1, input);
        fread(&result.firstSubChunk, sizeof(result.firstSubChunk), 1, input);
        fread(&result.secondSubChunk, sizeof(result.secondSubChunk), 1, input);

        getDataLayer(input, result);
        return result;
    }

    void getDataLayer(FILE *input, AudioFile &audioFile) {
        int numOfSamples = getNumberOfSamples(audioFile);

        audioFile.data = new int16_t[numOfSamples];
        for (int i = 0; i < numOfSamples; i++) {
            fread(&audioFile.data[i], 2, 1, input);
        }
    }

    AudioFile generateAudioFile(const char *filePath) {
        FILE *input = fopen(filePath, "rb");
        if (input == nullptr) {
            cout << "File cannot be opened!" << endl;
            char* error = strerror(errno);
            throw runtime_error(error);
        }

        AudioFile audio = convertToAudio(input);
        fclose(input);

        return audio;
    }

    void writingFile(const char *outputFile, AudioFile audioFile) {
        FILE *output = fopen(outputFile, "wb");
        fwrite(&audioFile.header, sizeof(audioFile.header), 1, output);
        fwrite(&audioFile.firstSubChunk, sizeof(audioFile.firstSubChunk), 1, output);
        fwrite(&audioFile.secondSubChunk, sizeof(audioFile.secondSubChunk), 1, output);

        int numOfSamples = getNumberOfSamples(audioFile);

        for (int i = 0; i < numOfSamples; i++) {
            fwrite(&audioFile.data[i], 2, 1, output);
        }
        fclose(output);
    }

    double interpolationFun(double dist, int16_t *data, int size) {
        int i = std::floor(dist);
        dist -= (int) dist;

        int x0 = i;
        int x1 = i + 1;
        if (x1 >= size) x1 = x0;

        double y0 = data[x0];
        double y1 = data[x1];

        return y0 + dist * (y1 - y0);
    }

    int getNumberOfSamples(const AudioFile &audioFile) const {
        int dataBytes = audioFile.secondSubChunk.subchunk2Size;
        int bytesPerSample = audioFile.firstSubChunk.bitsPerSample / 8;
        int numOfSamples = dataBytes / bytesPerSample;
        return numOfSamples;
    }

    AudioFile revertAudio(AudioFile file) {
        AudioFile result = file;
        int numOfSamples = getNumberOfSamples(file);
        int16_t *reverted = new int16_t[numOfSamples];
        for (int i = 0; i < numOfSamples; ++i) {
            int indexFromEnd = numOfSamples - i - 1;
            reverted[i] = result.data[indexFromEnd];
        }

        result.data = reverted;

        return result;
    }

public:
    void copy(const char *inputPath, const char *outputFile) {
        AudioFile audioFile = generateAudioFile(inputPath);
        writingFile(outputFile, audioFile);
    }

    void audioResize(const char *inputPath, const char *outputFile, double coefficient) {
        cout << "Scaling sound file... ";
        AudioFile audioFile = generateAudioFile(inputPath);

        if (coefficient < 0) {
            audioFile = revertAudio(audioFile);
            coefficient *= -1;
        }

        AudioFile out{};

        int numOfSamples = getNumberOfSamples(audioFile);

        int oldSize = numOfSamples;
        int newSize = oldSize * coefficient;

        int16_t *newData = new int16_t[newSize];
        for (int i = 0; i < newSize; i++) {
            double dist = ((double) i / (double) newSize) * (double) oldSize;
            newData[i] = interpolationFun(dist, audioFile.data, oldSize);
        }

        out.header = audioFile.header;
        out.firstSubChunk = audioFile.firstSubChunk;
        out.secondSubChunk = audioFile.secondSubChunk;
        out.data = newData;

        out.secondSubChunk.subchunk2Size = audioFile.secondSubChunk.subchunk2Size * coefficient;
        out.header.chunkSize = 36 + out.secondSubChunk.subchunk2Size;

        writingFile(outputFile, out);
        cout << "Done " << endl;
    }
};
