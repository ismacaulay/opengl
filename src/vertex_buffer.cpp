#include "vertex_buffer.h"

#include "renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
    GL_CALL(glGenBuffers(1, &rendererID_));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, rendererID_));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
    GL_CALL(glDeleteBuffers(1, &rendererID_));
}

void VertexBuffer::bind() const {
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, rendererID_));
}

void VertexBuffer::unbind() const {
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
