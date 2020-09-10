#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 vTex;

out vec3 oPos;
out vec2 v_TexCoord;

void main()
{
    gl_Position = vec4(aPos.x, -aPos.y, aPos.z, 1.0);
    oPos = aPos;
    v_TexCoord = vTex;
}
