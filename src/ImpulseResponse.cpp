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

    //Copy impulse_name to name_check, a string with no spaces for comparing with read_name later
    //TODO !
    std::string name_check(impulse_name);
    std::cout << "Before: " << name_check << "\n";
    for (int i; i<name_check.size(); i++) {
        if (name_check[i] == *" ") {
            std::cout << "'" << name_check[i] << "'\n";
            name_check.erase(i);
        } 
    }
    std::cout << "After: " << name_check << "\n";

    //Search for matching impulse_name in impulse lib file
    std::string read_name = "";
    bool found_name = false;
    int char_index = 0;
    while (char_index < h_file_info.size()) {
        //Ignore lines that start with comment identifier
        if (h_file_info[char_index] == *COMMENT_IDENTIFIER) {
            while ((h_file_info[char_index] != *"\n") && (char_index < h_file_info.size())) {
                char_index++;
            }
            //Reset read_name when moving to next line in impulse_responses.txt
            read_name = "";
        }

        //Reset read_name when entering a new line
        else if (h_file_info[char_index] == *"\n") {
            read_name = "";
        }

        //If we read the opening of the information section, check if we have read a name that matches the input impulse_name
        else if (h_file_info[char_index] == *INFO_SECTION_START_IDENTIFIER) {
            //TODO !
            std::cout << "read name: '" << read_name << "'\n";
        }

        //Add read character to read_name, to check later if it matches the impulse_name input into this constructor
        else if (h_file_info[char_index] != *" ") {
            read_name.push_back(h_file_info[char_index]);
        }

        char_index++;
    }    
}

//Testing
int main() {
    ImpulseResponse test("three echoes test");
}