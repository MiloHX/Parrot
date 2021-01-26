#pragma once

#include "parrot/Core.h"

namespace parrot {

    class PARROT_API Input {
    public:
        inline static bool                    isKeyPressed        (int key_code) { return s_instance->isKeyPressedImpl        (key_code);  }
        inline static bool                    isMouseButtonPressed(int button  ) { return s_instance->isMouseButtonPressedImpl(button  );  }
        inline static std::pair<float, float> getMousePosition    (            ) { return s_instance->getMousePositionImpl    (        );  }
    protected:
        virtual bool                    isKeyPressedImpl        (int key_code) = 0;
        virtual bool                    isMouseButtonPressedImpl(int button  ) = 0;
        virtual std::pair<float, float> getMousePositionImpl    (            ) = 0;
    private:
        static Input* s_instance;
    };

}