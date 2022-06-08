#ifndef TEXTURE_LOADER_HPP
#define TEXTURE_LOADER_HPP 1

#include "Common.hpp"

class TextureLoader
{
	static std::string getError(const std::string& fileName);
	static GLenum getFormat(const unsigned& nrChannels);

public:
	static void loadTexture(unsigned& textureId, const std::string& fileName, GLenum type);
	static void loadTexture2D(unsigned& textureId, const std::string& fileName);
	static void loadCubeMapTexture(unsigned& textureId, const std::vector<std::string>& fileNames);
};

#endif
