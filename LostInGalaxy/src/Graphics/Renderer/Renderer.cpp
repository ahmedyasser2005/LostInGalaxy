#include "pch.h"
#include "Renderer.hpp"
#include "Platform/Window.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Camera.hpp"
#include "Scene/Object.hpp"
#include "Scene/Light.hpp"

Renderer::Renderer( Window* window ) :
	m_device( std::make_unique<GraphicsDevice>( window->GetHandle(), window->GetWidth(), window->GetHeight() ) ),
	m_cameraCB( nullptr ),
	m_objectMatCB( nullptr ),
	m_lightPosCB( nullptr ),
	m_lightCB( nullptr ),
	m_materialCB( nullptr ),
	m_shadowMappingVS( std::make_shared<VertexShader>( m_device.get(), "Shaders/ShadowMappingVS.cso" ) ),
	m_shadowMap( std::make_unique<ShadowMap>( m_device.get(), m_shadowMappingVS, window->GetWidth(), window->GetHeight() ) ),
	m_currentMaterial( nullptr ),
	m_width( static_cast<float>(window->GetWidth()) ),
	m_height( static_cast<float>(window->GetHeight()) )
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

	m_cameraCB = std::make_unique<ConstantBuffer<CameraCB>>( m_device.get(), 0u );
	m_objectMatCB = std::make_unique<ConstantBuffer<ObjectMatCB>>( m_device.get(), 1u );
	m_lightPosCB = std::make_unique<ConstantBuffer<std::array<LightPosCB, 4>>>( m_device.get(), 2u );
	m_lightCB = std::make_unique<ConstantBuffer<std::array<LightCB, 4>>>( m_device.get(), 3u );
	m_materialCB = std::make_unique<ConstantBuffer<MaterialCB>>( m_device.get(), 4u );
}

GraphicsDevice* Renderer::GetGraphicsDevice() const noexcept(!_DEBUG)
{
	return m_device.get();
}

void Renderer::BeginFrame() noexcept(!_DEBUG)
{
	constexpr float clearColor[4] = { 0.07f, 0.02f, 0.12f, 1.0f };

	m_device->GetContext()->OMSetRenderTargets( 1u, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get() );
	m_device->GetContext()->OMSetDepthStencilState( m_depthStencilState.Get(), 1u );

	m_device->GetContext()->ClearRenderTargetView( m_renderTargetView.Get(), clearColor );
	m_device->GetContext()->ClearDepthStencilView( m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u );
}

void Renderer::EndFrame() noexcept(!_DEBUG)
{
	m_device->GetSwapChain()->Present( 1u, 0u );
}

void Renderer::Render( Scene* scene ) noexcept(!_DEBUG)
{
	//Pass1( scene );

	//constexpr float clearColor[4] = { 0.07f, 0.02f, 0.12f, 1.0f };

	//m_device->GetContext()->OMSetRenderTargets( 1u, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get() );
	//m_device->GetContext()->OMSetDepthStencilState( m_depthStencilState.Get(), 1u );

	//m_device->GetContext()->ClearRenderTargetView( m_renderTargetView.Get(), clearColor );
	//m_device->GetContext()->ClearDepthStencilView( m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u );

	Pass2( scene );
}

void Renderer::Pass1( Scene* scene ) noexcept(!_DEBUG)
{
	const D3D11_VIEWPORT vp = {
		.Width = m_width,
		.Height = m_height,
		.MaxDepth = 1.0f,
	};
	m_device->GetContext()->RSSetViewports( 1u, &vp );

	m_shadowMap->Bind();

	for( auto& object : scene->GetObjects() )
	{
		PerObjectData pod = {
			.objectRef = object,
			.objectPosCB = {
				.worldMatrix = DirectX::XMMatrixTranspose( object.GetWorldMatrix() ),
			},
		};

		PerFrameData pfd = {
		.cameraCB = {
				.viewMatrix = DirectX::XMMatrixTranspose( scene->GetActiveLight()->GetViewMatrix( object.transform.XYZ() ) ),
				.projMatrix = DirectX::XMMatrixTranspose( scene->GetActiveLight()->GetProjectionMatrix() ),
			},
		};
		BindPerFrame( pfd );

		BindPerObject( pod );

		Draw( object );
	}
}

void Renderer::Pass2( Scene* scene ) noexcept(!_DEBUG)
{
	const D3D11_VIEWPORT vp = {
		.Width = m_width,
		.Height = m_height,
		.MaxDepth = 1.0f,
	};
	m_device->GetContext()->RSSetViewports( 1u, &vp );

	PerFrameData pfd = {
		.cameraCB = {
			.viewMatrix = DirectX::XMMatrixTranspose( scene->GetActiveCamera()->GetViewMatrix() ),
			.projMatrix = DirectX::XMMatrixTranspose( scene->GetActiveCamera()->GetProjectionMatrix() ),
		},
	};
	BindPerFrame( pfd );

	std::array<LightPosCB, 4> lpCBs = {};
	std::array<LightCB, 4 > lCBs = {};
	for( uint8_t i = 0; i < 4; ++i )
	{
		lpCBs[i] = {
			.worldPosition = scene->GetLights()[i].transform.XYZ(),
		};
		lCBs[i] = {
			.tint = scene->GetLights()[i].tint,
			.intensity = scene->GetLights()[i].intensity,
		};
	}
	BindPerLight( lpCBs, lCBs );

	for( auto& object : scene->GetObjects() )
	{
		PerObjectData pod = {
			.objectRef = object,
			.objectPosCB = {
				.worldMatrix = DirectX::XMMatrixTranspose( object.GetWorldMatrix() ),
			},
		};
		BindPerObject( pod );

		// TODO: Add dynamic update to materials mid running.
		//if( !m_currentMaterial || m_currentMaterial != object.material.get() ) // if no mat or mat is different
		{
			m_currentMaterial = object.material.get();

			PerMaterialData pmd = {
				.materialCB = {
					.color = object.material->color,
					.shininess = object.material->shininess,
				},
			};
			BindPerMaterial( pmd );
		}

		Draw( object );
	}
}

void Renderer::BindPerFrame( const PerFrameData& data )
{
	m_cameraCB->Update( data.cameraCB );
	m_cameraCB->Bind();
}

void Renderer::BindPerObject( const PerObjectData& data )
{
	data.objectRef.mesh->vB.Bind();
	data.objectRef.mesh->iB.Bind();

	m_objectMatCB->Update( data.objectPosCB );
	m_objectMatCB->Bind();
}

void Renderer::BindPerLight( const std::array<LightPosCB, 4>& lpCBs, const std::array<LightCB, 4>& lCBs )
{
	m_lightPosCB->Update( lpCBs );
	m_lightCB->Update( lCBs );

	m_lightPosCB->Bind();
	m_lightCB->BindPS();
}

void Renderer::BindPerMaterial( const PerMaterialData& data )
{
	m_currentMaterial->vShader->Bind();
	m_currentMaterial->pShader->Bind();
	m_currentMaterial->texture->Bind();
	m_currentMaterial->sampler->Bind();

	m_materialCB->Update( data.materialCB );
	m_materialCB->BindPS();
}

void Renderer::Draw( Object& object ) noexcept(!_DEBUG)
{
	m_device->GetContext()->DrawIndexed( object.mesh->iB.GetCount(), 0u, 0u );
}
