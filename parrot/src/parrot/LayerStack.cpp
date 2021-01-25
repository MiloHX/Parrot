#include "prpch.h"
#include "LayerStack.h"

namespace parrot {

    LayerStack::LayerStack() {
        m_layer_insert = m_layer_list.begin();
    }

    LayerStack::~LayerStack() {
        for (Layer* layer : m_layer_list) {
            delete layer;
        }
    }

    void LayerStack::pushLayer(Layer* layer) {
        m_layer_insert = m_layer_list.emplace(m_layer_insert, layer);
    }

    void LayerStack::pushOverlay(Layer* overlay) {
        m_layer_list.emplace_back(overlay);
    }

    void LayerStack::popLayer(Layer* layer) {
        auto it = std::find(m_layer_list.begin(), m_layer_list.end(), layer);
        if (it != m_layer_list.end()) {
            m_layer_list.erase(it);
            m_layer_insert--;
        }
    }

    void LayerStack::popOverlay(Layer* overlay) {
        auto it = std::find(m_layer_list.begin(), m_layer_list.end(), overlay);
        if (it != m_layer_list.end()) {
            m_layer_list.erase(it);
        }
    }

}