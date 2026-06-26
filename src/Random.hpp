#pragma once

#include <cmath>
#include <random>

#include <glm/glm.hpp>

namespace rt
{
    inline float randomFloat(float min = 0.0f, float max = 1.0f)
    {
        thread_local std::random_device rd;
        thread_local std::mt19937 gen(rd());

        std::uniform_real_distribution<float> dis(min, max);
        return dis(gen);
    }

    inline glm::vec3 randomVec3(float min = 0.0f, float max = 1.0f)
    {
        return glm::vec3(rt::randomFloat(min, max), rt::randomFloat(min, max), rt::randomFloat(min, max));
    }

    inline glm::vec3 randomUnitVector()
    {
        while (true)
        {
            glm::vec3 p = randomVec3(-1.0f, 1.0f);
            float lensq = glm::dot(p, p);
            if (1e-160f < lensq && lensq <= 1.0f)
            {
                return p / std::sqrt(lensq);
            }
        }
    }

    inline glm::vec3 randomOnHemisphere(const glm::vec3 &normal)
    {
        glm::vec3 onUnitSphere = randomUnitVector();
        if (glm::dot(onUnitSphere, normal) > 0.0f)
            return onUnitSphere;
        else
            return -onUnitSphere;
    }

    inline glm::vec3 reflect(const glm::vec3 &v, const glm::vec3 &n)
    {
        return v - 2 * dot(v, n) * n;
    }

}
