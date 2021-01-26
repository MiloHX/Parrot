#include <Parrot.h>

class ExmapleLayer : public parrot::Layer {
public:
    ExmapleLayer() : Layer("Example") {}

    void onUpdate() override {
        if (parrot::Input::isKeyPressed(PR_KEY_A)) {
            PR_INFO("A is pressed (Poll)");
        }
    }

    void onEvent(parrot::Event& event) override {
        if (event.getEventType() == parrot::EventType::Key_Pressed) {
            parrot::KeyPressedEvent& e = static_cast<parrot::KeyPressedEvent&>(event);
            if (e.getKeyCode() == PR_KEY_F) {
                PR_INFO("F is pressed");
            }
            PR_INFO("{0}", (char)e.getKeyCode());
        }
    }
};

class Sandbox : public parrot::Application {
public:
    Sandbox() {
        pushLayer(new ExmapleLayer());
    }

    ~Sandbox() {
    }
};

// Return Application to be run on Parrot Engine
parrot::Application* parrot::createApplication() {
    return new Sandbox();
}