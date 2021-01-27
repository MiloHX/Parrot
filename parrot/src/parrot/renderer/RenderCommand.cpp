#include "prpch.h"
#include "RenderCommand.h"

#include "platform/OpenGL/OpenGLRendererAPI.h"

namespace parrot {

    RendererAPI* RenderCommand::s_renderer_API = new OpenGLRendererAPI();

}