#include "wav_file_io.hpp"

bool check_user_input(int num_args, char** args_in) {
    if (num_args != 4) {
        std::cout << "Insufficent number of arguments provided. Running this command should take the form " << args_in[0] 
            << " <input wav file> <output wav file> <length of delay in seconds>\n"; 
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
    WavFile wav;

    std::cout << "Reading in '" << in_file << "'...\n";
    if (!extract_wav_file(in_file, wav)) {
        exit(-1);
    }

    std::cout << "Adding " << delay << " seconds of delay...\n";
    //TODO MAKE WORK FOR 2 CHANNELS!!!
    //Determine k, the number of samples in delay seconds of audio
    int k = wav.sample_rate * delay;
    std::cout << "k = " << k << " samples\n";

    if (k > wav.data_len) {
        std::cout << "Cannot add a delay of " << delay << " seconds as this is longer than the input sample.\n";
        exit(-1);
    }
    std::cout << "data_len = " << wav.data_len << "\n";

    //Resize wav file so there is sufficient room to hold added delay samples
    wav.file_content.resize(wav.file_content.size() + k);
    wav.data = (int16_t*)&wav.file_content[44]; //FIGURE OUT BETTER WAY TO DEAL WITH THIS!!!

    //Define vector to hold interim calculations
    std::vector<float> data_delay;
    data_delay.resize(wav.data_len + k);

    //Copy first k values, which will have no delay
    for (int i=0; i<k; i++) {
        data_delay[i] = (float)wav.data[i];
    }
    //Calculate all delayed values
    for (int i=0; i<wav.data_len; i++) {
        data_delay[i+k] = (float)wav.data[i+k] + (float)wav.data[i];
    }
    //Normalize calculated data to fit within int16
    float max_val_delay = 0;
    for (int i=0; i<data_delay.size(); i++) {
        if (max_val_delay < std::abs(data_delay[i])) {
            max_val_delay = std::abs(data_delay[i]);
        }
    }
    std::cout << "max_val_delay = " << max_val_delay << "\n";
    for (int i=0; i<data_delay.size(); i++) {
        data_delay[i] = data_delay[i] * (32768 / max_val_delay);
    }
    //Write calculated data to data that will be written out
    for (int i=0; i<(wav.data_len+k); i++) {
        wav.data[i] = (int16_t)data_delay[i];
    }

    //Update data length
    wav.data_len = wav.data_len + k;


    std::cout << "Writing to '" << out_file << "'...\n";
    write_wav_file(out_file, wav);
}