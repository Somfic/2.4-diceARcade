#pragma once

#include <gl/glew.h>
#include <string>

class Texture
{
	GLuint id;
public:
	int witdh;
	int height;
	Texture(const std::string& fileName);
	void reTexture(unsigned char* imgData);
	void bind();
};