#pragma once

#include "Core.h"
#include "Layer.h"

namespace parrot {

    class PARROT_API LayerStack {
    public:
        LayerStack();
        ~LayerStack();

        void pushLayer  (Layer* layer  );
        void pushOverlay(Layer* overlay);
        void popLayer   (Layer* layer  );
        void popOverlay (Layer* overlay);

        std::vector<Layer*>::iterator begin() { return m_layer_list.begin(); }
        std::vector<Layer*>::iterator end  () { return m_layer_list.end  (); }
    private:
        std::vector<Layer*>           m_layer_list;
        unsigned int                  m_layer_insert_index = 0;
    };

}