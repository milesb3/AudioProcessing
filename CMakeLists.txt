cmake_minimum_required(VERSION 3.1)

project(
  BeanMusicProcessing
  VERSION 1.0
  LANGUAGES CXX)

add_executable(add_delay add_delay.cpp WavFile.cpp)
add_executable(fill_out fill_out.cpp WavFile.cpp)
add_executable(bitcrush bitcrush.cpp WavFile.cpp)
