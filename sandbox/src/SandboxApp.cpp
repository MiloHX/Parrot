#include <Parrot.h>
#include <parrot/core/EntryPoint.h>

#include "platform/OpenGL/OpenGLShader.h"
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"

class Sandbox : public parrot::Application {
public:
    Sandbox() {
        pushLayer(new Sandbox2D());
    }

    ~Sandbox() {
    }
};

// Return Application to be run on Parrot Engine
parrot::Application* parrot::createApplication() {
    return new Sandbox();
}