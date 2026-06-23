#include "Sphere.hpp"

#include <cmath>

namespace rt
{
    Sphere::Sphere(const glm::vec3 &center, float radius)
        : m_Center(center), m_Radius(radius)
    {
    }

    bool Sphere::Hit(const Ray &ray, float rayTmin, float rayTmax, HitDesc &desc) const
    {
        glm::vec3 oc = m_Center - ray.origin();
        float a = glm::dot(ray.direction(), ray.direction());
        float h = glm::dot(ray.direction(), oc);
        float c = glm::dot(oc, oc) - m_Radius * m_Radius;

        float discriminant = h * h - a * c;

        if (discriminant < 0.0f)
            return false;

        float sqrtd = std::sqrt(discriminant);

        float root = (h - sqrtd) / a;
        if (root <= rayTmin || root >= rayTmax)
        {
            root = (h + sqrtd) / a;
            if (root <= rayTmin || root >= rayTmax)
                return false;
        }

        desc.t = root;
        desc.point = ray.at(desc.t);

        glm::vec3 outwardNormal = (desc.point - m_Center) / m_Radius;
        desc.SetFaceNormal(ray, outwardNormal);

        return true;
    }

} // namespace rt
