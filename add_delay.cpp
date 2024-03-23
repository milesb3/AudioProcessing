#include "wav_file_io.hpp"

bool check_user_input(int num_args, char** args_in) {
    if (num_args != 3) {
        std::cout << "Insufficent number of arguments provided. Running this command should take the form " << args_in[0] 
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
    WavFile wav;

    std::cout << "Reading in '" << in_file << "'...\n";
    if (!extract_wav_file(in_file, wav)) {
        exit(-1);
    }

    std::cout << "Writing to '" << out_file << "'...\n";
    write_wav_file(out_file, wav);
}