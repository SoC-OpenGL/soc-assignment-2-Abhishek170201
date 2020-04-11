#version 400
layout(location = 0) in vec3 aPos;
layout(location=1) in vec2 aTex;
layout(location=2) in vec3 aNormal;
out vec2 Tex;
out vec3 Normal;
out vec3 fragpos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;
void main()
{gl_Position=projection*view*model*vec4(aPos,1.0f);
 Tex=aTex;
 normal=mat3(transpose(inverse(model)))*aNormal;
 fragpos=vec3(model*vec4(aPos,1.0f));
}
