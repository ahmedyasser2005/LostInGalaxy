#pragma once

#include "pch.h"
#include "Graphics/Shader.hpp"

class ShadowMap final : public IBind {
public:
	ShadowMap( GraphicsDevice* graphicsDevice, std::shared_ptr<VertexShader> vs, uint32_t width, uint32_t height ) noexcept(!_DEBUG);

	ID3D11DepthStencilView* GetDSV() const noexcept(!_DEBUG);
	ID3D11ShaderResourceView* GetSRV() const noexcept(!_DEBUG);

	void Bind() const noexcept(!_DEBUG) override;

private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture2D;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;
	std::shared_ptr<VertexShader> m_vShader;
};
