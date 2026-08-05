#include "pch.h"
#include "Sampler.hpp"

Sampler::Sampler( GraphicsDevice* graphicsDevice, uint16_t slot ) :
	IBind( graphicsDevice ),
	m_slot( slot )
{
	constexpr D3D11_SAMPLER_DESC samplerDesc = {
		.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT,
		.AddressU = D3D11_TEXTURE_ADDRESS_BORDER,
		.AddressV = D3D11_TEXTURE_ADDRESS_BORDER,
		.AddressW = D3D11_TEXTURE_ADDRESS_BORDER,
		.MaxAnisotropy = 16u,
		.BorderColor = { 0.5f, 0.0f, 0.0f, 1.0f },
	};

	const HRESULT  hr = m_device->GetDevice()->CreateSamplerState( &samplerDesc, &m_samplerState );
	assert( !FAILED( hr ) );
}

void Sampler::Bind() const noexcept
{
	m_device->GetContext()->PSSetSamplers( m_slot, 1u, m_samplerState.GetAddressOf() );
}

uint16_t Sampler::GetSlot() const noexcept
{
	return m_slot;
}
