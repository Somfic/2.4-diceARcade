#include "Texture.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



Texture::Texture(const std::string& fileName)
{

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	int width, height, bpp;
	unsigned char* imgData = stbi_load(fileName.c_str(), &width, &height, &bpp, 4);


	//flipping textures because open gl stores textures upside down.
	for (int i = 0; i < height / 2; i++)
	{
		for (int j = 0; j < width * 4; j++) {
			char s = imgData[i * width * 4 + j];
			imgData[i * width * 4 + j] = imgData[(height - i - 1) * width * 4 + j];
			imgData[(height - i - 1) * width * 4 + j] = s;
		}
	}

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

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}
