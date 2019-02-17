#include "vertex_buffer_layout.h"

template<>
void VertexBufferLayout::push<float>(unsigned int count) {
    elements_.push_back({ GL_FLOAT, count, GL_FALSE });
    stride_ += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
}

template<>
void VertexBufferLayout::push<unsigned int>(unsigned int count) {
    elements_.push_back({ GL_UNSIGNED_INT, count , GL_FALSE });
    stride_ += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
}

template<>
void VertexBufferLayout::push<unsigned char>(unsigned int count) {
    elements_.push_back({ GL_UNSIGNED_BYTE, count , GL_TRUE });
    stride_ += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
}
