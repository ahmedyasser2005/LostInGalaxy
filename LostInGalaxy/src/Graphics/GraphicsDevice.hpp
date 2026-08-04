#pragma once

#include "pch.h"

class GraphicsDevice final {
public:
	GraphicsDevice( HWND hwnd, uint32_t width, uint32_t height );
	~GraphicsDevice() noexcept = default;
	GraphicsDevice( const GraphicsDevice& ) = delete;
	GraphicsDevice& operator=( const GraphicsDevice& ) = delete;
	GraphicsDevice( GraphicsDevice&& ) noexcept = default;
	GraphicsDevice& operator=( GraphicsDevice&& ) noexcept = default;

	// Getters
	[[nodiscard]] ID3D11Device* GetDevice() const noexcept;
	[[nodiscard]] ID3D11DeviceContext* GetContext() const noexcept;
	[[nodiscard]] IDXGISwapChain* GetSwapChain() const noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;

};
