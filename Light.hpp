#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "Common.hpp"
#include "Program.hpp"

enum class LightType { None = 0, Directional = 1, Point = 2, Spot = 3, Ambient = 4 };

class Light
{
public:
	std::string name;
	bool active;

	LightType type;

	glm::vec3 position;
	glm::vec3 direction;
	float cutOff;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;

	Light(std::string name = "NONE", bool active = false);

	Light(std::string name, bool active, LightType type, glm::vec3 position, glm::vec3 direction, float cutOff,
		glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
		float constant, float linear, float quadratic);

	void initLikePointLight(glm::vec3 position,
		glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
		float constant, float linear, float quadratic);

	void initLikeSpotLight(glm::vec3 position, glm::vec3 direction, float cutOff,
		glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
		float constant, float linear, float quadratic);

	void initLikeDirectionalLight(glm::vec3 direction,
		glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

	void initLikeAmbientLight(glm::vec3 ambient);

	bool isLightOn();
	void turnOn();
	void turnOff();

	int putInShader(Program& lightProgram, int lightNumber);
};

extern const Light NoneLight;
#endif