#pragma once

namespace parrot {

    class GraphicsContext {
    public:
        virtual ~GraphicsContext() = default;
        virtual void init       () = 0;
        virtual void swapBuffers() = 0;
    };

}