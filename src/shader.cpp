#include "shader.h"

#include <GL/glew.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "renderer.h"

struct ShaderProgramSource {
    std::string vertexSource;
    std::string fragmentSource;
};

Shader::Shader(const std::string& filepath)
    : rendererID_(0)
    , filepath_(filepath)
{
    ShaderProgramSource source = parseShader(filepath);
    rendererID_ = createShader(source.vertexSource, source.fragmentSource);
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

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    GL_CALL(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

ShaderProgramSource Shader::parseShader(const std::string& path) {
    std::ifstream stream(path);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::stringstream ss[2];
    ShaderType mode = ShaderType::NONE;

    std::string line;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                mode = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                mode = ShaderType::FRAGMENT;
            }
        } else {
            ss[static_cast<int>(mode)] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::createShader(std::string& vertexShader, std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

unsigned int Shader::compileShader(unsigned int type, std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = static_cast<char*>(alloca(length * sizeof(char)));
        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
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
