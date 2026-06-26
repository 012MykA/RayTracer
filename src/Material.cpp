#include "Material.hpp"
#include "Random.hpp"
#include "VecUtils.hpp"

namespace rt
{
    Lambertian::Lambertian(const glm::vec3 &albedo)
        : m_Albedo(albedo)
    {
    }

    bool Lambertian::Scatter(const Ray &rayIn, const HitDesc &desc, glm::vec3 &attenuation, Ray &scattered) const
    {
        glm::vec3 scatterDirection = desc.normal + randomUnitVector();
        if (nearZero(scatterDirection))
            scatterDirection = desc.normal;

        scattered = Ray(desc.point, scatterDirection);
        attenuation = m_Albedo;
        return true;
    }

    Metal::Metal(const glm::vec3 &albedo, float fuzz)
        : m_Albedo(albedo), m_Fuzz(fuzz)
    {
    }

    bool Metal::Scatter(const Ray &rayIn, const HitDesc &desc, glm::vec3 &attenuation, Ray &scattered) const
    {
        glm::vec3 reflected = reflect(rayIn.direction(), desc.normal);
        reflected = glm::normalize(reflected) + (m_Fuzz * randomUnitVector());
        scattered = Ray(desc.point, reflected);
        attenuation = m_Albedo;
        return glm::dot(scattered.direction(), desc.normal) > 0.0f;
    }

} // namespace rt
