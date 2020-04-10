#version 400
out vec4 fragcolor;
in vec3 Tex;
uniform samplerCube cubemap;
void main()
{fragcolor=texture(cubemap,Tex);
}
