#include "pch.h"
#include "IndexBuffer.hpp"

IndexBuffer::IndexBuffer( GraphicsDevice* graphicsDevice, std::span<uint32_t> indices ) :
	IBind( graphicsDevice ),
	m_count( (uint32_t)indices.size() )
{
	const D3D11_BUFFER_DESC bufferDesc = {
		.ByteWidth = static_cast<UINT>(sizeof( uint32_t ) * indices.size()),
		.Usage = D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_INDEX_BUFFER,
		.CPUAccessFlags = 0u,
		.MiscFlags = 0u,
		.StructureByteStride = sizeof( uint32_t ),
	};
	const D3D11_SUBRESOURCE_DATA subData = { .pSysMem = indices.data() };
	const HRESULT hr = m_device->GetDevice()->CreateBuffer( &bufferDesc, &subData, &m_buffer );

	assert( !FAILED( hr ) );
}

void IndexBuffer::Bind() const noexcept(!_DEBUG)
{
	m_device->GetContext()->IASetIndexBuffer( m_buffer.Get(), DXGI_FORMAT_R32_UINT, 0u );
}

uint32_t IndexBuffer::GetCount() const noexcept(!_DEBUG)
{
	return m_count;
}
