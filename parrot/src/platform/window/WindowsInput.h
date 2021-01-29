#pragma once

#include "parrot/core/Input.h"

namespace parrot {

    class WindowsInput : public Input {
    protected:
        virtual bool                    isKeyPressedImpl        (int key_code) override;
        virtual bool                    isMouseButtonPressedImpl(int button  ) override;
        virtual std::pair<float, float> getMousePositionImpl    (            ) override;
    };
}