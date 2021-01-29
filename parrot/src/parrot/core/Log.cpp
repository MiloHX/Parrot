#include "prpch.h"
#include "parrot/core/Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace parrot {
    // Need to define them here
    std::shared_ptr<spdlog::logger> Log::s_internal_logger;
    std::shared_ptr<spdlog::logger> Log::s_application_logger;

    void Log::init() {
        spdlog::set_pattern("%^[%T.%e][%n][%-4!l]%$ %v");
        s_internal_logger    = spdlog::stdout_color_mt("PAR");
        s_application_logger = spdlog::stdout_color_mt("APP");
        s_internal_logger    ->set_level(spdlog::level::trace);
        s_application_logger ->set_level(spdlog::level::trace);
    }
}
