/*
# version 430 core

in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;  // not used

uniform mat4 ModelViewProjection;  // = projection * view * model
out vec4 color;

void main() {
  color = vColor;
  gl_Position = ModelViewProjection * vPosition;
}
*/

# version 330 core

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec2 vTexCoord;

uniform mat4 MVP;

out vec2 vs_texCoord;
out vec3 vs_worldpos;

void main(void) {
	vec4 position = MVP * vPosition;
	gl_Position = position;
	vs_worldpos = position.xyz;
	vs_texCoord = vTexCoord;
}

