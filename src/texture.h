#pragma once
#include <string>

class Texture {
public:
    Texture(const std::string& filepath);
    ~Texture();

    void bind(unsigned int slot = 0) const;
    void unbind() const;

    inline int getWidth() const { return width_; }
    inline int getHeight() const { return height_; }

private:
    unsigned int rendererID_;
    std::string filepath_;
    unsigned char* localBuffer_;
    int width_, height_, bpp_;
};
