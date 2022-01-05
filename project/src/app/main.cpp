#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <defines.h>
#include <engine/data/DataIO.hpp>
#include <engine/data/PerspectiveCamera.hpp>
#include <engine/data/Scene.hpp>
#include <engine/rayCaster.hpp>
#include <engine/tonemapping/reinhard.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <stbipp/ImageExporter.hpp>
#include <stbipp/ImageImporter.hpp>

int main(int argc, char* argv[])
{
    engine::Scene scene{};

    if(!engine::IO::loadScene(std::string(RESOURCE_PATH) + "/CornellBoxCubeLighten.fbx", scene))
    {
        throw std::runtime_error(std::string(__func__) + ": Could not load the given scene!");
    }
    std::cout << "Scene loaded successfully" << std::endl;

    if(!glfwInit())
    {
        std::cerr << "ERROR: could not start GLFW" << std::endl;
        return 1;
    }
    int width = 1024;
    int height = 1024;

    auto* window = glfwCreateWindow(1920, 1080, "Test", nullptr, nullptr);

    if(!window)
    {
        std::cerr << "ERROR: could not open window with GLFW3" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK)
    {
        std::cerr << "ERROR: could not start GLEW" << std::endl;
        return 1;
    }
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 400");
    GLuint textureID{0};
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0.0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, 1000);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, -1000);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    scene.cameras.front()->setRenderDimensions(width, height);
    stbipp::Image frame(width, height, stbipp::Color4f{0.0f});
    stbipp::Image frameDisplayed(width, height, stbipp::Color4f{0.0f});
    int sampleCount = 0;
    float keyVal = 0.72f;
    while(!glfwWindowShouldClose(window))
    {
        // update other events like input handling
        glfwPollEvents();

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        scene.backgroundColor = Vector3::Ones() * 0.0;
        const int maxDepth = 3;
        const int samples = 1;
        auto result = engine::renderScene(scene, 0, samples, maxDepth);
        sampleCount++;
        for(int x = 0; x < result.width(); ++x)
        {
            for(int y = 0; y < result.height(); ++y)
            {
                frame(x, y) += result(x, y);
                frameDisplayed(x, y) = frame(x, y);
                for(auto& chan: frameDisplayed(x, y))
                {
                    chan /= sampleCount;
                }
            }
        }
        tone::reinhard(frameDisplayed, keyVal);

        for(int x = 0; x < result.width(); ++x)
        {
            for(int y = 0; y < result.height(); ++y)
            {
                for(auto& chan: frameDisplayed(x, y))
                {
                    chan = std::min(chan, .999999990f);
                }
            }
        }
        glBindTexture(GL_TEXTURE_2D, textureID);
        {
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_RGB8,
                         width,
                         height,
                         0,
                         GL_RGBA,
                         GL_UNSIGNED_BYTE,
                         frameDisplayed.castData<stbipp::Color4uc>().data());
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();

        {
            int winWidth, winHeight;
            glfwGetWindowSize(window, &winWidth, &winHeight);
            ImGui::SetNextWindowPos(ImVec2(0.0, 0.0));
            ImGui::SetNextWindowSize(ImVec2(winWidth, winHeight));
        }
        ImGui::Begin("MainWindow",
                     NULL,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                       ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
        {
            ImGui::BeginChild("Parameters", ImVec2(600.0f, 0.0f), true);
            {
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                            1000.0f / ImGui::GetIO().Framerate,
                            ImGui::GetIO().Framerate);
                ImGui::SliderFloat("Reinhard key value", &keyVal, 0.0f, 1.0f);
                ImGui::Text("Samples %d", sampleCount);
            }
            ImGui::EndChild();
            ImGui::SameLine();

            ImGui::BeginChild("OpenGL Texture Text", ImVec2(0, 0), true);
            {
                ImGui::Image((void*)(intptr_t)textureID, ImVec2(width, height));
            }
            ImGui::EndChild();
        }
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glDeleteTextures(1, &textureID);

    glfwDestroyWindow(window);
    glfwTerminate();
}
