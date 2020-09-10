#pragma once
#include "signal.h"

#include "ibuffer.h"
#include "shader.h"
#include "varray.h"

#include <GL/glew.h>

#define ASSERT(x) \
    if (!(x))     \
        raise(SIGTRAP);
#define GlCall(x)   \
    GLClearError(); \
    x;              \
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char *function, const char *file, int line);

class Renderer
{
public:
    void Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const;
    void Clear() const;
};
