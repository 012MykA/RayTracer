#pragma once

#include "Hittable.hpp"

namespace rt
{
    class Sphere : public Hittable
    {
    public:
        Sphere(const glm::vec3 &center, float radius);

        virtual bool Hit(const Ray &ray, Interval rayT, HitDesc &desc) const override;

    private:
        glm::vec3 m_Center;
        float m_Radius;
        std::shared_ptr<Material> m_Material;
    };

} // namespace rt
