#pragma once

#include <glm/glm.hpp>

namespace rt
{
    inline bool nearZero(const glm::vec3 &v)
    {
        return (std::fabs(v.x) < 1e-8) && (std::fabs(v.y) < 1e-8) && (std::fabs(v.z) < 1e-8);
    }

} // namespace rt
