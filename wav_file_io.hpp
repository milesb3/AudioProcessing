#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <cstdint>

struct WavFile {
    std::vector<unsigned char> file_content;
    int16_t num_channels = 0;
    int32_t sample_rate = 0;
    int data_len = 0;
    int16_t* data = NULL;
};

bool extract_wav_file(const char* filename, WavFile& wav);
void print_header(const WavFile wav);
bool write_wav_file(const char* filename, const WavFile wav);