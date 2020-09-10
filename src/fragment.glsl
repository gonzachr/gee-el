#version 330 core
out vec4 fragColor;
in vec2 v_TexCoord;
uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
//    fragColor = vec4(ourColor, 1.0);
    vec4 texColor = texture(u_Texture, v_TexCoord);
    fragColor = texColor;
}
