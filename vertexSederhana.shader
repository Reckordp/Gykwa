#version 330 core

in vec2 posisi;

void main() {
	gl_Position = vec4(posisi.x, posisi.y, 0.0, 1.0);
}