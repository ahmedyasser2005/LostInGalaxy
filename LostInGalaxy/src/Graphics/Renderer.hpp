#pragma once

#include "pch.h"
#include "Graphics/GraphicsDevice.hpp"
#include "Graphics/ConstantBuffer.hpp"

struct CameraCB {
	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX projMatrix;
};
struct ObjectMatCB {
	DirectX::XMMATRIX worldMatrix;
};
struct alignas(16) LightPosCB {
	DirectX::XMFLOAT4 worldPosition;
};
struct alignas(16) LightCB {
	DirectX::XMFLOAT3 tint;
	float intensity;
};
struct alignas(16) MaterialCB {
	DirectX::XMFLOAT3 color;
	float shininess;
};

struct PerFrameData {
	CameraCB cameraCB;
};
struct PerObjectData {
	class Object& objectRef;
	ObjectMatCB objectPosCB;
};
struct PerLightData {
	LightPosCB lightPosCB;
	LightCB lightCB;
};
struct PerMaterialData {
	MaterialCB materialCB;
};



class Renderer final {
public:
	Renderer( class Window* window );
	~Renderer() noexcept = default;
	Renderer( const Renderer& ) = delete;
	Renderer& operator=( const Renderer& ) = delete;
	Renderer( Renderer&& ) = delete;
	Renderer& operator= ( Renderer&& ) = delete;

	[[nodiscard]] GraphicsDevice* GetGraphicsDevice() const noexcept;

	void BeginFrame() noexcept;
	void EndFrame() noexcept;

	void Render( class Scene* scene ) noexcept;

private:

	void BindPerFrame( const PerFrameData& data );
	void BindPerObject( const PerObjectData& data );
	void BindPerLight( const PerLightData& data );
	void BindPerMaterial( const PerMaterialData& data );

	void Draw( class Object& object ) noexcept;

	std::unique_ptr<GraphicsDevice> m_device;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;

	std::unique_ptr<ConstantBuffer<CameraCB>>	 m_cameraCB;
	std::unique_ptr<ConstantBuffer<ObjectMatCB>> m_objectMatCB;
	std::unique_ptr<ConstantBuffer<LightPosCB>>	 m_lightPosCB;
	std::unique_ptr<ConstantBuffer<LightCB>>	 m_lightCB;
	std::unique_ptr<ConstantBuffer<MaterialCB>>	 m_materialCB;

	struct Material* m_currentMaterial;

};
