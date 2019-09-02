#pragma once

#include <string>
#include <unordered_map>

class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void bind() const;
    void unbind() const;

    void setUniform1f(const std::string& name, float v);
    void setUniform3f(const std::string& name, float v0, float v1, float v2);
    void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
    unsigned int createShader(std::string& vertexShader, std::string& fragmentShader);
    unsigned int compileShader(unsigned int type, std::string& source);
    std::string loadShader(const std::string& path);

    int getUniformLocation(const std::string& name);

private:
    unsigned int rendererID_;
    std::unordered_map<std::string, int> uniformLocationCache_;
};
