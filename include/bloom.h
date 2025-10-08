#pragma once

class Bloom {
public:
    Bloom(int width, int height);
    void loadBloomFramebuffer();   
    void bind();
    void resize(int width, int height);
    void setMipLevel(int mipLevel);
    unsigned int getColorTextureId() { return m_textureColorID; }

private:
	int m_width, m_height;
    int m_mipLevel = 0;
	unsigned int m_framebufferID;
	unsigned int m_textureColorID;
};
