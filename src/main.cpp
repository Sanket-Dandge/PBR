#include "config.h"
#include "engine.h"
#include "render.h"
#include <memory>

int main (int argc, char *argv[]) {
    ConfigInfo config;
    config.imguiFontScale = 1.0f;

    Engine engine(config);
    auto scene = std::make_shared<Scene>();

    engine.begin(scene);

    engine.run();
    engine.shutdown();

    return 0;
}
