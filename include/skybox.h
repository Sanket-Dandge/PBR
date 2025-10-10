#pragma once

#include "cube.h"
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

class Skybox {
    public:
        Skybox(std::vector<std::string> faces);
        Skybox(uint32_t textureId);
        void loadCubemap(std::vector<std::string> faces);
        void Draw();

    private:
        uint32_t m_textureID;
        std::unique_ptr<Cube> m_Cube;
};
