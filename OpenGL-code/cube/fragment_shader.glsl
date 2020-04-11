#version 400
out vec4 fragcolor;
in vec2 Tex;
uniform sampler2D TheTexture;
void main()
{fragcolor=texture(TheTexture,Tex);
}
