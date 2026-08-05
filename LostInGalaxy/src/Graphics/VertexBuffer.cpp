#include "pch.h"
#include "VertexBuffer.hpp"
#include "Graphics/Mesh.hpp"

VertexBuffer::VertexBuffer( GraphicsDevice* graphicsDevice, std::span<Vertex> vertices ) :
	IBind( graphicsDevice )
{
	const D3D11_BUFFER_DESC bufferDesc = {
		.ByteWidth = static_cast<UINT>(sizeof( Vertex ) * vertices.size()),
		.Usage = D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_VERTEX_BUFFER,
		.CPUAccessFlags = 0u,
		.MiscFlags = 0u,
		.StructureByteStride = static_cast<UINT>(sizeof( Vertex )),
	};
	const D3D11_SUBRESOURCE_DATA subData = { .pSysMem = vertices.data() };
	HRESULT hr = m_device->GetDevice()->CreateBuffer( &bufferDesc, &subData, &m_buffer );

	assert( !FAILED( hr ) );
}

void VertexBuffer::Bind() const noexcept
{
	constexpr UINT stride = static_cast<UINT>(sizeof( Vertex ));
	constexpr UINT offset = 0u;

	m_device->GetContext()->IASetVertexBuffers( 0u, 1u, m_buffer.GetAddressOf(), &stride, &offset );
}
