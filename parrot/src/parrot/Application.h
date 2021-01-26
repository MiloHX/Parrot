#pragma once

#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "event/Event.h"
#include "event/ApplicationEvent.h"

namespace parrot {

    class PARROT_API Application {
    public:
        Application();
        virtual ~Application();

        void run();
        void onEvent(Event& event);

        void pushLayer  (Layer* layer  );
        void pushOverlay(Layer* overlay);

        inline Window& getWindow() { return *m_window;  }

        inline static Application& get() { return *s_Instance; }
    private:
        bool onWindowClose(Event& e);

        std::unique_ptr<Window> m_window;
        bool                    m_running = true;
        LayerStack              m_layer_stack;

        static Application* s_Instance;
    };

    Application* createApplication();

}

