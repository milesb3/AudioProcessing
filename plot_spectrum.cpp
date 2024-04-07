#include "WavFile.hpp"
#include <fftw3.h>
#include "math.h"
#include "gnuplot-iostream.h"

bool check_user_input(int num_args, char** args_in) {
    if (num_args < 2) {
        std::cout << "Insufficent number of arguments provided. Running this command should take the form:\n" << args_in[0] 
            << " <input wav file>\n"; 
        return false;
    }
    return true;
}

int main(int argc, char** argv) {
    if (!check_user_input(argc, argv)) {
        exit(-1);
    }
    char* in_file = argv[1];
    
    std::cout << "Reading in '" << in_file << "'...\n";
    WavFile wav(in_file);

    if (wav.data_size == 0) {
        exit(-1);
    }

    std::cout << "Executing real to complex fft...\n";
    //Extract wavfile samples into an array of doubles
    double *samples = new double[wav.data_size];
    for (int i=0; i<wav.data_size; i++) {
        samples[i] = (double) wav.data[i];
    }

    //Create output fftw array where output of fft will be stored
    fftw_complex *S_out;
    int size_S_out = (wav.data_size / 2) + 1;
    std::cout << "size of wav data = " << wav.data_size << "\n";
    std::cout << "size_S_out = " << size_S_out << "\n";
    S_out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size_S_out);

    //Calculate fft
    fftw_plan p;
    p = fftw_plan_dft_r2c_1d(wav.data_size, samples, S_out, FFTW_ESTIMATE);
    fftw_execute(p);

    std::cout << "Calculating PSD...\n";
    //Calculate PSD values from DFT. The PSD can be estimated by the square of the magnitued of FFt/DFT
    std::vector<double> PSD;
    PSD.resize(size_S_out);
    for (int i=0; i<size_S_out; i++) {
        //Normalization and conversion to dB are both done here in the same calculation
        PSD.at(i) = 10 * log10((pow(S_out[i][0] / (double) wav.data_size, 2) + pow(S_out[i][1] / (double) wav.data_size, 2)) * 2 / (double) wav.sample_rate);
    }

    //Calculate fft frequency bins (what frequency each value in the fft output is representing power for)
    std::vector<std::pair<double, double>> PSD_plot;
    for (int i=0; i<size_S_out; i++) {
        double freq_bin = ((double) i * wav.sample_rate) / wav.data_size;
        PSD_plot.push_back(std::make_pair(freq_bin, PSD.at(i)));
    }

    //Plot using gnuplot-iostream
    std::cout << "Plotting PSD in gnuplot...\n";
    Gnuplot gp;
    gp << "set title 'PSD of " << in_file << "' noenhanced\nshow title\n";
    gp << "set xlabel 'Frequency [Hz]'\nshow xlabel\n";
    gp << "set ylabel 'PSD [dBFs]'\nshow ylabel\n";
    gp << "plot" << gp.file1d(PSD_plot) << "with lines\n";

    //Free memory
    delete [] samples;
    fftw_free(S_out);
    fftw_destroy_plan(p);
}