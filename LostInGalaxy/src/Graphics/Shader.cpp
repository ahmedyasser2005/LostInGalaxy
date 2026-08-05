#include "pch.h"
#include "Shader.hpp"

Shader::Shader( GraphicsDevice* graphicsDevice, const wchar_t* vShaderFilename, const wchar_t* pShaderFilename ) :
	IBind( graphicsDevice )
{
	HRESULT hr = S_OK;

	hr = D3DReadFileToBlob( vShaderFilename, &m_vShaderBlob );
	assert( !FAILED( hr ) );
	hr = D3DReadFileToBlob( pShaderFilename, &m_pShaderBlob );
	assert( !FAILED( hr ) );

	constexpr D3D11_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{ "POSITION", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u },
		//{ "COLOR", 0u, DXGI_FORMAT_R8G8B8A8_UNORM, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u }
		{ "UV", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u }
	};

	hr = m_device->GetDevice()->CreateInputLayout(
		inputElementDescs,
		static_cast<UINT>(std::size( inputElementDescs )),
		m_vShaderBlob->GetBufferPointer(),
		m_vShaderBlob->GetBufferSize(),
		&m_inputLayout
	);
	assert( !FAILED( hr ) );

	hr = m_device->GetDevice()->CreateVertexShader(
		m_vShaderBlob->GetBufferPointer(),
		m_vShaderBlob->GetBufferSize(),
		nullptr,
		&m_vShader
	);
	assert( !FAILED( hr ) );

	hr = m_device->GetDevice()->CreatePixelShader(
		m_pShaderBlob->GetBufferPointer(),
		m_pShaderBlob->GetBufferSize(),
		nullptr,
		&m_pShader
	);
	assert( !FAILED( hr ) );
}

void Shader::Bind() const noexcept
{
	m_device->GetContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	m_device->GetContext()->IASetInputLayout( m_inputLayout.Get() );
	m_device->GetContext()->VSSetShader( m_vShader.Get(), nullptr, 0u );
	m_device->GetContext()->PSSetShader( m_pShader.Get(), nullptr, 0u );
}
