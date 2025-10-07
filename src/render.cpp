#include "render.h"
#include "glad.h"
#include "model.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

Render::Render(ConfigInfo &config, std::shared_ptr<Window> window, std::shared_ptr<CameraController> camera) 
    : m_config(config), s_window(window), s_camera(camera) {}

void Render::begin(std::shared_ptr<Scene> scene) {
    // m_scene = scene;
    //
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    stbi_set_flip_vertically_on_load(true);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = m_config.imguiFontScale;

    ImGui::StyleColorsDark();
    
    ImGui_ImplGlfw_InitForOpenGL(s_window->getGLFWwindow(), true);
    ImGui_ImplOpenGL3_Init(m_config.glslVersion);

    //  INFO: Framebuffer Loading
    //
    // m_framebuffer = std::make_unique<Framebuffer>(s_window->getWidth(), s_window->getHeight());
    // m_framebuffer->loadFramebuffer();

    //  INFO: Shader Loading
    m_cubeShader = std::make_unique<Shader>("resources/Shaders/cube.vs", "resources/Shaders/cube.fs");
    m_skyboxShader = std::make_unique<Shader>("resources/Shaders/skybox.vs", "resources/Shaders/skybox.fs");

    std::vector<std::string> faces = {
        "resources/Textures/skybox/right.jpg",
        "resources/Textures/skybox/left.jpg",
        "resources/Textures/skybox/top.jpg",
        "resources/Textures/skybox/bottom.jpg",
        "resources/Textures/skybox/front.jpg",
        "resources/Textures/skybox/back.jpg"
    };

    m_skybox = std::make_unique<Skybox>(faces);
    m_cube = std::make_unique<Cube>();
}

void Render::shutdown() {}

void Render::beginImGui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Engine");
}

void Render::render() {
    ImGui::CollapsingHeader("General");
    ImGui::Text("Average FPS %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    // m_framebuffer->bind();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec3 camPosition = s_camera->getCameraPosition();
    glm::mat4 projection = s_camera->getProjectionMatrix((float)s_window->getWidth() / (float)s_window->getHeight());
    glm::mat4 view = s_camera->getViewMatrix();

    m_cubeShader->use();
    m_cubeShader->setVec3("cameraPosition", camPosition);
    m_cubeShader->setMat4("model", glm::mat4(1.0f));
    m_cubeShader->setMat4("view", view);
    m_cubeShader->setMat4("projection", projection);
    m_cube->Draw();

    // skybox (draw this last to avoid running fragment shader in places where objects are present
    m_skyboxShader->use();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 skyboxView = glm::mat4(glm::mat3(view)); // remove translation so skybox is always surrounding camera

    m_skyboxShader->setInt("skybox", 0); // set skybox sampler to slot 0
    m_skyboxShader->setMat4("model", model);
    m_skyboxShader->setMat4("view", skyboxView);
    m_skyboxShader->setMat4("projection", projection);
    // m_skyboxShader->setFloat("bloomBrightnessCutoff", mBloomBrightnessCutoff);
    m_skybox->Draw();

    // draw ImGui
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwPollEvents();
    glfwSwapBuffers(s_window->getGLFWwindow());
}
