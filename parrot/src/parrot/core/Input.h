#pragma once

#include "parrot/core/Core.h"

namespace parrot {

    class Input {
    public:
        static bool                    isKeyPressed        (int key_code);
        static bool                    isMouseButtonPressed(int button  );
        static std::pair<float, float> getMousePosition    (            );
    };

}