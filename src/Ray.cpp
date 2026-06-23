#include "Ray.hpp"

namespace rt
{
    Ray::Ray(const glm::vec3 &origin, const glm::vec3 &direction)
        : m_Origin(origin), m_Direction(direction)
    {
    }

    glm::vec3 Ray::at(float distance) const
    {
        return m_Origin + m_Direction * distance;
    }

} // namespace rt
