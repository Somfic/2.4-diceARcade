#pragma once

#include <gl/glew.h>
#include <string>

class Texture
{
	GLuint id;
public:
	int width;
	int height;
	Texture();
	Texture(const std::string& fileName);
	void reTexture(unsigned char* imgData, int width, int height);
	void bind();
};