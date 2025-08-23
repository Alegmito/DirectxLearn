#pragma once

#include "GraphicsUtils.hpp"
#include "IndexedTriangleList.h"
#include <DirectXMath.h>
#include <cassert>
#include <vector>
class Cone{

public:
    template<class T>
    static IndexedTriangleList<T> MakeTesselated(size_t longDivs) {
        namespace dx = DirectX;
        assert(longDivs > 3);

        const auto base {dx::XMVectorSet(1.f, 0.f, -1.f, 0.f)};
        const auto longAngle {2.f * pi_d / longDivs};

        std::vector<T> vertices {};
        for (size_t iLong {}; iLong < longDivs; ++iLong) {
            vertices.emplace_back();
            auto v {dx::XMVector3Transform(
                base,
                dx::XMMatrixRotationZ(longAngle * iLong)
            )};
            dx::XMStoreFloat3(&vertices.back().pos, v);
        }

        vertices.emplace_back();
        vertices.back().pos = {0.f, 0.f, -1.f};
        const auto iCenter {vertices.size() - 1};

        vertices.emplace_back();
        vertices.back().pos = {0.f, 0.f, 1.f};
        const auto iTip {vertices.size() - 1};

        std::vector<uint32_t> indices {};
        for (size_t iLong {}; iLong < longDivs; ++iLong) {
            indices.push_back(iCenter);
            indices.push_back((iLong + 1) % longDivs);
            indices.push_back(iLong);
        }

        for (size_t iLong {}; iLong < longDivs; ++iLong) {
            indices.push_back(iLong);
            indices.push_back((iLong + 1) % longDivs);
            indices.push_back(iTip);
        }

        return {std::move(vertices), std::move(indices)};
    }

    template<class T>
    static IndexedTriangleList<T> Make() {
        return MakeTesselated<T>(24);
    }
};
