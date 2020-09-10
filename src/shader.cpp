#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>

#include "renderer.h"
#include "shader.h"


void Shader::Bind() const {
    GlCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const {
    GlCall(glUseProgram(0));
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) : m_VertexPath(vertexPath), m_FragmentPath(fragmentPath), m_RendererID(0)  {
    m_RendererID = CreateShader(vertexPath, fragmentPath);
}

Shader::~Shader()  {
    GlCall(glDeleteProgram(m_RendererID));
}

std::string Shader::ReadFile(const std::string& filepath) {
    // Open file
    std::ifstream stream(filepath);
    // String stream
    std::stringstream sstr;
    // Dump file into sstream
    sstr << stream.rdbuf();
    
    return sstr.str();
}

unsigned int Shader::CreateShader(const std::string& vertexPath, const std::string& fragmentPath) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexPath);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentPath);
    
    GlCall(glAttachShader(program, vs));
    GlCall(glAttachShader(program, fs));
    GlCall(glLinkProgram(program));
    GlCall(glValidateProgram(program));
    
    int result;
    GlCall(glGetProgramiv(program, GL_LINK_STATUS, &result));
    
    if (result == GL_FALSE) {
        int length;
        GlCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
        char *message = (char *)alloca(length * sizeof(char));
        GlCall(glGetProgramInfoLog(program, length, &length, message));
        std::cout << "Failed to link program!" << std::endl;
        std::cout << message << std::endl;
        GlCall(glDeleteProgram(program));
        return 0;
    }
    
    
    GlCall(glDeleteShader(vs));
    GlCall(glDeleteShader(fs));
    
    return program;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    GlCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GlCall(glShaderSource(id, 1, &src, NULL));
    GlCall(glCompileShader(id));
    
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        GlCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char *)alloca(length * sizeof(char));
        GlCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout   << "Failed to compile "
        << (type == GL_VERTEX_SHADER ? "vertex" : "shader")
        << " shader!" << std::endl;
        std::cout << message << std::endl;
        GlCall(glDeleteShader(id));
        return 0;
    }
    
    return id;
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    GlCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}


void Shader::SetUniform1i(const std::string& name, int value) {
    GlCall(glUniform1i(GetUniformLocation(name), value));
}

int Shader::GetUniformLocation(const std::string& name) {
    GlCall(unsigned int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];
    
    if (location == -1)
        std::cout << "Uniform " << name << "does not exist" << std::endl;
    
    m_UniformLocationCache[name] = location;
    return location;
}
