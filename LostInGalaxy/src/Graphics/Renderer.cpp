#include "pch.h"
#include "Renderer.hpp"
#include "Platform/Window.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Camera.hpp"
#include "Scene/Object.hpp"

Renderer::Renderer( Window* window ) :
	m_device( std::make_unique<GraphicsDevice>( window->GetHandle(), window->GetWidth(), window->GetHeight() ) ),
	m_worldCBuffer( nullptr ),
	m_viewprojCBuffer( nullptr )
{
	HRESULT hr = S_OK;

	// Render Target
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	hr = m_device->GetSwapChain()->GetBuffer( 0u, __uuidof(ID3D11Texture2D), &backBuffer );
	assert( !FAILED( hr ) );
	hr = m_device->GetDevice()->CreateRenderTargetView( backBuffer.Get(), nullptr, &m_renderTargetView );
	assert( !FAILED( hr ) );

	// Depth Stencil
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthBuffer;
	const D3D11_TEXTURE2D_DESC depthBufferDesc = {
		.Width = window->GetWidth(),
		.Height = window->GetHeight(),
		.MipLevels = 1u,
		.ArraySize = 1u,
		.Format = DXGI_FORMAT_D24_UNORM_S8_UINT,
		.SampleDesc = {.Count = 1u, .Quality = 0u },
		.Usage = D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_DEPTH_STENCIL,
		.CPUAccessFlags = 0u,
		.MiscFlags = 0u,
	};
	hr = m_device->GetDevice()->CreateTexture2D( &depthBufferDesc, nullptr, &depthBuffer );
	assert( !FAILED( hr ) );

	constexpr D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {
		.Format = DXGI_FORMAT_D24_UNORM_S8_UINT,
		.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D,
	};
	hr = m_device->GetDevice()->CreateDepthStencilView( depthBuffer.Get(), &depthStencilViewDesc, &m_depthStencilView );
	assert( !FAILED( hr ) );

	constexpr D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {
		.DepthEnable = TRUE,
		.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
		.DepthFunc = D3D11_COMPARISON_LESS,
	};
	hr = m_device->GetDevice()->CreateDepthStencilState( &depthStencilDesc, &m_depthStencilState );
	assert( !FAILED( hr ) );

	m_worldCBuffer = std::make_unique<ConstantBuffer<DirectX::XMMATRIX>>( m_device.get(), 0u );
	m_viewprojCBuffer = std::make_unique<ConstantBuffer<DirectX::XMMATRIX>>( m_device.get(), 1u );

	const D3D11_VIEWPORT vp = {
		.Width = static_cast<FLOAT>(window->GetWidth()),
		.Height = static_cast<FLOAT>(window->GetHeight()),
		.MaxDepth = 1.0f,
	};

	m_device->GetContext()->RSSetViewports( 1u, &vp );
}

void Renderer::Render( Scene* scene ) noexcept
{
	DirectX::XMMATRIX viewproj = DirectX::XMMatrixTranspose( scene->GetActiveCamera()->GetViewProjMatrix() );
	m_viewprojCBuffer->Update( viewproj );

	for( auto* object : scene->GetObjects() )
	{
		Draw( object );
	}
}

void Renderer::BeginFrame() noexcept
{
	constexpr float clearColor[4] = { 0.07f, 0.02f, 0.12f, 1.0f };
	m_device->GetContext()->OMSetRenderTargets( 1u, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get() );
	m_device->GetContext()->ClearRenderTargetView( m_renderTargetView.Get(), clearColor );
	m_device->GetContext()->ClearDepthStencilView( m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u );
}

void Renderer::EndFrame() noexcept
{
	m_device->GetSwapChain()->Present( 1u, 0u );
}

void Renderer::Draw( Object* object ) noexcept
{
	DirectX::XMMATRIX world = DirectX::XMMatrixTranspose( object->GetWorldMatrix() );
	m_worldCBuffer->Update( world );

	object->mesh->vB.Bind();
	object->mesh->iB.Bind();
	object->material->shader.Bind();
	object->material->texture.Bind();
	object->material->sampler.Bind();
	m_worldCBuffer->Bind();
	m_viewprojCBuffer->Bind();

	m_device->GetContext()->DrawIndexed( object->mesh->iB.GetCount(), 0u, 0u );
}

GraphicsDevice* Renderer::GetGraphicsDevice() const noexcept
{
	return m_device.get();
}
