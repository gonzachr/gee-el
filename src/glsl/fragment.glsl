#version 330 core
out vec4 fragColor;

// in vec2 v_TexCoord;

uniform vec4 u_Color;

uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;

void main()
{
    // vec4 texColor1 = texture(u_Texture1, v_TexCoord);
    // vec4 texColor2 = texture(u_Texture2, v_TexCoord);
    // vec4 texColor = mix(texColor1, texColor2, 0.2);
    fragColor = u_Color;
}
