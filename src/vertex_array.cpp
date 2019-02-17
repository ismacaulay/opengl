#include "vertex_array.h"

#ifdef __APPLE__
#define CHECK_VAO_SUPPORTED(x) x;
#else
// #include <GL/glew.h>
#define CHECK_VAO_SUPPORTED(x)\
    if(GLEW_ARB_vertex_array_object) {\
        x;\
    }
#endif

#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"
#include "renderer.h"

VertexArray::VertexArray() {
    CHECK_VAO_SUPPORTED(GL_CALL(glGenVertexArrays(1, &rendererID_)))
}

VertexArray::~VertexArray() {
    CHECK_VAO_SUPPORTED(GL_CALL(glDeleteVertexArrays(1, &rendererID_)))
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
    CHECK_VAO_SUPPORTED(GL_CALL(glBindVertexArray(rendererID_)))
}

void VertexArray::unbind() const {
    CHECK_VAO_SUPPORTED(GL_CALL(glBindVertexArray(0)))
}
