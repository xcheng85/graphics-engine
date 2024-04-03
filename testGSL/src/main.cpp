#include <iostream>
#include <gsl/gsl>
#include <algorithm>
#include <cstring>

bool compare_string(const char* left, const char* right) {
    return std::strcmp(left, right) < 0;
}

void print_string(const char* value) {
    std::cout << value << std::endl;
}

int main(int argc, char** argv) {
    gsl::span<char*> arguments(argv, argc);
    std::stable_sort(arguments.begin(), arguments.end(), compare_string);
    std::for_each(arguments.begin(), arguments.end(), print_string);
    return 0;
}