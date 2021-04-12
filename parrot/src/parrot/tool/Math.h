#pragma once

#include <cstdint>
#include <algorithm>
#include <glm/glm.hpp>

namespace parrot {

    static constexpr inline double PI        = 3.141592653589793238463;
    static constexpr inline float  PI_F      = 3.14159265358979f      ;
    static constexpr inline double TAU       = 6.283185307179586476925;
    static constexpr inline float  TAU_F     = 6.28318530717958f      ;
    static constexpr inline double HALF_PI   = 1.570796326794896619231;
    static constexpr inline float  HALF_PI_F = 1.57079632679489f      ;

    // For String Hash Only
    namespace {
        static constexpr inline uint32_t val_32_const   = 0x811c9dc5        ;
        static constexpr inline uint32_t prime_32_const = 0x1000193         ;
        static constexpr inline uint64_t val_64_const   = 0xcbf29ce484222325;
        static constexpr inline uint64_t prime_64_const = 0x100000001b3     ;
    }

    inline bool almostEqual(const float lhs, const float rhs, const float epsilon = 0.01f) {
        return abs(lhs - rhs) <= epsilon;
    }

    inline bool almostEqual(const double lhs, const double rhs, const double epsilon = 0.01) {
        return abs(lhs - rhs) <= epsilon;
    }

    bool decomposeGlmTransform(const glm::mat4& transform, glm::vec3& out_translation, glm::vec3& out_rotation, glm::vec3& out_scale);

    //
    // fnv1a String Hash Solution is from 
    // @__discovery https://notes.underscorediscovery.com/constexpr-fnv1a/
    //
    constexpr uint32_t stringHash(const char* const str, const uint32_t value = val_32_const) noexcept {
        return (str[0] == '\0') ? value : stringHash(&str[1], (value ^ uint32_t(str[0])) * prime_32_const);
    }

    constexpr uint64_t stringHash64(const char* const str, const uint64_t value = val_64_const) noexcept {
        return (str[0] == '\0') ? value : stringHash64(&str[1], (value ^ uint64_t(str[0])) * prime_64_const);
    }

    constexpr uint32_t operator ""_hash(const char* str, std::size_t size) noexcept {
        return stringHash(str);
    }

    constexpr uint64_t operator ""_hash64(const char* str, std::size_t size) noexcept {
        return stringHash64(str);
    }

}
