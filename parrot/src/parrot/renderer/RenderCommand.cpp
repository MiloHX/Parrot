#include "prpch.h"
#include "parrot/renderer/RenderCommand.h"
#include "platform/OpenGL/OpenGLRendererAPI.h"

namespace parrot {

    RendererAPI* RenderCommand::s_renderer_API = new OpenGLRendererAPI();

}