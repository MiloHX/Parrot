#pragma once

#include "parrot/Layer.h"
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
        virtual void onImGuiRender() override;
        void begin();
        void end  ();
    };
   
}