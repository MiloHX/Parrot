#pragma once

#include "Core.h"
#include "event/Event.h"

#include <string>

namespace parrot {

    class Layer {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer();

        virtual void onAttach     () {}
        virtual void onDetach     () {}
        virtual void onUpdate     () {}
        virtual void onImGuiRender() {}
        virtual void onEvent      (Event& event) {}

        inline const std::string& getName() const { return m_debug_name; }
    protected:
        std::string m_debug_name;
    };


}