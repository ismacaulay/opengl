#include "renderer.h"

#include <iostream>

void glClearError() {
    while(glGetError() != GL_NO_ERROR);
}

bool glLogCall(const char* function, const char* file, int line) {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cout << "[opengl error] " << file << "::" << function << ":" << line << ": " << err << std::endl;
        return false;
    }
    return true;
}
