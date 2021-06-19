cmake_minimum_required(VERSION 3.0.0)
project(binutils VERSION 0.1.0)

set(header_sources


)

set(sources
    ${header_sources}

)

add_executable(binutils ${sources} src/source.cpp)
target_include_directories(binutils PRIVATE include)
