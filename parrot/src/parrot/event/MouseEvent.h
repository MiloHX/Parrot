#pragma once

#include "Event.h"

namespace parrot {

    class MouseMovedEvent : public Event {
    public:
        MouseMovedEvent(float x, float y) : m_mouse_x(x), m_mouse_y(y) {}

        inline float getX() const { return m_mouse_x; }
        inline float getY() const { return m_mouse_y; }

        std::string toString() const override {
            std::stringstream ss;
            ss << "MouseMovedEvent: X: " << m_mouse_x << ", Y: " << m_mouse_y;
            return ss.str();
        }

        EVENT_CLASS_TYPE(Mouse_Moved)
        EVENT_CLASS_CATEGORY(EventCategory::Event_Cat_Mouse_Movement | EventCategory::Event_Cat_Input)

    private:
        float m_mouse_x, m_mouse_y;
    };


    class MouseScrolledEvent : public Event {
    public:
        MouseScrolledEvent(float offset_x, float offset_y) : m_offset_x(offset_x), m_offset_y(offset_y) {}

        inline float getOffsetX() const { return m_offset_x; }
        inline float getOffsetY() const { return m_offset_y; }

        std::string toString() const override {
            std::stringstream ss;
            ss << "MouseScrolledEvent: Offset X: " << m_offset_x << ", Offset Y: " << m_offset_y;
            return ss.str();
        }

        EVENT_CLASS_TYPE(Mouse_Scrolled)
        EVENT_CLASS_CATEGORY(EventCategory::Event_Cat_Mouse_Movement | EventCategory::Event_Cat_Input)

    private:
        float m_offset_x, m_offset_y;
    };

    
    class MouseButtonEvent : public Event {
    public:
        inline int getMouseButton() const { return m_button; }

        EVENT_CLASS_CATEGORY(EventCategory::Event_Cat_Mouse_Button | EventCategory::Event_Cat_Input)
    protected:
        MouseButtonEvent(int button) : m_button(button) {} // To be derived, constructor is protected
        int m_button;
    };
    

    class MouseButtonPressedEvent : public MouseButtonEvent {
    public:
        MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

        std::string toString() const override {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent:" << m_button;
            return ss.str();
        }
        EVENT_CLASS_TYPE(Mouse_Button_Pressed)
    };


    class MouseButtonReleasedEvent : public MouseButtonEvent {
    public:
        MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

        std::string toString() const override {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent:" << m_button;
            return ss.str();
        }
        EVENT_CLASS_TYPE(Mouse_Button_Released)
    };
}