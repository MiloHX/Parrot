#include "prpch.h"
#include "ImGuiLayer.h"
#include "platform/OpenGL/ImGuiOpenGLRenderer.h"
#include "parrot/Application.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace parrot {

    ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

    ImGuiLayer::~ImGuiLayer() {
    }

    void ImGuiLayer::onAttach() {
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        // Temporary:
        io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
        io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
        io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
        io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
        io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
        io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
        io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
        io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
        io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
        io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
        io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
        io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
        io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
        io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
        io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
        io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
        io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
        io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
        io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
        io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::onDetach() {

    }

    void ImGuiLayer::onUpdate() {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::get();
        io.DisplaySize = ImVec2(app.getWindow().getWidth(), app.getWindow().getHeight());

        float time = (float)glfwGetTime();
        io.DeltaTime = m_time > 0.0f ? (time - m_time) : (1.0f / 60.0f);
        m_time = time;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImGuiLayer::onEvent(Event& event) {
        EventDispatcher dispatcher(event);
        dispatcher.dispatch<MouseButtonPressedEvent >(PR_BIND_EVENT_FUNC(ImGuiLayer::onMouseButtonPressed ));
        dispatcher.dispatch<MouseButtonReleasedEvent>(PR_BIND_EVENT_FUNC(ImGuiLayer::onMouseButtonReleased));
        dispatcher.dispatch<MouseMovedEvent         >(PR_BIND_EVENT_FUNC(ImGuiLayer::onMouseMoved         ));
        dispatcher.dispatch<MouseScrolledEvent      >(PR_BIND_EVENT_FUNC(ImGuiLayer::onMouseScrolled      ));
        dispatcher.dispatch<KeyPressedEvent         >(PR_BIND_EVENT_FUNC(ImGuiLayer::onKeyPressed         ));
        dispatcher.dispatch<KeyReleasedEvent        >(PR_BIND_EVENT_FUNC(ImGuiLayer::onKeyReleased        ));
        dispatcher.dispatch<KeyTypedEvent           >(PR_BIND_EVENT_FUNC(ImGuiLayer::onKeyTyped           ));
        dispatcher.dispatch<WindowResizedEvent      >(PR_BIND_EVENT_FUNC(ImGuiLayer::onWindowResized      ));
    }

    bool ImGuiLayer::onMouseButtonPressed(MouseButtonPressedEvent& event) {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[event.getMouseButton()] = true;
        return false;
    }

    bool ImGuiLayer::onMouseButtonReleased(MouseButtonReleasedEvent& event) {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[event.getMouseButton()] = false;
        return false;
    }

    bool ImGuiLayer::onMouseMoved(MouseMovedEvent& event) {
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2(event.getX(), event.getY());
        return false;
    }

    bool ImGuiLayer::onMouseScrolled(MouseScrolledEvent& event) {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheel  += event.getOffsetY();
        io.MouseWheelH += event.getOffsetX();
        return false;
    }

    bool ImGuiLayer::onKeyPressed(KeyPressedEvent& event) {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[event.getKeyCode()] = true;
        io.KeyCtrl  = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT  ] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT  ];
        io.KeyAlt   = io.KeysDown[GLFW_KEY_LEFT_ALT    ] || io.KeysDown[GLFW_KEY_RIGHT_ALT    ];
        io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER  ] || io.KeysDown[GLFW_KEY_RIGHT_SUPER  ];
        return false;
    }

    bool ImGuiLayer::onKeyReleased(KeyReleasedEvent& event) {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[event.getKeyCode()] = false;
        return false;
    }

    bool ImGuiLayer::onKeyTyped(KeyTypedEvent& event) {
        ImGuiIO& io = ImGui::GetIO();
        int key_code = event.getKeyCode();
        if (key_code > 0 && key_code < 0x10000) {
            io.AddInputCharacter((unsigned short)key_code);
        }
        return false;
    }

    bool ImGuiLayer::onWindowResized(WindowResizedEvent& event) {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(event.getWidth(), event.getHeight());
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        glViewport(0, 0, event.getWidth(), event.getHeight());
        return false;
    }
}