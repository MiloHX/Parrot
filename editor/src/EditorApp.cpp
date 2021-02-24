#include <Parrot.h>
#include <parrot/core/EntryPoint.h>

#include "EditorLayer.h"

namespace parrot {

    class Editor : public Application {
    public:
        Editor() : Application("Parrot Editor") {
            pushLayer(new EditorLayer());
        }

        ~Editor() {
        }
    };

    // Return Application to be run on Parrot Engine
    Application* createApplication() {
        return new Editor();
    }

}