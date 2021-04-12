#pragma once

#include <string>

namespace parrot {

    class FileDialogs {
    public:
        // Empty string will be returned if canceled
        static std::string openFile(const char* filter);
        static std::string saveFile(const char* filter);
    };
}
