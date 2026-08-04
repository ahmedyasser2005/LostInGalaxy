#pragma once

#include "pch.h"
#include "Graphics/GraphicsDevice.hpp"
#include "Graphics/ConstantBuffer.hpp"

class Renderer final {
public:
	Renderer( class Window* window );
	~Renderer() noexcept = default;
	Renderer( const Renderer& ) = delete;
	Renderer& operator=( const Renderer& ) = delete;
	Renderer( Renderer&& ) = delete;
	Renderer& operator= ( Renderer&& ) = delete;

	void Render( class Scene* scene ) noexcept;

	[[nodiscard]] GraphicsDevice* GetGraphicsDevice() const noexcept;

private:
	// Private Methods
	void BeginFrame() noexcept;
	void EndFrame() noexcept;
	void Draw( class Object* object ) noexcept;

	// Member Data
	std::unique_ptr<GraphicsDevice> m_device;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	std::unique_ptr<ConstantBuffer<DirectX::XMMATRIX>> m_worldCBuffer;
	std::unique_ptr<ConstantBuffer<DirectX::XMMATRIX>> m_viewprojCBuffer;

};
