#include "WavFile.hpp"

bool check_user_input(int num_args, char** args_in) {
    if (num_args < 3) {
        std::cout << "Insufficent number of arguments provided. Running this command should take the form:\n" << args_in[0] 
            << " <input wav file> <output wav file>\n"; 
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
    
    std::cout << "Reading in '" << in_file << "'...\n";
    WavFile wav(in_file);

    if (wav.data_size == 0) {
        exit(-1);
    }

    std::vector<float> out_data = wav.get_samples();

    //Find max value in input wav file
    int max_samp = 0;
    for (int i=0; i<wav.data_size; i++) {
        if (max_samp < wav.data[i]) {
            max_samp = wav.data[i];
        }
    }

    float fill_mult = 32768.0 / max_samp;

    std::cout << "Filling out each sample from " << in_file << " with fill multiplier " << fill_mult << "...\n";
    for (int i=0; i<wav.data_size; i++) {
        wav.data[i] *= fill_mult;
    }

    std::cout << "Writing to '" << out_file << "'...\n";
    wav.write_out(out_file);
}
