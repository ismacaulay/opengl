#pragma once

#ifndef __APPLE__
#include <GL/glew.h>
#endif
#include <cassert>

// Dont call this all the time in a release build
#define GL_CALL(x) glClearError();\
    x;\
    assert(glLogCall(#x, __FILE__, __LINE__))

void glClearError();
bool glLogCall(const char* function, const char* file, int line);


class VertexArray;
class IndexBuffer;
class Shader;

class Renderer {
public:

    void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void clear() const;
private:

};
