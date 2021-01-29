#pragma once

#include "parrot/event/Event.h"

namespace parrot {

    class  WindowResizedEvent : public Event {
    public:
        WindowResizedEvent(unsigned int width, unsigned int height) : m_width(width), m_height(height) {}

        inline unsigned int getWidth () const { return m_width ; }
        inline unsigned int getHeight() const { return m_height; }

        std::string toString() const override {
            std::stringstream ss;
            ss << "WindowResizedEvent: Width: " << m_width << ", Height: " << m_height;
            return ss.str();
        }

        EVENT_CLASS_TYPE(Window_Resize)
        EVENT_CLASS_CATEGORY(EventCategory::Event_Cat_Application)

    private:
        unsigned int m_width, m_height;
    };

    
    class  WindowCloseEvent : public Event {
    public:
        WindowCloseEvent() {}

        EVENT_CLASS_TYPE(Window_Close)
        EVENT_CLASS_CATEGORY(EventCategory::Event_Cat_Application)
    }; 


    class  WindowFocusEvent : public Event {
    public:
        WindowFocusEvent() {}

        EVENT_CLASS_TYPE(Window_Focus)
        EVENT_CLASS_CATEGORY(EventCategory::Event_Cat_Application)
    }; 


    class  WindowLostFocusEvent : public Event {
    public:
        WindowLostFocusEvent() {}

        EVENT_CLASS_TYPE(Window_Lost_Focus)
        EVENT_CLASS_CATEGORY(EventCategory::Event_Cat_Application)
    }; 


    class  WindowMovedEvent : public Event {
    public:
        WindowMovedEvent() {}

        EVENT_CLASS_TYPE(Window_Moved)
        EVENT_CLASS_CATEGORY(EventCategory::Event_Cat_Application)
    }; 
}
