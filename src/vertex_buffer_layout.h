#pragma once
#include <glad/glad.h>
// #ifdef __APPLE__
// #include <OpenGL/gl3.h>
// #else
// #include <GL/glew.h>
// #endif

#include <cassert>
#include <iostream>
#include <vector>

struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int getSizeOfType(unsigned int type) {
        switch(type) {
            case GL_FLOAT:
                return 4;
            case GL_UNSIGNED_INT:
                return 4;
            case GL_UNSIGNED_BYTE:
                return 1;
            default:
                std::cout << "Unknown type: " << type << std::endl;
        }

        assert(false);
        return 0;
    }
};

class VertexBufferLayout {
public:
    VertexBufferLayout()
        : stride_(0)
    {}

    template<typename T>
    void push(unsigned int count) {
        assert(false);
    }

    inline const std::vector<VertexBufferElement>& getElements() const { return elements_; }
    inline unsigned int getStride() const { return stride_; }

private:
    std::vector<VertexBufferElement> elements_;
    unsigned int stride_;
};


template<>
void VertexBufferLayout::push<float>(unsigned int count);
template<>
void VertexBufferLayout::push<unsigned int>(unsigned int count);
template<>
void VertexBufferLayout::push<unsigned char>(unsigned int count);
