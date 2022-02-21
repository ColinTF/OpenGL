#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in float aRot;

out vec3 color;

uniform float scale;

void main() {

	vec3 pos = -aPos*scale;

	vec3 pos2;

	pos2.x = pos.x*cos(aRot*3.14) - pos.y*sin(aRot*3.14);
	pos2.y = pos.x*sin(aRot*3.14) + pos.y*cos(aRot*3.14);

	gl_Position = vec4(pos2.x, pos2.y, pos.z, 1.0);
	color = aColor;
}