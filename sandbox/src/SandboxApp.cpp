#include <Parrot.h>

class ExmapleLayer : public parrot::Layer {
public:
    ExmapleLayer() : Layer("Example") {}

    void onUpdate() override {
        PR_INFO("Example Updating...");
    }

    void onEvent(parrot::Event& event) override {
        PR_TRACE("{0}", event);
    }
};

class Sandbox : public parrot::Application {
public:
    Sandbox() {
        pushLayer(new ExmapleLayer());
        pushOverlay(new parrot::ImGuiLayer());
    }

    ~Sandbox() {
    }
};

// Return Application to be run on Parrot Engine
parrot::Application* parrot::createApplication() {
    return new Sandbox();
}