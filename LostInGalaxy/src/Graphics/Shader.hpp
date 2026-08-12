#pragma once

#include "pch.h"
#include "Graphics/IBind.hpp"

class Shader : public IBind {
public:
	Shader( GraphicsDevice* graphicsDevice, const std::filesystem::path& shaderPath ) noexcept(!_DEBUG);
	virtual ~Shader() noexcept(!_DEBUG) = default;

protected:
	Microsoft::WRL::ComPtr<ID3DBlob> m_blob;
};

class VertexShader final : public Shader {
public:
	VertexShader( GraphicsDevice* graphicsDevice, const std::filesystem::path& shaderPath ) noexcept(!_DEBUG);

	void Bind() const noexcept(!_DEBUG) override;

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout>  m_inputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
};

class PixelShader final : public Shader {
public:
	PixelShader( GraphicsDevice* graphicsDevice, const std::filesystem::path& shaderPath ) noexcept(!_DEBUG);

	void Bind() const noexcept(!_DEBUG) override;

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
};
