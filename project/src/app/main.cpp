#include "engine/Renderer.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
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
    int width = 1024;
    int height = 1024;
    std::optional<engine::Scene> sceneOptional{};

    if(!(sceneOptional = engine::IO::loadScene(std::string(RESOURCE_PATH) + "/CornellBoxCubeLighten.fbx")))
    {
        throw std::runtime_error(std::string(__func__) + ": Could not load the given scene!");
    }
    auto& scene = sceneOptional.value();
    scene.cameras.front()->setRenderDimensions(width, height);
    scene.backgroundColor = Vector3::Ones() * 0.0;
    std::cout << "Scene loaded successfully" << std::endl;

    if(!glfwInit())
    {
        std::cerr << "ERROR: could not start GLFW" << std::endl;
        return 1;
    }

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

    int sampleCount = 0;
    float keyVal = 0.72f;

    engine::Renderer renderer;
    engine::RenderProperties renderParams;
    renderParams.maxDepth = 3;
    renderParams.samples = 128;
    renderer.setRenderProperties(renderParams);
    renderer.setScene(scene);
    auto renderBuffer = renderer.createRenderBuffer(scene.cameras.front());
    stbipp::Image result(renderBuffer->result().width(), renderBuffer->result().height());
    while(!glfwWindowShouldClose(window))
    {
        // update other events like input handling
        glfwPollEvents();

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderer.setRenderProperties(renderParams);
        {
            std::shared_lock<std::shared_mutex> lock(renderBuffer->m_readMutex);
            // result = renderBuffer->result();
            result = renderBuffer->result();
        }
        for(auto& color: result)
        {
            color /= renderBuffer->renderedSamples();
        }
        tone::reinhard(result, keyVal);

        for(auto& color: result)
        {
            for(auto& chan: color)
            {
                chan = std::min(chan, 1.0f);
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
                         result.castData<stbipp::Color4uc>().data());
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
                       ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
        {
            if(ImGui::BeginMenuBar())
            {
                if(ImGui::BeginMenu("File"))
                {
                    if(ImGui::MenuItem("Save Rendering"))
                    {
                        if(!stbipp::saveImage("result.jpg", result, stbipp::ImageSaveFormat::RGB))
                        {
                            std::cerr << "Could not save the result render\n";
                        }
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
            ImGui::BeginChild("Parameters", ImVec2(600.0f, 0.0f), true);
            {
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                            1000.0f / ImGui::GetIO().Framerate,
                            ImGui::GetIO().Framerate);
                ImGui::SliderFloat("Reinhard key value", &keyVal, 0.0f, 1.0f);
                ImGui::Text("Samples %d", renderBuffer->renderedSamples());
                {
                    static int samples{static_cast<int>(renderParams.samples)},
                      maxDepth{static_cast<int>(renderParams.maxDepth)};
                    ImGui::DragInt("Samples", &samples);
                    ImGui::DragInt("MaxDepth", &maxDepth);
                    renderParams.samples = samples;
                    renderParams.maxDepth = maxDepth;
                }
            }
            ImGui::EndChild();
            ImGui::SameLine();

            ImGui::BeginChild("OpenGL Texture Text", ImVec2(0, 0), true);
            {
                ImGui::Image((void*)(intptr_t)textureID, ImVec2(result.width(), result.height()));
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
