#pragma once

#include "pch.h"
#include "Graphics/GraphicsDevice.hpp"
#include "Graphics/ConstantBuffer.hpp"

struct ViewProjCB {
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX proj;
};

class Renderer final {
public:
	Renderer( class Window* window );
	~Renderer() noexcept = default;
	Renderer( const Renderer& ) = delete;
	Renderer& operator=( const Renderer& ) = delete;
	Renderer( Renderer&& ) = delete;
	Renderer& operator= ( Renderer&& ) = delete;

	void Render( class Scene* scene ) noexcept;
	void BeginFrame() noexcept;
	void EndFrame() noexcept;

	[[nodiscard]] GraphicsDevice* GetGraphicsDevice() const noexcept;

private:
	void Draw( class Object* object ) noexcept;

	std::unique_ptr<GraphicsDevice> m_device;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	std::unique_ptr<ConstantBuffer<DirectX::XMMATRIX>> m_worldCBuffer;
	std::unique_ptr<ConstantBuffer<ViewProjCB>> m_viewprojCBuffer;

};
