#pragma once

#include "parrot/core/Core.h"
#include "parrot/core/KeyCode.h"

namespace parrot {

    class Input {
    public:
        static bool                    isKeyPressed        (KeyCode key_code);
        static bool                    isMouseButtonPressed(MouseButton button);
        static std::pair<float, float> getMousePosition    (            );
    };

}