#ifndef MODEL_HPP
#define MODEL_HPP

#include "Common.hpp"
#include "Mesh.hpp"

class Model
{
public:
    std::vector<TextureMesh> textures_loaded; // —писок загруженных текстур
    std::vector<Mesh> meshes; // ћеши
    std::string directory; // ѕуть к папке с моделью и еЄ текстурами
    bool gammaCorrection; // нужна ли коррекци¤ гаммы

    Model(std::string const& path, bool isUV_flipped = true, bool gamma = false);
    void Draw(Program& program, bool isPickingPhase);

private:
    void loadModel(std::string const& path, bool isUV_flipped);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<TextureMesh> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
#endif
