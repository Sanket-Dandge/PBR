#pragma once

#include <cstdio>

#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[32m"

#define LOG(fmt, ...) \
    std::printf("\n");(COLOR_GREEN fmt "\n" COLOR_RESET, ##__VA_ARGS__)
