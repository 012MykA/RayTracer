#pragma once

#include "Hittable.hpp"
#include "Material.hpp"

#include <memory>

namespace rt
{
    class Sphere : public Hittable
    {
    public:
        Sphere(const glm::vec3 &center, float radius, std::shared_ptr<Material> mat);

        virtual bool Hit(const Ray &ray, Interval rayT, HitDesc &desc) const override;

    private:
        glm::vec3 m_Center;
        float m_Radius;
        std::shared_ptr<Material> m_Material;
    };

} // namespace rt
