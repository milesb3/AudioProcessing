#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <cstdint>

class WavFile {
    public:
        std::vector<unsigned char> file; //vector containing binary characters of read wav file
        int16_t num_channels = 0; //int containing the number of channels in audio data
        int32_t sample_rate = 0; //int containing the sample rate of the data
        int data_start = 0; //index of file where data starts (where the header ends)
        int data_size = 0; //int containing the size of the data vector

        int16_t* data = NULL; //int16 pointer to where the data starts in file

        WavFile(const char*);
        void enlarge(int);
        std::vector<int16_t> get_samples();
        void print_header();
        void write_out(const char*);

};