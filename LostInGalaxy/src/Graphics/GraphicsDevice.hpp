#pragma once

#include "pch.h"

class GraphicsDevice final {
public:
	GraphicsDevice( HWND hwnd, uint32_t width, uint32_t height );
	~GraphicsDevice() noexcept(!_DEBUG) = default;
	GraphicsDevice( const GraphicsDevice& ) = delete;
	GraphicsDevice& operator=( const GraphicsDevice& ) = delete;
	GraphicsDevice( GraphicsDevice&& ) noexcept(!_DEBUG) = default;
	GraphicsDevice& operator=( GraphicsDevice&& ) noexcept(!_DEBUG) = default;

	// Getters
	[[nodiscard]] ID3D11Device* GetDevice() const noexcept(!_DEBUG);
	[[nodiscard]] ID3D11DeviceContext* GetContext() const noexcept(!_DEBUG);
	[[nodiscard]] IDXGISwapChain* GetSwapChain() const noexcept(!_DEBUG);

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;

};
