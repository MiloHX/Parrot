#include "prpch.h"
#include "parrot/core/LayerStack.h"

namespace parrot {

    LayerStack::LayerStack() {
    }

    LayerStack::~LayerStack() {
        for (Layer* layer : m_layer_list) {
            delete layer;
        }
    }

    void LayerStack::pushLayer(Layer* layer) {
        m_layer_list.emplace(m_layer_list.begin() + m_layer_insert_index, layer);
        ++m_layer_insert_index;
    }

    void LayerStack::pushOverlay(Layer* overlay) {
        m_layer_list.emplace_back(overlay);
    }

    void LayerStack::popLayer(Layer* layer) {
        auto it = std::find(m_layer_list.begin(), m_layer_list.end(), layer);
        if (it != m_layer_list.end()) {
            m_layer_list.erase(it);
            --m_layer_insert_index;
        }
    }

    void LayerStack::popOverlay(Layer* overlay) {
        auto it = std::find(m_layer_list.begin(), m_layer_list.end(), overlay);
        if (it != m_layer_list.end()) {
            m_layer_list.erase(it);
        }
    }

}