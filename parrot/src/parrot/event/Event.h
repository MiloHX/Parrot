#pragma once

#include "parrot/core/Core.h"

namespace parrot {

    // TODO: Event Buffer

    enum class EventType {
        None = 0,
        Window_Close, Window_Resize, Window_Focus, Window_Lost_Focus, Window_Moved,
        Key_Pressed, Key_Released, Key_Typed,
        Mouse_Button_Pressed, Mouse_Button_Released, Mouse_Moved, Mouse_Scrolled,
    };

    enum EventCategory {
        None                     = 0,
        Event_Cat_Application    = BIT(0),
        Event_Cat_Input          = BIT(1),
        Event_Cat_Keyboard       = BIT(2),
        Event_Cat_Mouse_Movement = BIT(3),
        Event_Cat_Mouse_Button   = BIT(4),
    };

#define EVENT_CLASS_TYPE(type) static  EventType   getStaticType() { return EventType::##type; }\
                               virtual EventType   getEventType() const override { return getStaticType(); }\
                               virtual const char* getName()      const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlag() const override { return category; }

    class Event {
        friend class EventDispatcher;
    public:
        virtual ~Event() = default;
        // Must be implmeneted
        virtual EventType   getEventType()    const = 0;
        virtual const char* getName()         const = 0;
        virtual int         getCategoryFlag() const = 0;

        // Can be overrided
        virtual std::string toString()        const { return getName(); }

        inline bool isInCategory(EventCategory category) { return getCategoryFlag() & category; }
        inline bool isHandled   ()                       { return m_handled; }
    protected:
        bool m_handled = false;
    };


    class EventDispatcher {
        template<typename T>
        using EventFunc = std::function<bool(T&)>;
    public:
        EventDispatcher(Event& event) : m_event(event) {

        }

        template<typename T>
        bool dispatch(EventFunc<T> func) {
            if (m_event.getEventType() == T::getStaticType()) {
                m_event.m_handled = func(*(T*)&m_event);
                return true;
            }
            return false;
        }

    private:
        Event& m_event;
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e) {
        return os << e.toString();
    }
}