#pragma once

#include <string>
#include <unordered_map>

struct ShaderProgramSource;

class Shader {
public:
    Shader(const std::string& filepath);
    ~Shader();

    void bind() const;
    void unbind() const;

    void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
    unsigned int createShader(std::string& vertexShader, std::string& fragmentShader);
    unsigned int compileShader(unsigned int type, std::string& source);
    ShaderProgramSource parseShader(const std::string& path);

    int getUniformLocation(const std::string& name);

private:
    unsigned int rendererID_;
    const std::string filepath_;
    std::unordered_map<std::string, int> uniformLocationCache_;
};