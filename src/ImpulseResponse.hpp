#include <iostream>
#include <fstream>
#include <vector>

#define IMPULSE_LIB_FILE "src/impulse_responses.txt"
#define COMMENT_IDENTIFIER "#"

class ImpulseResponse {
    public:
        std::string name; //Name of impulse response
        std::vector<std::pair<uint64_t, float>> vector; //vector containing impulse response. Vector of pairs to be more efficient for impulse responses with many zeroes

        ImpulseResponse(const char*);
        bool reset_impulse(const char*);
};