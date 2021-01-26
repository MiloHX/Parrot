#pragma once

#include "parrot/Layer.h"
#include "parrot/event/MouseEvent.h"
#include "parrot/event/KeyEvent.h"
#include "parrot/event/ApplicationEvent.h"

namespace parrot {

    class PARROT_API ImGuiLayer : public Layer {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void onAttach() override;
        void onDetach() override;
        void onUpdate() override;
        void onEvent(Event& event) override;
    private:
        bool onMouseButtonPressed (MouseButtonPressedEvent & event);
        bool onMouseButtonReleased(MouseButtonReleasedEvent& event);
        bool onMouseMoved         (MouseMovedEvent         & event);
        bool onMouseScrolled      (MouseScrolledEvent      & event);
        bool onKeyPressed         (KeyPressedEvent         & event);
        bool onKeyReleased        (KeyReleasedEvent        & event);
        bool onKeyTyped           (KeyTypedEvent           & event);
        bool onWindowResized      (WindowResizedEvent      & event);

        float m_time = 0.0f;

    };
   
}