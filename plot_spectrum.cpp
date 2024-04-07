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
    std::vector<std::vector<double>> channels_samples;
    channels_samples.resize(wav.num_channels);
    int num_samp_per_channel = wav.data_size / wav.num_channels;
    for (auto &channel_samples: channels_samples) {
        channel_samples.resize(num_samp_per_channel);
    }
    for (int i=0, j=0; i<wav.data_size; ++j) {
        for (auto &channel_samples: channels_samples) {
            channel_samples[j] = wav.data[i++];
        }
    }

    //Create output fftw array where output of fft will be stored
    std::vector<fftw_complex*> channels_fft;
    channels_fft.resize(wav.num_channels);
    int size_fft_out = (num_samp_per_channel / 2) + 1;
    for (auto &channel_fft: channels_fft) {
        channel_fft = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size_fft_out);
    }

    //Calculate fft
    std::vector<fftw_plan> channels_plan;
    channels_plan.resize(wav.num_channels);
    for (int i=0; i<wav.num_channels; i++) {
        channels_plan[i] = fftw_plan_dft_r2c_1d(num_samp_per_channel, channels_samples[i].data(), channels_fft[i], FFTW_ESTIMATE);
        fftw_execute(channels_plan[i]);
    }

    std::cout << "Calculating PSD plot...\n";
    //Calculate PSD values from DFT. The PSD can be estimated by the square of the magnitued of FFt/DFT
    std::vector<std::vector<std::pair<double, double>>> channels_PSD_plot;
    channels_PSD_plot.resize(wav.num_channels);
    for (auto &channel_PSD_plot: channels_PSD_plot) {
        channel_PSD_plot.resize(size_fft_out);
    }
    for (int i=0; i<size_fft_out; i++) {
        double freq_bin = ((double) i * wav.sample_rate) / num_samp_per_channel;
        for (int j=0; j<wav.num_channels; j++) {
            double PSD_val = 10 * log10((pow(channels_fft[j][i][0] / (double) num_samp_per_channel, 2) + pow(channels_fft[j][i][1] / (double) num_samp_per_channel, 2)) * 2 / (double) wav.sample_rate);
            channels_PSD_plot[j][i] = std::make_pair(freq_bin, PSD_val);
        }
    }

    //Plot using gnuplot-iostream
    std::cout << "Plotting PSD in gnuplot...\n";
    Gnuplot gp;
    gp << "set title 'PSD of " << in_file << "' noenhanced\nshow title\n";
    gp << "set xlabel 'Frequency [Hz]'\nshow xlabel\n";
    gp << "set ylabel 'PSD [dBFs]'\nshow ylabel\n";
    gp << "plot" << gp.file1d(channels_PSD_plot[0]) << "with lines ";
    if (wav.num_channels > 1) {
        gp << "title 'left channel', " << gp.file1d(channels_PSD_plot[1]) << "with lines title 'right channel";
    }
    else {
        gp << "title 'mono'";
    }
    gp << "\n";
}