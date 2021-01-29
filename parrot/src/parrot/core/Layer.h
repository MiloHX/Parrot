#pragma once

#include "parrot/core/Core.h"
#include "parrot/event/Event.h"
#include "parrot/core/TimeStep.h"

#include <string>

namespace parrot {

    class Layer {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer();

        virtual void onAttach     () {}
        virtual void onDetach     () {}
        virtual void onUpdate     (TimeStep time_step) {}
        virtual void onImGuiRender() {}
        virtual void onEvent      (Event& event) {}

        inline const std::string& getName() const { return m_debug_name; }
    protected:
        std::string m_debug_name;
    };


}