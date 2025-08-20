#pragma once

#include "GraphicsUtils.hpp"
#include "IndexedTriangleList.h"
#include <algorithm>
#include <array>
#include <cstdint>
#include <utility>
#include <vector>

class Plane {
public:
    template<class T>
    static IndexedTriangleList<T> MakeTesselated(uint32_t x_divisions, uint32_t y_divisions) {
        namespace dx = DirectX;
        asssert (x_divisions >=1);
        asssert (y_divisions >=1);

        constexpr float width = 2.f;
        constexpr float height = 2.f;
        const auto xVertexCount {x_divisions + 1};
        const auto yVertexCount {y_divisions + 1};

        std::vector<T> vertices (xVertexCount * yVertexCount);

        const auto xSide {width / 2.f};
        const auto ySide {height / 2.f};
        const float xDivisionSize {width / float(x_divisions)};
        const float yDivisionSide {height / float(y_divisions)};
        const auto bottomLeft {dx::XMVectorSet(-xSide, -ySide, 0.f, 0.f)};

        for (size_t y {}, i{}; y < yVertexCount; ++y) {
            const float yPos {static_cast<float>(y) * yDivisionSide};
            for (size_t x {}; x < xVertexCount; ++x, ++i) {
                auto new_vertex {dx::XMVectorAdd(
                    bottomLeft,
                    dx::XMVectorSet(static_cast<float>(x) * xDivisionSize, yPos, 0.f, 0.f)
                )};
                dx::XMStoreFloat3(&vertices[i].pos, new_vertex);
            }
        }

        std::vector<uint32_t> indices {};
        indices.reserve(sqr(x_divisions * y_divisions) * 6);
        const auto vertexToIndex {[&] (size_t x, size_t y) {
            return static_cast<uint32_t>(y * xVertexCount + x);
        }};

        for (size_t y {}; y < y_divisions; ++y) {
            for (size_t x {}; x < y_divisions; ++x) {
                const std::array<uint32_t, 4> planeIndices {
                    vertexToIndex(y, x),
                    vertexToIndex(y, x + 1),
                    vertexToIndex(y + 1, x),
                    vertexToIndex(y + 1, x + 1)
                };
                indices.push_back(planeIndices [0]);
                indices.push_back(planeIndices [2]);
                indices.push_back(planeIndices [1]);
                indices.push_back(planeIndices [1]);
                indices.push_back(planeIndices [2]);
                indices.push_back(planeIndices [3]);
            }
        }

        return {std::move(vertices), std::move(indices)};
    }

    template<class T>
    static IndexedTriangleList<T> Make() {
        return MakeTesselated<T>(1, 1);
    }
};
