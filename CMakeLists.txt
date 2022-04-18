cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(paillier LANGUAGES CXX)

# Use -std=c++14 as default.
set(CMAKE_CXX_STANDARD 14)
# Disable C++ extensions
set(CMAKE_CXX_EXTENSIONS OFF)
# Require full C++ standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)


aux_source_directory(./src SOURCE_DIR)
include_directories(./include)
add_executable(paillier paillier.cpp ${SOURCE_DIR})


target_link_libraries(paillier gmp)

