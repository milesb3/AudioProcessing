#include "WavFile.hpp"
#include "gnuplot-iostream.h"
#include <boost/tuple/tuple.hpp>

bool check_user_input(int num_args, char** args_in) {
    if (num_args < 2) {
        std::cout << "Insufficent number of arguments provided. Running this command should take the form:\n" << args_in[0] 
            << " <input wav file>\n"; 
        return false;
    }
    return true;
}

int main(int argc, char** argv) {
    //NOTE: this plotting is only correct for wav files with 1 channel
    if (!check_user_input(argc, argv)) {
        exit(-1);
    }
    char* in_file = argv[1];
    
    std::cout << "Reading in '" << in_file << "'...\n";
    WavFile wav(in_file);

    if (wav.data_size == 0) {
        exit(-1);
    }

    std::cout << "Creating sample plot vectors...\n";
    //Extract sample vector
    std::vector<float> samples = wav.get_samples();

    //Create vector of indexes
    std::vector<double> sample_indexes;
    std::vector<std::pair<double, double>> samp_plot;
    for (double i=0; i<samples.size(); i++) {
        samp_plot.push_back(std::make_pair(i, (double) samples.at((int) i)));
    }

    std::cout << "Plotting vectors in gnuplot...\n";
    Gnuplot gp;
    gp << "plot" << gp.file1d(samp_plot) << "with lines\n";
}