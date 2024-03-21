#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <cstdint>

int check_user_input(int num_args, char** args_in) {
    if (num_args != 3) {
        std::cout << "Insufficent number of arguments provided. Running this command should take the form " << args_in[0] 
            << " <input wav file> <output wav file>\n"; 
        exit(-1);
    }
    return 0;
}

bool extract_wav_file(const char* filename, std::vector<unsigned char>& wav) {
    std::ifstream f(filename, std::ios::binary);
    if (!f.is_open()) {
        std::cerr << "Could not open file: " << filename << "\n";
        return false;
    }
    f.seekg(0, std::ios::end);
    size_t size = (size_t)f.tellg();
    wav.resize(size);
    f.seekg(0);
    f.read((char*)&wav[0], size);
    return true;
}

int main(int argc, char** argv) {
    check_user_input(argc, argv); 
    char* in_file = argv[1];
    char* out_file = argv[2];

    std::vector<unsigned char> wav_in;
    extract_wav_file(in_file, wav_in);

    std::string header;
    header.resize(44);
    memcpy(header.data(), wav_in.data(), header.size());

    int32_t data_form_len = *(int32_t*)&header[16];
    int16_t num_channels = *(int16_t*)&header[22];
    int32_t fs = *(int32_t*)&header[24];
    int32_t data_sect_size = *(int32_t*)&header[40];

    std::cout << "data format length: " << data_form_len << "\n";
    std::cout << "number of channels: " << num_channels << "\n";
    std::cout << "sample rate: " << fs << "\n";
    std::cout << (char*)&header[36] << "\n";
    std::cout << "size of the data section: " << data_sect_size << "\n";

}