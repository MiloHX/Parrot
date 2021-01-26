#pragma once

#include "parrot/Layer.h"

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
        float m_time = 0.0f;

    };
   
}