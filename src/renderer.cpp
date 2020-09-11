#include "renderer.h"
#include <iostream>

void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout   << "OpenGL Error: "
                    << std::hex << error
                    << function
                    << " " << file
                    << ":" << line
                    << std::endl;
        return false;
    }
    return true;
}

void Renderer::Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const {
    va.Bind();
    ib.Bind();
    shader.Bind();
    GlCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() const {
    GlCall(glClearColor(0.2f, 0.3f, 0.5f, 1.0f));
    GlCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
