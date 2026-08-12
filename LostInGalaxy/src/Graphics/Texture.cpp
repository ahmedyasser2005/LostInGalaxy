#include "pch.h"
#include "Texture.hpp"

Texture::Texture( GraphicsDevice* graphicsDevice, Image&& image, uint16_t slot ) :
	IBind( graphicsDevice ),
	m_slot( slot )
{
	HRESULT hr = S_OK;

	const D3D11_TEXTURE2D_DESC textureDesc = {
		.Width = static_cast<UINT>(image.texMetadata.width),
		.Height = static_cast<UINT>(image.texMetadata.height),
		.MipLevels = static_cast<UINT>(image.texMetadata.mipLevels),
		.ArraySize = static_cast<UINT>(image.texMetadata.arraySize),
		.Format = image.texMetadata.format,
		.SampleDesc = {.Count = 1u, .Quality = 0u,},
		.Usage = D3D11_USAGE_IMMUTABLE,
		.BindFlags = D3D11_BIND_SHADER_RESOURCE,
		.CPUAccessFlags = 0u,
		.MiscFlags = 0u,
	};

	const D3D11_SUBRESOURCE_DATA subResData = {
		.pSysMem = image.scratchImage.GetPixels(),
		.SysMemPitch = static_cast<UINT>(image.scratchImage.GetPixelsSize() / image.texMetadata.width),
	};
	hr = m_device->GetDevice()->CreateTexture2D( &textureDesc, &subResData, &m_textureBuffer );
	assert( !FAILED( hr ) );

	const D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {
	   .Format = textureDesc.Format,
	   .ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D,
	   .Texture2D = {.MostDetailedMip = 0u, .MipLevels = 1u, },
	};

	hr = m_device->GetDevice()->CreateShaderResourceView( m_textureBuffer.Get(), &shaderResourceViewDesc, &m_shaderResourceView );
	assert( !FAILED( hr ) );
}

void Texture::Bind() const noexcept(!_DEBUG)
{
	m_device->GetContext()->PSSetShaderResources( m_slot, 1u, m_shaderResourceView.GetAddressOf() );
}

uint16_t Texture::GetSlot() const noexcept(!_DEBUG)
{
	return m_slot;
}
