#pragma once

#include <glm/glm.hpp>

namespace rt
{
    class Ray
    {
    public:
        Ray(const glm::vec3 &origin, const glm::vec3 &direction);

        const glm::vec3 &origin() const { return m_Origin; }
        const glm::vec3 &direction() const { return m_Direction; }

        // Returns a point at a distance from the origin
        glm::vec3 at(float distance) const;

    private:
        glm::vec3 m_Origin;
        glm::vec3 m_Direction;
    };

} // namespace rt
