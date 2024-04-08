#include "WavFile.hpp"
#include "math.h"

bool check_user_input(int num_args, char** args_in) {
    if (num_args < 4) {
        std::cout << "Insufficent number of arguments provided. Running this command should take the form:\n" << args_in[0] 
            << " <input wav file> <output wav file> <number of bits to crush to>\n"; 
        return false;
    }
    return true;
}

int main(int argc, char** argv) {
    if (!check_user_input(argc, argv)) {
        exit(-1);
    }
    char* in_file = argv[1];
    char* out_file = argv[2];
    int bits = atoi(argv[3]);
    
    std::cout << "Reading in '" << in_file << "'...\n";
    WavFile wav(in_file);

    if (wav.data_size == 0) {
        exit(-1);
    }

    int crush_factor = pow(2, 16-bits);

    std::cout << "Crushing samples to fit within " << bits << " bits with crush factor " << crush_factor << "\n";
    for (int i=0; i<wav.data_size; i++) {
        wav.data[i] = wav.data[i] / crush_factor;
        wav.data[i] = wav.data[i] * crush_factor;
    }

    std::cout << "Writing to '" << out_file << "'...\n";
    wav.write_out(out_file);
}
