#pragma once
#include <DirectxMath.h>
#include <vector>

template<class T>
class IndexedTriangleList {
public:
    IndexedTriangleList() = default;
	IndexedTriangleList( std::vector<T> verts,std::vector<uint32_t> indcs )
		:
		vertices_( std::move( verts ) ),
		indices_( std::move( indcs ) )
	{
		assert( vertices_.size() > 2 );
		assert( indices_.size() % 3 == 0 );
	}
	void Transform( DirectX::FXMMATRIX matrix )
	{
		for( auto& v : vertices_ )
		{
			const DirectX::XMVECTOR pos = DirectX::XMLoadFloat3( &v.pos );
			DirectX::XMStoreFloat3(
				&v.pos,
				DirectX::XMVector3Transform( pos,matrix )
			);
		}
	}

public:
	std::vector<T> vertices_;
	std::vector<uint32_t> indices_;
};
