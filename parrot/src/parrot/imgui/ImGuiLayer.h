#pragma once

#include "parrot/core/Layer.h"
#include "parrot/event/MouseEvent.h"
#include "parrot/event/KeyEvent.h"
#include "parrot/event/ApplicationEvent.h"

namespace parrot {

    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        virtual void onAttach     () override;
        virtual void onDetach     () override;
        virtual void onEvent      (Event& event) override;
        virtual void onImGuiRender() override;
        void begin();
        void end  ();

        void blockEvents(bool block_events) { m_block_events = block_events; }
    private:
        bool m_block_events = true;
    };
   
}