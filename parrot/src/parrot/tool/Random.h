#pragma once

#include <random>

class Random {
public:
    static void init() {
        s_random_engine.seed(std::random_device()());
    }

    static float randomFloat() {
        return static_cast<float>(s_distribution(s_random_engine)) / static_cast<float>(std::numeric_limits<uint32_t>::max());
    }
private:
    static std::mt19937 s_random_engine;
    static std::uniform_int_distribution<std::mt19937::result_type> s_distribution;
};