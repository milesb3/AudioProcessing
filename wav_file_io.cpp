#include "wav_file_io.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <cstdint>
/*
struct WavFile {
    std::vector<unsigned char> file_content;
    int16_t num_channels = 0;
    int32_t sample_rate = 0;
    int16_t* data = NULL;
};
*/
bool extract_wav_file(const char* filename, WavFile& wav) {
    //re-initialize input WavFile struct
    wav.file_content.clear();
    wav.num_channels = 0;
    wav.sample_rate = 0;
    wav.data = NULL;

    //Read in wav file and store in WavFile struct's file_content vector of unsigned chars
    std::ifstream f(filename, std::ios::binary);
    if (!f.is_open()) {
        std::cerr << "Could not open file '" << filename << "'\n";
        return false;
    }
    f.seekg(0, std::ios::end);
    size_t size = (size_t)f.tellg();
    wav.file_content.resize(size);
    f.seekg(0);
    f.read((char*)&wav.file_content[0], size);

    //Parse header
    //Check if file is large enough to contain a wav file header
    if (wav.file_content.size() < 44) {
        std::cout << "Input file is too small to be a wav file\n";
        return false;
    }
    //Check for signature "RIFF" and "WAVE" sections of the wav header
    std::string header_check;
    header_check.resize(8);
    memcpy(header_check.data(), wav.file_content.data(), 4);
    memcpy(header_check.data() + 4, wav.file_content.data() + 8, 4);
    if (header_check != "RIFFWAVE") {
        std::cout << "Error! header_check = '" << header_check << "', but expected 'RIFFWAVE'\n";
        return false;
    }

    //Parse out number of channels and sample rate
    wav.num_channels = *(int16_t*)&wav.file_content[22];
    wav.sample_rate = *(int32_t*)&wav.file_content[24];

    //Find end of header and start of data section
    header_check.clear();
    header_check.resize(4);
    memcpy(header_check.data(), wav.file_content.data() + 36, 4);

    int data_offset = 0;
    if (header_check == "data") {
        data_offset = 44;
    }
    else {
        //Handle "LIST" section
        data_offset = *(int*)&wav.file_content[40] + 48;
    }
    wav.data = (int16_t*)(wav.file_content.data() + data_offset);
    wav.data_len = (wav.file_content.size() - data_offset)/2;

    return true;
}

void print_header(const WavFile wav) {
    std::string print_str;
    int print_int;

    print_str.resize(4);
    memcpy(print_str.data(), wav.file_content.data(), 4);
    std::cout << print_str << "\n";

    print_int = *(int32_t*)&wav.file_content[4];
    std::cout << "File size: " << print_int << "\n";

    memcpy(print_str.data(), wav.file_content.data() + 8, 4);
    std::cout << print_str << "\n";

    memcpy(print_str.data(), wav.file_content.data() + 12, 4);
    std::cout << print_str << "\n";

    print_int = *(int32_t*)&wav.file_content[16];
    std::cout << "Length of format: " << print_int << "\n";

    print_int = *(int16_t*)&wav.file_content[20];
    std::cout << "Type of format: " << print_int << "\n";

    print_int = *(int16_t*)&wav.file_content[22];
    std::cout << "Number of channels: " << print_int << "\n";

    print_int = *(int32_t*)&wav.file_content[24];
    std::cout << "Sample rate: " << print_int << "\n";

    print_int = *(int32_t*)&wav.file_content[28];
    std::cout << "(Sample Rate * BitsPerSample * Channels) / 8: " << print_int << "\n";

    print_int = *(int16_t*)&wav.file_content[32];
    std::cout << "(BitsPerSample * Channels) / 8.1 - 8 bit mono2 - 8 bit stereo/16 bit mono4 - 16 bit stereo: " << print_int << "\n";

    print_int = *(int16_t*)&wav.file_content[34];
    std::cout << "Bits per sample: " << print_int << "\n";

    memcpy(print_str.data(), wav.file_content.data() + 36, 4);
    std::cout << print_str << "\n";

    if (print_str == "data") {
        print_int = *(int32_t*)&wav.file_content[40];
        std::cout << "Size of data section: " << print_int << "\n";
        return;
    }

    print_int = *(int32_t*)&wav.file_content[40];
    std::cout << "Size of the rest of 'LIST' section: " << print_int << "\n";

    memcpy(print_str.data(), wav.file_content.data() + 44, 4);
    std::cout << "List type ID: " << print_str << "\n";

    print_str.resize(print_int);
    memcpy(print_str.data(), wav.file_content.data() + 48, print_int);
    std::cout << "Rest of 'LIST' section:\n" << print_str << "\n";
}

bool write_wav_file(const char* filename, const WavFile wav) {
    std::ofstream f(filename, std::ios::binary);
    f.write((char*)&wav.file_content[0], wav.file_content.size());
    return true;
}