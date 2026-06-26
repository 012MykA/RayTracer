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

    Metal::Metal(const glm::vec3 &albedo)
    {
    }

    bool Metal::Scatter(const Ray &rayIn, const HitDesc &desc, glm::vec3 &attenuation, Ray &scattered) const
    {
        glm::vec3 reflected = reflect(rayIn.direction(), desc.normal);
        scattered = Ray(desc.point, reflected);
        attenuation = m_Albedo;
        return true;
    }

} // namespace rt
