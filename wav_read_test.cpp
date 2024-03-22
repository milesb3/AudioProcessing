#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <cstdint>

struct WavHeader {
    int16_t num_channels = 0;
    int32_t sample_rate = 0;
    int data_start = 0;
};

bool check_user_input(int num_args, char** args_in) {
    if (num_args != 3) {
        std::cout << "Insufficent number of arguments provided. Running this command should take the form " << args_in[0] 
            << " <input wav file> <output wav file>\n"; 
        return false;
    }
    return true;
}

WavHeader parse_wav_header(const std::vector<unsigned char>& wav) {
    WavHeader wav_head;
    if (wav.size() < 44) {
        std::cout << "Input file is too small to be wav :(\n";
        return wav_head;
    }

    std::string file_type_check;
    file_type_check.resize(8);
    for (int i=0; i<4; i++) {
        file_type_check[i] = wav[i];
    }
    for (int i=4; i<8; i++) {
        file_type_check[i] = wav[i+4];
    }

    if (file_type_check != "RIFFWAVE") {
        std::cout << "Input file is missing wav file header identfiers :(\n";
        return wav_head;
    }

    wav_head.num_channels = *(int16_t*)&wav[22];
    wav_head.sample_rate = *(int32_t*)&wav[24];

    std::string chunk;
    chunk.resize(4);
    memcpy(chunk.data(), wav.data() + 36, 4);

    if (chunk == "data") {
        wav_head.data_start = 44;
    }
    else {
        //Handle "LIST" chunk
        wav_head.data_start = *(int32_t*)&wav[40] + 40;
    }

    return wav_head;
}

bool extract_wav_file(const char* filename, std::vector<unsigned char>& wav) {
    //Read in wav file and store in vector of unsiged chars
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
    if (!check_user_input(argc, argv)) {
        exit(-1);
   }
    char* in_file = argv[1];
    char* out_file = argv[2];
    std::vector<unsigned char> wav_in;
    WavHeader wav_in_head;
    
    extract_wav_file(in_file, wav_in);
    wav_in_head = parse_wav_header(wav_in);

    std::cout << "-Header information-" << "\n";
    std::cout << "Number of channels: " << wav_in_head.num_channels << "\n";
    std::cout << "Sample rate: " << wav_in_head.sample_rate << "\n";
    std::cout << "Data offset: " << wav_in_head.data_start << "\n";

    std::vector<unsigned char> data_out;
    data_out.resize(wav_in.size() - wav_in_head.data_start);
    memcpy(data_out.data(), wav_in.data() + wav_in_head.data_start, data_out.size());
}