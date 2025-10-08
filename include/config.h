#pragma once

#include <string>

struct ConfigInfo {
    float imguiFontScale = 1.0f;
    const char* glslVersion = "#version 330 core";
    std::string hdrPath;
};

