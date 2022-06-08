#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include "Texture.hpp"
#include "Shape.hpp"
#include "Program.hpp"

class Skybox : public Shape
{
	Texture texture;
	void init();

public:
	Skybox() { init(); }
	Skybox(const Texture& tex) :texture(tex) { init(); }

	virtual void render(Program& program);

	Texture& getTexture() { return texture; }
};
#endif
