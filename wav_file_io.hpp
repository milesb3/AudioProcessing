#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <cstdint>

struct WavFile {
    std::vector<unsigned char> file_content;
    int16_t num_channels;
    int32_t sample_rate;
    int16_t* data;
};

bool extract_wav_file(const char* filename, WavFile& wav);

bool write_wav_file(const char* filename, const WavFile wav);