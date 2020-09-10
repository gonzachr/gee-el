#pragma once

#include <string>
#include <unordered_map>

class Shader
{
private:
    std::string m_VertexPath;
    std::string m_FragmentPath;
    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache;
    
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();
    
    void Bind() const;
    void Unbind() const;
    static std::string ReadFile(const std::string& filepath);
    
    // Set uniforms
    void SetUniform1i(const std::string& name, int value);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
private:
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexPath, const std::string& fragmentPath);
    int GetUniformLocation(const std::string& name);
};
