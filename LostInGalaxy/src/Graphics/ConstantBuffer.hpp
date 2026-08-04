#pragma once

#include "pch.h"
#include "Graphics/IBind.hpp"

template<typename T>
class ConstantBuffer final : public IBind {
public:
	ConstantBuffer( GraphicsDevice* graphicsDevice, uint32_t slot ) :
		IBind( graphicsDevice ),
		m_slot( slot )
	{
		constexpr D3D11_BUFFER_DESC bufferDesc = {
			.ByteWidth = static_cast<UINT>(sizeof( T )), // Potential Bug!
			.Usage = D3D11_USAGE_DYNAMIC,
			.BindFlags = D3D11_BIND_CONSTANT_BUFFER,
			.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
			.MiscFlags = 0u,
			.StructureByteStride = static_cast<UINT>(sizeof( T )),
		};

		const HRESULT hr = m_device->GetDevice()->CreateBuffer( &bufferDesc, nullptr, &m_buffer );

		assert( !FAILED( hr ) );
	}
	~ConstantBuffer() noexcept = default;
	ConstantBuffer( const ConstantBuffer& ) = delete;
	ConstantBuffer& operator=( const ConstantBuffer& ) = delete;
	ConstantBuffer( ConstantBuffer&& ) noexcept = default;
	ConstantBuffer& operator=( ConstantBuffer&& ) noexcept = default;

	void Update( T data ) noexcept
	{
		D3D11_MAPPED_SUBRESOURCE mappedSubRes = {};
		m_device->GetContext()->Map( m_buffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedSubRes );
		std::memcpy( mappedSubRes.pData, &data, sizeof( T ) );
		m_device->GetContext()->Unmap( m_buffer.Get(), 0u );
	}
	void Bind() const noexcept override
	{
		m_device->GetContext()->VSSetConstantBuffers( m_slot, 1u, m_buffer.GetAddressOf() );
	}

	uint32_t GetSlot() const noexcept
	{
		return m_slot;
	}

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	const uint32_t m_slot;

};
