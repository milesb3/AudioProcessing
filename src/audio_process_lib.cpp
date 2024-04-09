#include "audio_process_lib.hpp"

bool convolve(const std::vector<float> signal, const std::vector<float> impulse, std::vector<float> &result) {
    //Resize vector result to fit output of convolution
    result.resize(signal.size() + impulse.size());

    //Let n >= m
    int n = signal.size()>=impulse.size()? signal.size() : impulse.size();
    int m = signal.size()>=impulse.size()? impulse.size() : signal.size();

    

    return true;
}
