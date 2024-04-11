#include "audio_process_lib.hpp"

bool convolve(const std::vector<float> &signal, const std::unordered_map<uint64_t, float> &impulse, std::vector<float> &result) {
    std::cout << "s size = " << signal.size() << "\n";
    std::cout << "imp size = " << impulse.size() << "\n";
    if (signal.size() == 0 || impulse.size() == 0 || signal.size() < impulse.size()) {
        return false;
    }
    
    //Resize vector result to fit output of convolution
    result.clear();
    result.resize(signal.size() + impulse.size(), 0.0);
    std::cout << "res size = " << result.size() << "\n";

    //Perform convolution
    for (int n=0; n < result.size(); n++) {
        for (auto &pair: impulse) {
            const uint64_t k = pair.first;
            float impulse_val = pair.second;
            if ((n-k>=0) && (n-k<signal.size())) {
                result[n] += impulse_val * signal[n-k];
            }
        }
    } 

    return true;
}
