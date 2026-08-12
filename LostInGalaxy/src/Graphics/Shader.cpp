#include "pch.h"
#include "Shader.hpp"

Shader::Shader( GraphicsDevice* graphicsDevice, const std::filesystem::path& shaderPath ) noexcept(!_DEBUG) :
	IBind( graphicsDevice )
{
	const HRESULT hr = D3DReadFileToBlob( shaderPath.c_str(), &m_blob );
	assert( !FAILED( hr ) );
}

VertexShader::VertexShader( GraphicsDevice* graphicsDevice, const std::filesystem::path& shaderPath ) noexcept(!_DEBUG) :
	Shader( graphicsDevice, shaderPath )
{
	HRESULT hr = S_OK;

	constexpr D3D11_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{ "POSITION", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u },
		{ "NORMAL", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u },
		{ "TEXCOORD", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u },
	};

	hr = m_device->GetDevice()->CreateInputLayout(
		inputElementDescs,
		static_cast<UINT>(std::size( inputElementDescs )),
		m_blob->GetBufferPointer(),
		m_blob->GetBufferSize(),
		&m_inputLayout
	);
	assert( !FAILED( hr ) );

	hr = m_device->GetDevice()->CreateVertexShader(
		m_blob->GetBufferPointer(),
		m_blob->GetBufferSize(),
		nullptr,
		&m_vertexShader
	);
	assert( !FAILED( hr ) );
}

void VertexShader::Bind() const noexcept(!_DEBUG)
{
	m_device->GetContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	m_device->GetContext()->IASetInputLayout( m_inputLayout.Get() );
	m_device->GetContext()->VSSetShader( m_vertexShader.Get(), nullptr, 0u );
}

PixelShader::PixelShader( GraphicsDevice* graphicsDevice, const std::filesystem::path& shaderPath ) noexcept(!_DEBUG) :
	Shader( graphicsDevice, shaderPath )
{
	const HRESULT hr = m_device->GetDevice()->CreatePixelShader(
	   m_blob->GetBufferPointer(),
	   m_blob->GetBufferSize(),
	   nullptr,
	   &m_pixelShader
	);
	assert( !FAILED( hr ) );
}

void PixelShader::Bind() const noexcept(!_DEBUG)
{
	m_device->GetContext()->PSSetShader( m_pixelShader.Get(), nullptr, 0u );
}
