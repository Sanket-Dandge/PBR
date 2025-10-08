#pragma once

#include <cstdint>
#include <string>

class HDR {
    public:
        HDR(const std::string& path);
        void loadHDR(const std::string& path);
        uint32_t getID() { return m_hdrID; }
    
    private:
        uint32_t m_hdrID;
};
