#include <print>

auto main() -> int {
    std::println("Hello, world!");

    constexpr size_t SIZE = 10;
    int *stuffs = new int[SIZE];
    stuffs[0] = 1;

    delete[] stuffs;

    return 0;
}
