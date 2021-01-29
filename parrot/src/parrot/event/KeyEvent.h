#pragma once

#include "parrot/event/Event.h"

namespace parrot {

    class KeyEvent : public Event {
    public:
        inline int getKeyCode() const { return m_key_code; }

        EVENT_CLASS_CATEGORY(EventCategory::Event_Cat_Keyboard | EventCategory::Event_Cat_Input)
    protected:
        KeyEvent(int key_code) : m_key_code(key_code) {} // To be derived, constructor is protected
        int m_key_code;

    };

    class KeyPressedEvent : public KeyEvent {
    public:
        KeyPressedEvent(int key_code, int repeat_count) : KeyEvent(key_code), m_repeat_count(repeat_count) {}

        inline int getRepeatCount() const { return m_repeat_count; }

        std::string toString() const override {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << m_key_code << " (" << m_repeat_count << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(Key_Pressed)
    private:
        int m_repeat_count;
    };

    class KeyReleasedEvent : public KeyEvent {
    public:
        KeyReleasedEvent(int key_code) : KeyEvent(key_code) {}

        std::string toString() const override {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << m_key_code;
            return ss.str();
        }

        EVENT_CLASS_TYPE(Key_Released)
    };

    class KeyTypedEvent : public KeyEvent {
    public:
        KeyTypedEvent(int key_code) : KeyEvent(key_code) {}

        std::string toString() const override {
            std::stringstream ss;
            ss << "KeyTypedEvent: " << m_key_code;
            return ss.str();
        }

        EVENT_CLASS_TYPE(Key_Typed)
    private:
    };

}