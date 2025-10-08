#include "Core.h"
#include "config.h"
#include "engine.h"
#include "render.h"
#include <memory>

int main (int argc, char *argv[]) {
    ConfigInfo config;
    config.imguiFontScale = 1.0f;
    config.hdrPath = "resources/Textures/hdr/arches_pinetree.hdr";

    Engine engine(config);
    auto scene = std::make_shared<Scene>();

    LOG("before begin engine");
    engine.begin(scene);
    LOG("after begin engine");

    scene->mLightPositions.push_back(glm::vec3(0.0f, 0.0f, 10.0f));
    scene->mLightColors.push_back(glm::vec3(250.0f, 250.0f, 250.0f));

    auto helmetModel = std::make_shared<Model>("resources/Models/helmet/DamagedHelmet.gltf", false);
    auto helmet = Entity(helmetModel);
    helmet.setOrientation(glm::angleAxis(1.5708f, glm::vec3(1.0f, 0.0f, 0.0f)));
    scene->m_entities.push_back(helmet);


    float spacing = 2.5;
    float values[] = {0.0, 0.3, 0.5, 0.7, 1.0};

    for (auto i = 0; i < 5; i++) {
        for (auto j = 0; j < 5; j++) {
            auto sphereMaterial = std::make_shared<Material>();

            sphereMaterial->useTextureAlbedo = false;
            sphereMaterial->useTextureMetallicRoughness = false;
            sphereMaterial->useTextureNormal = false;
            sphereMaterial->useTextureAmbientOcclusion = false;
            sphereMaterial->useTextureEmissive = false;
            sphereMaterial->albedo = glm::vec3(1.0, 1.0, 1.0);
            sphereMaterial->metallic = values[i];
            sphereMaterial->roughness = values[j];

            auto sphereModel = std::make_shared<Model>("resources/Models/sphere/sphere.gltf", sphereMaterial, true);
            auto sphere = Entity(sphereModel);

            sphere.setPosition(glm::vec3(0.0 + j * spacing, 0.0 + i * spacing, 0.0));

            scene->m_entities.push_back(sphere);
        }
    }

    LOG("before engine run");
    engine.run();
    LOG("after engine run");
    engine.shutdown();

    return 0;
}
