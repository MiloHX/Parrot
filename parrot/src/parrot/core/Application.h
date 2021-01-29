#pragma once

#include "parrot/core/Core.h"
#include "parrot/core/Window.h"
#include "parrot/core/LayerStack.h"
#include "parrot/event/Event.h"
#include "parrot/event/ApplicationEvent.h"
#include "parrot/imgui/ImGuiLayer.h"
#include "parrot/core/TimeStep.h"

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
        bool onWindowClose  (WindowCloseEvent  & event);
        bool onWindowResized(WindowResizedEvent& event);

        static Application* s_instance;

        TimeStep    m_time_step;
        float       m_last_frame_time;

        Ref<Window> m_window;
        ImGuiLayer* m_imgui_layer;
        bool        m_running   = true;
        bool        m_minimized = false;
        LayerStack  m_layer_stack;
    };

    Application* createApplication();

}

