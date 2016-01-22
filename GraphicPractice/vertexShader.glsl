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

#version 330 core

in vec4 vPosition;
in vec4 vColor;

uniform mat4 ModelViewProject;
out vec4 color;

void main()	{
	color = vColor;
	gl_Position = ModelViewProject * vPosition;


}
