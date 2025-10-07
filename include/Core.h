#pragma once

#include <cstdio>
#include <cstdint>

#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[32m"

#define LOG(fmt, ...) \
    std::printf(COLOR_GREEN fmt COLOR_RESET "\n", ##__VA_ARGS__)
