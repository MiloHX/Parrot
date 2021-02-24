#pragma once
#include "parrot/core/Core.h"

namespace parrot {

    struct FrameBufferProps {
        uint32_t width  = 0;
        uint32_t height = 0;
        uint32_t samples = 1;
        bool     swap_chain_target = false;
    };

    class FrameBuffer {
    public:
        virtual void bind  () = 0;
        virtual void unbind() = 0;

        virtual uint32_t getColorAttachmentRendererID() const = 0;

        virtual const FrameBufferProps& getFrameBufferProps() const = 0;

        static Ref<FrameBuffer> create(const FrameBufferProps& frame_buffer_props);
    };

}