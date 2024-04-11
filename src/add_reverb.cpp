#include "WavFile.hpp"
#include "audio_process_lib.hpp"

bool check_user_input(int num_args, char** args_in) {
    if (num_args < 2) {
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
    
    std::vector<float> signal = wav.get_samples();

    std::cout << "Generating reverb impulse response...\n";
    //For test, make impulse response that creates echo every second, three times
    std::unordered_map<uint64_t, float> h_reverb;
    h_reverb[0] = 1;
    h_reverb[wav.sample_rate] = 1;
    h_reverb[wav.sample_rate * 2] = 1;
    h_reverb[wav.sample_rate * 3] = 1;


    std::cout << "Performing convolution...\n";
    std::vector<float> result;
    std::cout << convolve(signal, h_reverb, result) << "\n";

    std::cout << "Normalizing result into int16\n";
    float max_val_result = 0;
    for (auto val: result) {
        if (val > max_val_result) {
            max_val_result = val;
        }
    }
    std::cout << "result size = " << result.size() << "\n";
    std::cout << "wav data size (b4) = " << wav.data_size << "\n";
    wav.enlarge(result.size());
    std::cout << "wav data size (aft) = " << wav.data_size << "\n";
    for (int i=0; i < wav.data_size; i++) {
        wav.data[i] =  (int16_t) (result[i] * (INT16_MAX / max_val_result));
    }

    std::cout << "Writing to '" << out_file << "'...\n";
    wav.write_out(out_file);
}
