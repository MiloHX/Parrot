#include "prpch.h"
#include "ImGuiLayer.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "parrot/core/Application.h"
#include "parrot/tool/Profiler.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace parrot {

    ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

    ImGuiLayer::~ImGuiLayer() {
    }

    void ImGuiLayer::onAttach() {
        // Setup ImGui Context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;    // Keyboard Control
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Gamepad Control
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;        // Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;      // Mult-Viewport / Platform Windows
        //io.ConfigFlags |= ImGuiViewportFlags_NoTaskBarIcon;
        //io.ConfigFlags |= ImGuiViewportFlags_NoAutoMerge;

        io.FontDefault = io.Fonts->AddFontFromFileTTF("asset/font/Jura/static/Jura-Regular.ttf", 28.0f);
        io.Fonts->AddFontFromFileTTF("asset/font/Jura/static/Jura-Bold.ttf", 28.0f);

        // Setup ImGui Style
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsClassic();
        // ImGui::StyleColorsLight();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        setDarkThemeColors();

        Application& app = Application::get();
        GLFWwindow* window = static_cast<GLFWwindow*>(app.getWindow().getNativeWindow());

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::onDetach() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::onEvent(Event& event) {
        if (m_block_events) {
            ImGuiIO& io = ImGui::GetIO();
            if (event.isInCategory(EventCategory::Event_Cat_Mouse_Button  ) && io.WantCaptureMouse || 
                event.isInCategory(EventCategory::Event_Cat_Mouse_Movement) && io.WantCaptureMouse || 
                event.isInCategory(EventCategory::Event_Cat_Keyboard      ) && io.WantCaptureKeyboard) {
                event.setHandled(true);
            }
        }
    }

    void ImGuiLayer::begin() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::end() {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::get();
        io.DisplaySize = ImVec2(static_cast<float>(app.getWindow().getWidth()), static_cast<float>(app.getWindow().getHeight()));

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

    }

    void ImGuiLayer::setDarkThemeColors() {
        auto& style_colors = ImGui::GetStyle().Colors;
        style_colors[ImGuiCol_WindowBg          ] = ImVec4{  0.1f, 0.10f, 0.11f, 1.0f };

        style_colors[ImGuiCol_Header            ] = ImVec4{  0.2f, 0.20f, 0.21f, 1.0f };
        style_colors[ImGuiCol_HeaderHovered     ] = ImVec4{  0.3f, 0.30f, 0.31f, 1.0f };
        style_colors[ImGuiCol_HeaderActive      ] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

        style_colors[ImGuiCol_Button            ] = ImVec4{  0.2f, 0.20f, 0.21f, 1.0f };
        style_colors[ImGuiCol_ButtonHovered     ] = ImVec4{  0.3f, 0.30f, 0.31f, 1.0f };
        style_colors[ImGuiCol_ButtonActive      ] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

        style_colors[ImGuiCol_FrameBg           ] = ImVec4{  0.2f, 0.20f, 0.21f, 1.0f };
        style_colors[ImGuiCol_FrameBgHovered    ] = ImVec4{  0.3f, 0.30f, 0.31f, 1.0f };
        style_colors[ImGuiCol_FrameBgActive     ] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

        style_colors[ImGuiCol_Tab               ] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
        style_colors[ImGuiCol_TabHovered        ] = ImVec4{ 0.38f, 0.38f, 0.38f, 1.0f };
        style_colors[ImGuiCol_TabActive         ] = ImVec4{ 0.28f, 0.28f, 0.28f, 1.0f };
        style_colors[ImGuiCol_TabUnfocused      ] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
        style_colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.20f, 0.20f, 0.20f, 1.0f };

        style_colors[ImGuiCol_TitleBg           ] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
        style_colors[ImGuiCol_TitleBgActive     ] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
        style_colors[ImGuiCol_TitleBgCollapsed  ] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

    }

    void ImGuiLayer::onImGuiRender() {

        // TODO: Should it be moved to a dedicated layer?
        #if 0
        ImGui::Begin("Profiler");
        for (const ProfileResult& result : Profiler::getResult()) {
            char label[50];
            strcpy(label, "%.3fms ");
            strcat(label, result.name);
            ImGui::Text(label, result.time);
        }
        Profiler::clear();
        ImGui::End();
        #endif
    }

}