cmake_minimum_required(VERSION 3.12)

project(Supecs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# set(CMAKE_CXX_FLAGS_RELEASE "-O4")
# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)
 add_compile_options(-W4 -Wextra -Wpedantic -Wcast-align -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wextra-semi -Wfloat-equal -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wsign-conversion -Wsign-promo)
# add_compile_options(/GR-)

# Define the executable target and its source files
add_executable(Supecs "examples/Game.cpp" )

# Include directories containing header files

target_include_directories(Supecs PRIVATE src)
target_include_directories(Supecs PRIVATE include)
target_compile_options(Supecs PRIVATE)