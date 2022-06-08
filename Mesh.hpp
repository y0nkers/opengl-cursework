#ifndef MESH_HPP
#define MESH_HPP

#include "Common.hpp"
#include "Program.hpp"

struct Vertex { // Точка
	glm::vec3 Position; // Позиция
	glm::vec3 Normal; // Нормаль из точки
	glm::vec2 TexCoords; // Текстурная координата
	glm::vec3 Tangent; //  
	glm::vec3 Bitangent; //
};

struct TextureMesh { // Текстура
	unsigned int id; // id текстуры
	std::string type; // Тип текстуры
	std::string path; // Путь к файлу с текстурой
};

class Mesh {
public:
	std::vector<Vertex>       vertices; // массив точек
	std::vector<unsigned int> indices; // массив индексов точек
	std::vector<TextureMesh>      textures; // массив текстур
	unsigned int VAO;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureMesh> textures);

	void Draw(Program program, bool isPickingPhase);

private:
	unsigned int VBO, EBO;

	void setupMesh();
};
#endif
