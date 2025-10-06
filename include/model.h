#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader.h"
#include "mesh.h"

#include <string>
#include <vector>

inline unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

class Model {
    public:
        // INFO: stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
        std::vector<Texture> textures_loaded;
        std::vector<Mesh>    meshes;
        std::string directory;
        bool gammaCorrection;

        // INFO: constructor, expects a filepath to a 3D model.
        Model(std::string const &path, bool gamma = false);

        // INFO: draws the model, and thus all its meshes
        void Draw(Shader &shader);

    private:
        // INFO: loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
        void loadModel(std::string const &path);

        // INFO: processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);

        // INFO: checks all material textures of a given type and loads the textures if they're not loaded yet.
        // the required info is returned as a Texture struct.
        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

