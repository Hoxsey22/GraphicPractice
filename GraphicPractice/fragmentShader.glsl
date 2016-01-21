/*
# version 330 core

in vec4 color;
out vec4 fragColor;

void main() {
  fragColor = color;
}
*/

# version 330 core

in vec2 vs_texCoord;

out vec4 color;

in vec3 vs_worldpos;

uniform sampler2D Texture;

// define light properties
uniform vec4 color_ambient = vec4(0.2, 0.2, 0.2, 1.0);

void main(void) {
	float ambient = 1.0f; // scale the ambient light
	vec4 texColor = texture(Texture, vs_texCoord);
	color = ambient * texColor;
}