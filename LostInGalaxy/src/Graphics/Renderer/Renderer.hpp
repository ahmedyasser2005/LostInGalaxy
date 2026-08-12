#pragma once

#include "pch.h"
#include "Graphics/GraphicsDevice.hpp"
#include "Graphics/ConstantBuffer.hpp"
#include "Graphics/ShadowMap.hpp"

struct CameraCB {
	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX projMatrix;
};
struct ObjectMatCB {
	DirectX::XMMATRIX worldMatrix;
};
struct LightPosCB {
	DirectX::XMVECTOR worldPosition;
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
	~Renderer() noexcept(!_DEBUG) = default;
	Renderer( const Renderer& ) = delete;
	Renderer& operator=( const Renderer& ) = delete;
	Renderer( Renderer&& ) = delete;
	Renderer& operator= ( Renderer&& ) = delete;

	[[nodiscard]] GraphicsDevice* GetGraphicsDevice() const noexcept(!_DEBUG);

	void BeginFrame() noexcept(!_DEBUG);
	void EndFrame() noexcept(!_DEBUG);

	void Render( class Scene* scene ) noexcept(!_DEBUG);

private:
	void Pass1( class Scene* scene ) noexcept(!_DEBUG);
	void Pass2( class Scene* scene ) noexcept(!_DEBUG);

	void BindPerFrame( const PerFrameData& data );
	void BindPerObject( const PerObjectData& data );
	void BindPerLight( const std::array<LightPosCB, 4>& lpCBs, const std::array<LightCB, 4>& lCBs );
	void BindPerMaterial( const PerMaterialData& data );

	void Draw( class Object& object ) noexcept(!_DEBUG);

private:
	std::unique_ptr<GraphicsDevice> m_device;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>				m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>				m_depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>				m_depthStencilView;
	std::unique_ptr<ConstantBuffer<CameraCB>>					m_cameraCB;
	std::unique_ptr<ConstantBuffer<ObjectMatCB>>				m_objectMatCB;
	std::unique_ptr<ConstantBuffer<std::array<LightPosCB, 4>>>	m_lightPosCB;
	std::unique_ptr<ConstantBuffer<std::array<LightCB, 4>>>		m_lightCB;
	std::unique_ptr<ConstantBuffer<MaterialCB>>					m_materialCB;

	std::shared_ptr<VertexShader> m_shadowMappingVS;
	std::unique_ptr<ShadowMap> m_shadowMap;

	struct Material* m_currentMaterial;

	float m_width, m_height;
};
