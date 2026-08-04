#include "pch.h"
#include "GraphicsDevice.hpp"

GraphicsDevice::GraphicsDevice( HWND hwnd, uint32_t width, uint32_t height )
{
	HRESULT hr = S_OK;

	UINT createDeviceFlags = 0u;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	constexpr std::array<D3D_FEATURE_LEVEL, 2> featureLevels = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };
	D3D_FEATURE_LEVEL supportedFeatureLevels = {};
	hr = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createDeviceFlags,
		featureLevels.data(),
		static_cast<UINT>(featureLevels.size()),
		D3D11_SDK_VERSION,
		&m_device,
		&supportedFeatureLevels,
		&m_context
	);
	assert( !FAILED( hr ) );

	Microsoft::WRL::ComPtr<IDXGIDevice> idxgiDevice;
	hr = m_device.As( &idxgiDevice );
	assert( !FAILED( hr ) );

	Microsoft::WRL::ComPtr<IDXGIAdapter> idxgiAdapter;
	hr = idxgiDevice->GetAdapter( &idxgiAdapter );
	assert( !FAILED( hr ) );

	Microsoft::WRL::ComPtr<IDXGIFactory> idxgiFactory;
	hr = idxgiAdapter->GetParent( __uuidof(IDXGIFactory), &idxgiFactory );
	assert( !FAILED( hr ) );

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {
		.BufferDesc = {
			.Width = width, .Height = height,
			.RefreshRate = {.Numerator = 0u, .Denominator = 1u, },
			.Format = DXGI_FORMAT_R8G8B8A8_UNORM,
			.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
			.Scaling = DXGI_MODE_SCALING_UNSPECIFIED,
		},
		.SampleDesc = {.Count = 1u, .Quality = 0u, },
		.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
		.BufferCount = 2u,
		.OutputWindow = hwnd,
		.Windowed = TRUE,
		.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
		.Flags = 0u,
	};
	hr = idxgiFactory->CreateSwapChain( m_device.Get(), &swapChainDesc, &m_swapChain );
	assert( !FAILED( hr ) );
}

ID3D11Device* GraphicsDevice::GetDevice() const noexcept
{
	return m_device.Get();
}

ID3D11DeviceContext* GraphicsDevice::GetContext() const noexcept
{
	return m_context.Get();
}

IDXGISwapChain* GraphicsDevice::GetSwapChain() const noexcept
{
	return m_swapChain.Get();
}
