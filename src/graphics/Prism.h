#pragma once
#include "GraphicsUtils.hpp"
#include "IndexedTriangleList.h"
#include <DirectXMath.h>
#include <DirectXMathVector.inl>
#include <vector>

class Prism{
public:
    template<class T>
    static IndexedTriangleList<T>MakeTessalated(size_t longDivs) {
        namespace dx = DirectX;
        assert(longDivs > 3);

        const auto base {dx::XMVectorSet(1.f, 0.f, -1.f, 0.f)};
        const auto offset {dx::XMVectorSet(0.f, 0.f, 2.f, 0.f)};
        const float longAngle {static_cast<float>(2.f * pi_d / longDivs)};

        std::vector<T> vertices {};

        vertices.emplace_back();
        vertices.back().pos = {0.f, 0.f, -1.f};
        const auto iCenterNear {vertices.size() - 1};

        vertices.emplace_back();
        vertices.back().pos = {0.f, 0.f, 1.f};
        const auto iCenterFar {vertices.size() - 1};

        for (size_t iLong {}; iLong < longDivs; ++iLong) {
            const auto near { dx::XMVector3Transform(base, dx::XMMatrixRotationZ(longAngle * iLong))};
            const auto far {dx::XMVectorAdd(near, offset)};

            vertices.emplace_back();
            dx::XMStoreFloat3(&vertices.back().pos, near);

            vertices.emplace_back();
            dx::XMStoreFloat3(&vertices.back().pos, far);
        }

        std::vector<T> indices {};
        //side indices
        const auto modDivs {longDivs * 2};
        //                    skip  center vertices
        for (size_t iLong {}, i {2}; iLong < longDivs; iLong++, i += 2) {
            indices.push_back(i);
            indices.push_back((i) % modDivs + 2);
            indices.push_back(i + 1);
            indices.push_back((i) % modDivs + 2);
            indices.push_back((i + 1) % modDivs + 2);
            indices.push_back(i + 1);
        }

        for (size_t iLong {}, i {2}; iLong < longDivs; iLong++, i += 2) {
            indices.push_back(i);
            indices.push_back(iCenterNear);
            indices.push_back((i) % modDivs + 2);

            indices.push_back(iCenterFar);
            indices.push_back(i + 1);
            indices.push_back((i + 1) % modDivs + 2);
        }

        return {std::move(vertices), std::move(indices)};
    }

    template<class T>
    static IndexedTriangleList<T> Make() {
        return MakeTessalated<T>(24);
    }
};
