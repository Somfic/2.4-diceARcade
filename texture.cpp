#include "Texture.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string& fileName)
{

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	stbi_set_flip_vertically_on_load(true);
	int width, height, bpp;
	unsigned char* imgData = stbi_load(fileName.c_str(), &width, &height, &bpp, 4);
	this->width = width;
	this->height = height;
	
	glTexImage2D(GL_TEXTURE_2D,
		0, //level
		GL_RGBA, //internal format
		width, //width
		height, //height
		0, //border
		GL_RGBA, //data format
		GL_UNSIGNED_BYTE, //data type
		imgData);
	stbi_image_free(imgData);//data

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::reTexture(unsigned char* imgData, int width, int height) {
	this->width = width;
	this->height = height;
	glTexImage2D(GL_TEXTURE_2D,
		0, //level
		GL_RGBA, //internal format
		this->width, //width
		this->height, //height
		0, //border
		GL_RGBA, //data format
		GL_UNSIGNED_BYTE, //data type
		imgData);
	stbi_image_free(imgData);
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}
