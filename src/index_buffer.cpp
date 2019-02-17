#include "index_buffer.h"

#include "renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    : count_(count)
{
    GL_CALL(glGenBuffers(1, &rendererID_));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID_));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() {
    GL_CALL(glDeleteBuffers(1, &rendererID_));
}

void IndexBuffer::bind() const {
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID_));
}

void IndexBuffer::unbind() const {
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
