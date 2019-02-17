#include "texture.h"

#include "renderer.h"
#include "vendor/stb_image/stb_image.h"

#include <iostream>
Texture::Texture(const std::string& filepath)
    : rendererID_(0)
    , filepath_(filepath)
    , localBuffer_(nullptr)
    , width_(0)
    , height_(0)
    , bpp_(0)
{
    stbi_set_flip_vertically_on_load(1); // flip upside down. opengl starts at bottom left
    localBuffer_ = stbi_load(filepath.c_str(), &width_, &height_, &bpp_, 4); // 4 for rgba

    GL_CALL(glGenTextures(1, &rendererID_));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, rendererID_));

    // required params
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer_));

    unbind();

    if (localBuffer_) {
        stbi_image_free(localBuffer_);
    }
}

Texture::~Texture() {
    GL_CALL(glDeleteTextures(1, &rendererID_));
}

void Texture::bind(unsigned int slot) const {
    GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, rendererID_));
}

void Texture::unbind() const {
    GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}
