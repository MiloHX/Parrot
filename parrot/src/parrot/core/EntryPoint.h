#pragma once

#ifdef PR_PLATFORM_WINDOWS

extern parrot::Application* parrot::createApplication();

int main(int argc, char** argv) {
    // TODO: To be moved to scheduler
    parrot::Log::init();

    PR_CORE_INFO("ENGINE START!");
    PR_WARN("CLIENT LOGGING START!");
    auto app = parrot::createApplication();
    app->run();
    delete app;
}

#endif
