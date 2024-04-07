cmake_minimum_required(VERSION 3.1)

project(
  BeanMusicProcessing
  VERSION 1.0
  LANGUAGES CXX)

add_executable(add_delay add_delay.cpp WavFile.cpp)
add_executable(fill_out fill_out.cpp WavFile.cpp)
add_executable(bitcrush bitcrush.cpp WavFile.cpp)
add_executable(plot_samples plot_samples.cpp WavFile.cpp)
add_executable(plot_spectrum plot_spectrum.cpp WavFile.cpp)

target_link_libraries(plot_spectrum fftw3)

find_package(Boost 1.74.0 COMPONENTS iostreams system filesystem) 
if(Boost_FOUND)
include_directories(${Boost_INCLUDE_DIRS}) 
target_link_libraries(plot_samples ${Boost_LIBRARIES})
target_link_libraries(plot_spectrum ${Boost_LIBRARIES})
endif()