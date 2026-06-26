#pragma once

#include "Ray.hpp"
#include "Interval.hpp"

#include <glm/glm.hpp>

#include <memory>

namespace rt
{
    class Material;

    struct HitDesc
    {
        glm::vec3 point;
        glm::vec3 normal;
        std::shared_ptr<Material> material;
        float t;
        bool isFrontFace;

        void SetFaceNormal(const Ray &ray, const glm::vec3 &outwardNormal)
        {
            isFrontFace = glm::dot(ray.direction(), outwardNormal) < 0;
            normal = isFrontFace ? outwardNormal : -outwardNormal;
        }
    };

    class Hittable
    {
    public:
        virtual ~Hittable() = default;

        virtual bool Hit(const Ray &ray, Interval rayT, HitDesc &desc) const = 0;
    };

} // namespace rt
