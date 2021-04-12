#pragma once

#include <Parrot.h>

namespace parrot {

    class HierarchyPanel {
    public:
        HierarchyPanel() = default;
        HierarchyPanel(const Ref<Scene>& scene);

        void setScene(const Ref<Scene>& scene);

        void onImGuiRender();

        Entity getSelectedEntity() const { return m_selected_entity; }
    private:
        void drawEntityNode(Entity& entity);
        void drawComponents(Entity& entity);

        Ref<Scene> m_scene;
        Entity     m_selected_entity;
    };

}