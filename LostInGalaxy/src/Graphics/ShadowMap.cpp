#include "pch.h"
#include "ShadowMap.hpp"

ShadowMap::ShadowMap( GraphicsDevice* graphicsDevice, std::shared_ptr<VertexShader> vs, uint32_t width, uint32_t height ) noexcept(!_DEBUG) :
	IBind( graphicsDevice ),
	m_vShader( vs )
{
	HRESULT hr = S_OK;

	D3D11_TEXTURE2D_DESC textureDesc = {
		.Width = width,
		.Height = height,
		.MipLevels = 1u,
		.ArraySize = 1u,
		.Format = DXGI_FORMAT_R32_TYPELESS,
		.SampleDesc = {
			.Count = 1u,
			.Quality = 0u,
	},
	.Usage = D3D11_USAGE_DEFAULT,
	.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE,
	.CPUAccessFlags = 0u,
	.MiscFlags = 0u,
	};
	hr = m_device->GetDevice()->CreateTexture2D( &textureDesc, nullptr, &m_texture2D );
	assert( !FAILED( hr ) );

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {
		.Format = DXGI_FORMAT_D32_FLOAT,
		.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D,
		.Flags = 0u,
		.Texture2D = {
			.MipSlice = 0u,
		},
	};
	hr = m_device->GetDevice()->CreateDepthStencilView( m_texture2D.Get(), &dsvDesc, &m_depthStencilView );
	assert( !FAILED( hr ) );

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {
		.Format = DXGI_FORMAT_R32_FLOAT,
		.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D,
		.Texture2D = {
			.MostDetailedMip = 0u,
			.MipLevels = 1u,
		},
	};
	hr = m_device->GetDevice()->CreateShaderResourceView( m_texture2D.Get(), &srvDesc, &m_shaderResourceView );
	assert( !FAILED( hr ) );
}

ID3D11DepthStencilView* ShadowMap::GetDSV() const noexcept(!_DEBUG)
{
	return m_depthStencilView.Get();
}

ID3D11ShaderResourceView* ShadowMap::GetSRV() const noexcept(!_DEBUG)
{
	return m_shaderResourceView.Get();
}

void ShadowMap::Bind() const noexcept(!_DEBUG)
{
	m_vShader->Bind();
}
