#pragma once

#include <random>

namespace rt
{
    inline float randomFloat(float min = 0.0f, float max = 1.0f)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());

        std::uniform_real_distribution<float> dis(min, max);
        return dis(gen);
    }

}

namespace glm
{
    inline vec3 randomVec3()
    {
        return vec3(rt::randomFloat(), rt::randomFloat(), rt::randomFloat());
    }
}
