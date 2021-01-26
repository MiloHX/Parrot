#pragma once

#include "parrot/Window.h"
#include "parrot/renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace parrot {

    class WindowsWindow : public Window {
    public:
        WindowsWindow(const WindowProps& props);
        virtual ~WindowsWindow();

        void onUpdate() override;

        inline unsigned int getWidth () const override { return m_data.width;  }
        inline unsigned int getHeight() const override { return m_data.height; }

        // Window Attribute
        inline void  setEventCallBack(const EventCallBackFunc& callback) override { m_data.event_callback = callback; }
        inline void* getNativeWindow() const override { return m_window;  }
        void setVSync(bool enabled) override;
        bool isVSync () const override;

    private:
        virtual void init(const WindowProps& props);
        virtual void shutdown();

        GLFWwindow*      m_window;
        GraphicsContext* m_context;

        struct WindowData {
            std::string       title;
            unsigned int      width;
            unsigned int      height;
            bool              vSync;
            EventCallBackFunc event_callback;
        };

        WindowData m_data;
    };
}