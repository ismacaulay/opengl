#include "vertex_array.h"

#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"
#include "renderer.h"

VertexArray::VertexArray() {
    GL_CALL(glGenVertexArrays(1, &rendererID_));
}

VertexArray::~VertexArray() {
    GL_CALL(glDeleteVertexArrays(1, &rendererID_));
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
    bind();
    vb.bind();

    unsigned int offset = 0;
    const auto& elements = layout.getElements();
    for (auto i = 0ul; i < elements.size(); i++) {
        const auto& element = elements[i];
        GL_CALL(glEnableVertexAttribArray(i));
        GL_CALL(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset));
        offset += element.count * VertexBufferElement::getSizeOfType(element.type);
    }
}

void VertexArray::bind() const {
    GL_CALL(glBindVertexArray(rendererID_));
}

void VertexArray::unbind() const {
    GL_CALL(glBindVertexArray(0));
}
