#include "pch.h"

#include "engine/glCore.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "engine/world/World.h"
#include "engine/world/GameObject.h"
#include "engine/core/Time.h"
#include "engine/core/WindowInfo.h"
#include "engine/core/InputManager.h"
#include "engine/renderer/Renderer.h"
#include "engine/renderer/effects/ssao.h"
#include "engine/events/EventManager.h"
#include "engine/concepts/AssetManager.h"
#include "engine/concepts/AttributeManager.h"
#include "engine/dbg/OpenGLDebug.h"

//#include <crtdbg.h>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WindowInfo::width, WindowInfo::height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    InputManager::SetWindow(window);
    glfwSetKeyCallback(window, InputManager::KeyCallback);
    glfwSetMouseButtonCallback(window, InputManager::MouseButtonCallback);
    glfwSetCursorPosCallback(window, InputManager::CursorPosCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Asset management
    AssetManager::AddShader(std::make_shared<Shader>("res/shaders/Blank.shader"), "ReplacementShader"); //! Split stuff?
    AssetManager::AddShader(std::make_shared<Shader>("res/shaders/Standard.shader"), "Standard");
    AssetManager::AddShader(std::make_shared<Shader>("res/shaders/UI.shader"), "UI");

    Renderer::Init();
    //Renderer::EnableVisibleColliders();
    Renderer::SetSSAO(std::make_unique<SSAO>(WindowInfo::width, WindowInfo::height));
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsLight();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

    World world;
    world.Init();

    Time::Start();
    EventManager::Start();
    InputManager::InitMousePos(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        Time::UpdateTimestep();
        Time::Update();
        InputManager::Update();
        EventManager::Update();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        world.Render();

        if (InputManager::KeyPressed(GLFW_MOUSE_BUTTON_3))
            break;

        InputManager::Reset();
        glfwSwapBuffers(window);
        glfwPollEvents(); //! Move to top?
    }
    //! Improve this system
    world.Close();
    Renderer::Close();
    AssetManager::Close();
    EventManager::Close();
    AttributeManager::Close();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    glfwTerminate();
    //_CrtDumpMemoryLeaks();
    return 0;
}