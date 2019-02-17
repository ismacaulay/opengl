#pragma once

#include <glad/glad.h>

// #ifdef __APPLE__
// #include <OpenGL/gl3.h>
// #include <OpenGL/glext.h>
// #else
// #include <GL/glew.h>
// #include <GL/gl.h>
// #include <GL/glu.h>
// #endif

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

    void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void clear() const;
private:

};
