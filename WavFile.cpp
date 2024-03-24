#include "WavFile.hpp"

//Define position (as byte offset) of regular wav file header properties
#define RIFF_OFFSET 0
#define FILE_SIZE_OFFSET 4
#define WAVE_OFFSET 8
#define NUM_CHANNELS_OFFSET 22
#define SAMPLE_RATE_OFFSET 24
#define DATA_OR_LIST_OFFSET 36

WavFile::WavFile(const char* filename) {
    //Read in wav file and store in WavFile struct's file_content vector of unsigned chars
    std::ifstream f(filename, std::ios::binary);
    if (!f.is_open()) {
        std::cerr << "Could not open file '" << filename << "'\n";
        return;
    }
    f.seekg(0, std::ios::end);
    size_t size = (size_t)f.tellg();
    file.resize(size);
    f.seekg(0);
    f.read((char*)&file[0], size);

    //Parse header
    if (file.size() < 44) {
        std::cout << "Input file is too small to be a wav file\n";
    }
    //Check for signature "RIFF" and "WAVE" sections of the wav header
    std::string header_check;
    header_check.resize(8);
    memcpy(header_check.data(), file.data(), 4);
    memcpy(header_check.data() + 4, file.data() + WAVE_OFFSET, 4);
    if (header_check != "RIFFWAVE") {
        std::cout << "Error! header_check = '" << header_check << "', but expected 'RIFFWAVE'\n";
        return;
    }

    //Parse out number of channels and sample rate
    num_channels = *(int16_t*)&file[NUM_CHANNELS_OFFSET];
    sample_rate = *(int32_t*)&file[SAMPLE_RATE_OFFSET];

    //Find end of header and start of data section
    header_check = "aaaa"; //Default value to start while
    int byte_offset = DATA_OR_LIST_OFFSET;
    int32_t sect_size = 0; //Keeps track of sub-chunk size for LIST sections and data section size for data section
    
    while (header_check != "data") {
        memcpy(header_check.data(), file.data() + byte_offset, 4);
        byte_offset += 4; //Account for last read
        sect_size = *(int32_t*)&file[byte_offset];
        byte_offset += 4; //Account for last read
        data_start = byte_offset;
        byte_offset += sect_size;
    }

    data = (int16_t*)(file.data() + data_start);
    data_size = *(int32_t*)&file[data_start - 4] / 2;
}

void WavFile::enlarge(int num_new_elem) {
    //Allocates enough space to add num_new_elem int16s to WavFile properties

    //Resize file to fit added number of elements
    file.resize(file.size() + (num_new_elem * 2));
    //Update file size in header
    *(int32_t*)&file[FILE_SIZE_OFFSET] = file.size() - 8;
    //Update data file size in header
    *(int32_t*)&file[data_start - 4] = file.size() - data_start;
    //Reset data pointer as resize will change where wav file data is allocated
    data = (int16_t*)(file.data() + data_start);
    //Set data size accordingly
    data_size += num_new_elem;
}

std::vector<int16_t> WavFile::get_samples() {
    std::vector<int16_t> samples;
    samples.resize(data_size);

    for (int i=0; i<data_size; i++) {
        samples[i] = data[i];
    }

    return samples;
}

void WavFile::print_header() {
    std::string printer;
    printer.resize(4);

    std::cout << "\n----- wav file header -----\n";

    memcpy(printer.data(), file.data(), 4);
    std::cout << printer << "\n";

    std::cout << "File size: " << *(int32_t*)&file[4] << "\n";

    memcpy(printer.data(), file.data() + WAVE_OFFSET, 4);
    std::cout << printer << "\n";

    std::cout << "Length of format: " << *(int32_t*)&file[16] << "\n";

    std::cout << "Type of format: " << *(int16_t*)&file[20] << "\n";

    std::cout << "Number of channels: " << *(int16_t*)&file[22] << "\n";

    std::cout << "Sample rate: " << *(int32_t*)&file[24] << "\n";

    std::cout << "(Sample Rate * BitsPerSample * Channels) / 8: " << *(int32_t*)&file[28] << "\n";

    std::cout << "(BitsPerSample * Channels) / 8.1 - 8 bit mono2 - 8 bit stereo/16 bit mono4 - 16 bit stereo: " << *(int16_t*)&file[32] << "\n";

    std::cout << "Bits per sample: " << *(int16_t*)&file[34] << "\n";

    memcpy(printer.data(), file.data() + DATA_OR_LIST_OFFSET, 4);
    std::cout << printer << "\n";

    int byte_offset = DATA_OR_LIST_OFFSET + 4;
    int32_t sect_size = 0;
    while (printer != "data") {
        sect_size = *(int32_t*)&file[byte_offset];
        byte_offset += 4;
        
        memcpy(printer.data(), file.data() + byte_offset, 4);
        byte_offset += 4;
        sect_size -= 4; //Account for bytes of section we just read
        std::cout << printer << "\n";

        printer.resize(sect_size);
        memcpy(printer.data(), file.data() + byte_offset, sect_size);
        byte_offset += sect_size;
        std::cout << printer << "\n";

        printer.resize(4);
        memcpy(printer.data(), file.data() + byte_offset, 4);
        byte_offset += 4;
        std::cout << printer << "\n";
    }

    std::cout << "Size of data section: " << *(int32_t*)&file[byte_offset] << "\n";

   std::cout << "-----        .        -----\n";
}

void WavFile::write_out (const char* filename) {
    std::ofstream f(filename, std::ios::binary);
    f.write((char*)&file[0], file.size());
}