#pragma once

#include "Hittable.hpp"

#include <vector>
#include <memory>

namespace rt
{
    class HittableList : public Hittable
    {
    public:
        HittableList() = default;
        HittableList(std::shared_ptr<Hittable> obj);

        virtual bool Hit(const Ray &ray, Interval rayT, HitDesc &desc) const override;

        void Add(std::shared_ptr<Hittable> obj);

        void Clear();

    private:
        std::vector<std::shared_ptr<Hittable>> m_Objects;
    };

} // namespace rt
