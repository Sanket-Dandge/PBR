#include "Core.h"
#include "config.h"
#include "engine.h"
#include "render.h"
#include <memory>

int main (int argc, char *argv[]) {
    ConfigInfo config;
    config.imguiFontScale = 1.0f;
    config.hdrPath = "resources/Textures/hdr/newport_loft.hdr";

    Engine engine(config);
    auto scene = std::make_shared<Scene>();

    LOG("before begin engine");
    engine.begin(scene);
    LOG("after begin engine");

    scene->mLightPositions.push_back(glm::vec3(0.0f, 0.0f, 10.0f));
    scene->mLightColors.push_back(glm::vec3(150.0f, 150.0f, 150.0f));

    auto helmetModel = std::make_shared<Model>("resources/Models/helmet/DamagedHelmet.gltf", false);
    auto helmet = Entity(helmetModel);
    helmet.setOrientation(glm::angleAxis(1.5708f, glm::vec3(1.0f, 0.0f, 0.0f)));
    scene->m_entities.push_back(helmet);

    LOG("before engine run");
    engine.run();
    LOG("after engine run");
    engine.shutdown();

    return 0;
}
