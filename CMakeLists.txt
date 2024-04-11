cmake_minimum_required(VERSION 3.1)

project(
  BeanAudioProcessing
  VERSION 1.0
  LANGUAGES CXX)

add_executable(add_delay src/add_delay.cpp src/WavFile.cpp)
add_executable(fill_out src/fill_out.cpp src/WavFile.cpp)
add_executable(bitcrush src/bitcrush.cpp src/WavFile.cpp)
add_executable(add_reverb src/add_reverb.cpp src/WavFile.cpp src/audio_process_lib.cpp)
add_executable(plot_samples src/plot_samples.cpp src/WavFile.cpp)
add_executable(plot_spectrum src/plot_spectrum.cpp src/WavFile.cpp)

target_link_libraries(plot_spectrum fftw3)

find_package(Boost 1.74.0 COMPONENTS iostreams system filesystem) 
if(Boost_FOUND)
include_directories(${Boost_INCLUDE_DIRS}) 
target_link_libraries(plot_samples ${Boost_LIBRARIES})
target_link_libraries(plot_spectrum ${Boost_LIBRARIES})
endif()