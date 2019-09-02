#include "shader.h"

#include <cassert>
#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <string>

void glClearError() {
    while (glGetError() != GL_NO_ERROR)
        ;
}

bool glLogCall(const char* function, const char* file, int line) {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cout << "[opengl error] " << file << "::" << function << ":" << line << ": " << err
                  << std::endl;
        return false;
    }
    return true;
}

// Dont call this all the time in a release build
#define GL_CALL(x)                                                                                 \
    glClearError();                                                                                \
    x;                                                                                             \
    assert(glLogCall(#x, __FILE__, __LINE__))

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) : rendererID_(0) {
    std::string vertexSource = loadShader(vertexPath);
    std::string fragmentSource = loadShader(fragmentPath);
    rendererID_ = createShader(vertexSource, fragmentSource);
}

Shader::~Shader() {
    GL_CALL(glDeleteProgram(rendererID_));
}

void Shader::bind() const {
    GL_CALL(glUseProgram(rendererID_));
}

void Shader::unbind() const {
    GL_CALL(glUseProgram(0));
}

void Shader::setUniform1f(const std::string& name, float v) {
    GL_CALL(glUniform1f(getUniformLocation(name), v));
}

void Shader::setUniform3f(const std::string& name, float v0, float v1, float v2) {
    GL_CALL(glUniform3f(getUniformLocation(name), v0, v1, v2));
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    GL_CALL(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

std::string Shader::loadShader(const std::string& path) {
    std::ifstream file(path);
    file.open(path);

    std::stringstream ss;
    ss << file.rdbuf();
    file.close();
    return ss.str();
}

unsigned int Shader::createShader(std::string& vertexShader, std::string& fragmentShader) {
    GL_CALL(unsigned int program = glCreateProgram());
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GL_CALL(glAttachShader(program, vs));
    GL_CALL(glAttachShader(program, fs));
    GL_CALL(glLinkProgram(program));
    GL_CALL(glValidateProgram(program));

    GL_CALL(glDeleteShader(vs));
    GL_CALL(glDeleteShader(fs));

    return program;
}

unsigned int Shader::compileShader(unsigned int type, std::string& source) {
    GL_CALL(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GL_CALL(glShaderSource(id, 1, &src, nullptr));
    GL_CALL(glCompileShader(id));

    int result;
    GL_CALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        GL_CALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = static_cast<char*>(alloca(length * sizeof(char)));
        GL_CALL(glGetShaderInfoLog(id, length, &length, message));

        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                  << std::endl;
        std::cout << message << std::endl;
        GL_CALL(glDeleteShader(id));
        return 0;
    }

    return id;
}

int Shader::getUniformLocation(const std::string& name) {
    auto search = uniformLocationCache_.find(name);
    if (search != uniformLocationCache_.end()) {
        return search->second;
    }

    GL_CALL(int location = glGetUniformLocation(rendererID_, name.c_str()));
    if (location == -1) {
        std::cout << "Warning: uniform '" << name << "' does not exist" << std::endl;
    }
    uniformLocationCache_[name] = location;
    return location;
}
