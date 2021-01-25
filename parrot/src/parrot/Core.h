#pragma once

// DLL export

#ifdef PR_PLATFORM_WINDOWS
    #ifdef PR_BUILD_DLL
        #define PARROT_API __declspec(dllexport)
    #else
        #define PARROT_API __declspec(dllimport)
    #endif
#else
    #error Parrot only support Windows right now
#endif

// Assertion

#ifdef PR_ENABLE_ASSERTS
    #define PR_ASSERT(x, ...)     { if(!(x)) { PR_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
    #define PR_INT_ASSERT(x, ...) { if(!(x)) { PR_INT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
    #define PR_ASSERT(x, ...)     
    #define PR_INT_ASSERT(x, ...) 
#endif

// Bit Operation
#define BIT(x) (1 << x)