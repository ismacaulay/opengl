#pragma once

#include <glad/glad.h>
#include <cassert>

void glClearError();
bool glLogCall(const char* function, const char* file, int line);

// Dont call this all the time in a release build
#define GL_CALL(x) glClearError();\
    x;\
    assert(glLogCall(#x, __FILE__, __LINE__))

class VertexArray;
class IndexBuffer;
class Shader;

class Renderer {
public:
    Renderer();
    ~Renderer();

    void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void clear() const;
private:

};
