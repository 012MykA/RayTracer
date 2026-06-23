#pragma once

#include "Ray.hpp"

#include <glm/glm.hpp>

namespace rt
{
    struct HitDesc
    {
        glm::vec3 point;
        glm::vec3 normal;
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

        virtual bool Hit(const Ray &ray, float rayTmin, float rayTmax, HitDesc &desc) const = 0;
    };

} // namespace rt
