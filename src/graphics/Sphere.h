#pragma once

#include "IndexedTriangleList.h"
#include <DirectxMath.h>
#include <cassert>
#include <cstdint>

namespace dx = DirectX;
class Sphere {
    static constexpr float radius_base = 1.f;
public:
    template<class T>
    static IndexedTriangleList<T> MakeTesselated(
        uint32_t latDivisions,
        uint32_t longDivisions
    ) {
        assert(latDivisions >= 3);
        assert(longDivisions >= 3);

        const auto base {dx::XMVectorSet( 0.0f,0.0f,radius_base,0.0f )};
		const float lattitudeAngle = dx::XM_PI / latDivisions;
		const float longitudeAngle = 2.0f * dx::XM_PI / longDivisions;

		std::vector<T> vertices;
		for( size_t iLat = 1; iLat < latDivisions; iLat++ )
		{
			const auto latBase = dx::XMVector3Transform( 
				base,
				dx::XMMatrixRotationX( lattitudeAngle * iLat ) 
			);
			for( size_t iLong = 0; iLong < longDivisions; iLong++ )
			{
				vertices.emplace_back();
				auto v = dx::XMVector3Transform( 
					latBase,
					dx::XMMatrixRotationZ( longitudeAngle * iLong )
				);
				dx::XMStoreFloat3( &vertices.back().pos,v );
			}
		}

        const auto northPoleIndex {vertices.size()};
        vertices.emplace_back();
        dx::XMStoreFloat3(&vertices.back().pos, base);
        const auto southPoleIndex {vertices.size()};
        vertices.emplace_back();
        dx::XMStoreFloat3(&vertices.back().pos, dx::XMVectorNegate(base));

        const auto calcIdx = [&]( unsigned short iLat,unsigned short iLong ) { 
            return iLat * longDivisions + iLong; 
        };

        std::vector<uint32_t> indices {};
        for( unsigned short iLat = 0; iLat < latDivisions - 2; iLat++ ) {
			for( size_t iLong {}, iLongNext {1}; iLong < longDivisions; iLong++, iLongNext++) {
                // Wrap when iLongNext == longDivisions
                iLongNext %= longDivisions;

				indices.push_back( calcIdx( iLat,iLong) );
				indices.push_back( calcIdx( iLat + 1,iLong) );
				indices.push_back( calcIdx( iLat,iLongNext) );
				indices.push_back( calcIdx( iLat,iLongNext) );
				indices.push_back( calcIdx( iLat + 1,iLong) );
				indices.push_back( calcIdx( iLat + 1,iLongNext) );
			}
		}

		// cap fans
        // wrap when iLongNext == longDivisions
        for( size_t iLong {}, iLongNext {1}; iLong < longDivisions; iLong++, iLongNext++) {
            iLongNext %= longDivisions;
			// north
			indices.push_back( northPoleIndex );
			indices.push_back( calcIdx( 0,iLong ) );
			indices.push_back( calcIdx( 0,iLongNext ) );
			// south
			indices.push_back( calcIdx( latDivisions - 2,iLongNext ) );
			indices.push_back( calcIdx( latDivisions - 2,iLong ) );
			indices.push_back( southPoleIndex );
		}

		return { std::move( vertices ),std::move( indices ) };
    }
    
    template<class T>
    static IndexedTriangleList<T> Make() {
        return MakeTesselated<T>(12, 24);
    }
};
