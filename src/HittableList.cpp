#include "HittableList.hpp"

namespace rt
{
    HittableList::HittableList(std::shared_ptr<Hittable> obj)
    {
        Add(obj);
    }

    bool HittableList::Hit(const Ray &ray, float rayTmin, float rayTmax, HitDesc &desc) const
    {
        HitDesc tmpDesc;
        bool hitAnything = false;
        float closestSoFar = rayTmax;

        for (const auto &obj : m_Objects)
        {
            if (obj->Hit(ray, rayTmin, closestSoFar, tmpDesc))
            {
                hitAnything = true;
                closestSoFar = tmpDesc.t;
                desc = tmpDesc;
            }
        }

        return hitAnything;
    }

    void HittableList::Add(std::shared_ptr<Hittable> obj)
    {
        m_Objects.push_back(obj);
    }

    void HittableList::Clear()
    {
        m_Objects.clear();
    }

} // namespace rt
