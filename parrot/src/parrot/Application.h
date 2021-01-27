#pragma once

#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "event/Event.h"
#include "event/ApplicationEvent.h"
#include "imgui/ImGuiLayer.h"
#include "renderer/Shader.h"
#include "renderer/Buffer.h"
#include "renderer/VertexArray.h"

namespace parrot {

    class Application {
    public:
        Application();
        virtual ~Application();

        void run();
        void onEvent(Event& event);

        void pushLayer  (Layer* layer  );
        void pushOverlay(Layer* overlay);

        inline Window& getWindow() { return *m_window;  }

        inline static Application& get() { return *s_instance; }
    private:
        bool onWindowClose(Event& e);

        static Application* s_instance;

        std::unique_ptr<Window> m_window;
        ImGuiLayer*             m_imgui_layer;
        bool                    m_running = true;
        LayerStack              m_layer_stack;

        std::shared_ptr<VertexArray > m_tr_vertex_array;
        std::shared_ptr<Shader      > m_tr_shader;
        std::shared_ptr<Shader      > m_sq_shader;

        std::shared_ptr<VertexArray > m_sq_vertex_array;

    };

    Application* createApplication();

}

