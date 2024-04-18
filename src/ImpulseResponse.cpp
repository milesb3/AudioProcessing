#include "ImpulseResponse.hpp"

ImpulseResponse::ImpulseResponse(const char* impulse_name) {
    /*
    Function takes in the name of the desired impulse response, searches for it in the IMPULSE_LIB_FILE 
    (defined in impulse_response.txt) and, if there is a matching impulse response name, creates the 
    vector representing the input impulse response.
    */
   std::string lib_filename = IMPULSE_LIB_FILE;
   std::ifstream f(lib_filename.data(), std::ios::binary);
   if (!f.is_open()) {
        std::cerr << "Could not open '" << lib_filename << "'\n";
        return;
    }
    //Define vector to store read characters from impulse lib file
    std::vector<unsigned char> h_file_info;

    //Reead impulse lib file and store in h_file_info
    f.seekg(0, std::ios::end);
    size_t size = (size_t)f.tellg();
    h_file_info.resize(size);
    f.seekg(0);
    f.read((char*)&h_file_info[0], size);

    //Search for matching impulse_name in impulse lib file
    bool found_name = false;
    int char_index = 0;
    while (char_index < h_file_info.size()) {
        //Ignore lines that start with comment identifier
        if (h_file_info[char_index] == *COMMENT_IDENTIFIER) {
            std::cout << h_file_info[char_index] << "\n";
        }
        char_index++;
    }    
}

//Testing
int main() {
    ImpulseResponse test("three echoes test");
}