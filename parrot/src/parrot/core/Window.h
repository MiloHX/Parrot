#pragma once

#include "prpch.h"

#include "parrot/core/Core.h"
#include "parrot/event/Event.h"

namespace parrot {

    struct WindowProps {
        std::string  title;
        uint32_t width;
        uint32_t height;

        WindowProps(const std::string& title = "Parrot",
                    uint32_t           width  = 1280,
                    uint32_t           height = 720) : title(title), width(width), height(height) { }
    };

    // Platform independent Interface
    class Window {
    public:
        using EventCallBackFunc = std::function<void(Event&)>;

        virtual ~Window() {}

        // To be implemented
        virtual void         onUpdate()        = 0;
        virtual unsigned int getWidth()  const = 0;
        virtual unsigned int getHeight() const = 0;
        virtual void         setEventCallBack(const EventCallBackFunc& callback) = 0;
        virtual void         setVSync        (bool enalbed)                      = 0;
        virtual bool         isVSync         () const                            = 0;
        virtual void*        getNativeWindow () const                            = 0;

        static Window*       create(const WindowProps& props = WindowProps());
    };

}
