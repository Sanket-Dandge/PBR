#pragma once

#include "cube.h"
#include "hdr.h"
#include "shader.h"
#include <memory>
#include <string>

class HDRCube {
    public:
        HDRCube(const std::string& hdrPath);
        void Draw(Shader& shader);

    private:
        std::unique_ptr<Cube> m_cube;
        HDR m_hdr;

};
