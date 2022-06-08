#ifndef PICKING_TEXTURE_HPP
#define PICKING_TEXTURE_HPP

#include "Common.hpp"

class PickingTexture
{
public:
	PickingTexture() {}
	~PickingTexture();

	void Init(unsigned int WindowWidth, unsigned int WindowHeight);
	void EnableWriting();
	void DisableWriting();

	struct PixelInfo {
		unsigned ObjectID = 0;
		unsigned DrawID = 0;
		unsigned PrimID = 0;
	};

	PixelInfo ReadPixel(unsigned int x, unsigned int y);

private:
	GLuint m_fbo = 0;
	GLuint m_pickingTexture = 0;
	GLuint m_depthTexture = 0;
};
#endif
