#include "HittableList.hpp"

namespace rt
{
    HittableList::HittableList(std::shared_ptr<Hittable> obj)
    {
        Add(obj);
    }

    bool HittableList::Hit(const Ray &ray, Interval rayT, HitDesc &desc) const
    {
        HitDesc tmpDesc;
        bool hitAnything = false;
        float closestSoFar = rayT.max;

        for (const auto &obj : m_Objects)
        {
            if (obj->Hit(ray, Interval(rayT.min, closestSoFar), tmpDesc))
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
