
#version 330 core

in vec4 vPosition;
in vec4 vColor;

//uniform mat4 ModelViewProject;
out vec4 color;

void main()	{
	color = vColor;
	gl_Position = vPosition;


}
