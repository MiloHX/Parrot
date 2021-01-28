#pragma once

#include "Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace parrot {
    class Log {
    public:
        static void init();
        inline static std::shared_ptr<spdlog::logger>& getInternalLogger   () { return s_internal_logger   ; }
        inline static std::shared_ptr<spdlog::logger>& getApplicationLogger() { return s_application_logger; }
    private:
        static std::shared_ptr<spdlog::logger> s_internal_logger   ;
        static std::shared_ptr<spdlog::logger> s_application_logger;
    };
}

// Parrot internal logging
#define PR_CORE_TRACE(...) ::parrot::Log::getInternalLogger()->trace(__VA_ARGS__)
#define PR_CORE_INFO(...)  ::parrot::Log::getInternalLogger()->info(__VA_ARGS__)
#define PR_CORE_WARN(...)  ::parrot::Log::getInternalLogger()->warn(__VA_ARGS__)
#define PR_CORE_ERROR(...) ::parrot::Log::getInternalLogger()->error(__VA_ARGS__)
#define PR_CORE_FATAL(...) ::parrot::Log::getInternalLogger()->fatal(__VA_ARGS__)

// Application logging
#define PR_TRACE(...)     ::parrot::Log::getApplicationLogger()->trace(__VA_ARGS__)
#define PR_INFO(...)      ::parrot::Log::getApplicationLogger()->info(__VA_ARGS__)
#define PR_WARN(...)      ::parrot::Log::getApplicationLogger()->warn(__VA_ARGS__)
#define PR_ERROR(...)     ::parrot::Log::getApplicationLogger()->error(__VA_ARGS__)
#define PR_FATAL(...)     ::parrot::Log::getApplicationLogger()->fatal(__VA_ARGS__)

