#include <iostream>
#include <vector>
#include <unordered_map>

bool convolve(const std::vector<float> &signal, const std::vector<std::pair<uint64_t, float>> &impulse, std::vector<float> &result);