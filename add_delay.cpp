#include "WavFile.hpp"

bool check_user_input(int num_args, char** args_in) {
    if (num_args < 4) {
        std::cout << "Insufficent number of arguments provided. Running this command should take the form:\n" << args_in[0] 
            << " <input wav file> <output wav file> <length of delay in seconds> [OPTIONAL] <delay sample multiplier>\n"; 
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
    float delay = atof(argv[3]);
    float d_mult = 1.0;
    if (argc > 4) {
        d_mult = atof(argv[4]);
    }
    
    std::cout << "Reading in '" << in_file << "'...\n";
    WavFile wav(in_file);

    if (wav.data_size == 0) {
        exit(-1);
    }
    std::vector<unsigned char> in = wav.file;

    //Determine k, the number of samples in delay seconds of audio
    int k = wav.sample_rate * delay * wav.num_channels;

    std::cout << "Adding " << delay << " seconds (" << k / wav.num_channels << " samples) of delay...\n";

    int original_data_size = wav.data_size;

    //Resize wav file so there is sufficient room to hold added delay samples
    wav.enlarge(k);

    //Define vector to hold interim calculations
    std::vector<float> data_delay;
    data_delay.resize(wav.data_size);

    //Copy first k values, which will have no delay
    for (int i=0; i<k; i++) {
        data_delay[i] = (float)wav.data[i];
    }
    //Calculate all delayed values
    for (int i=0; i<original_data_size; i+=wav.num_channels) {
        for (int j=0; j<wav.num_channels; j++) {
            data_delay[i+k+j] = (float)wav.data[i+k+j] + d_mult * (float)wav.data[i+j];
        }
    }
    //Normalize calculated data to fit within int16
    float max_val_delay = 0;
    for (int i=0; i<data_delay.size(); i++) {
        if (max_val_delay < std::abs(data_delay[i])) {
            max_val_delay = std::abs(data_delay[i]);
        }
    }
    for (int i=0; i<data_delay.size(); i++) {
        data_delay[i] = data_delay[i] * (32768 / max_val_delay);
    }
    //Write calculated data to data that will be written out
    for (int i=0; i<(wav.data_size); i++) {
        wav.data[i] = (int16_t)data_delay[i];
    }

    std::cout << "Writing to '" << out_file << "'...\n";
    wav.write_out(out_file);
}
