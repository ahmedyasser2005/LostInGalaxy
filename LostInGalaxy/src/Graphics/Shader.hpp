#pragma once

#include "pch.h"
#include "Graphics/IBind.hpp"

class Shader final : public IBind {
public:
	Shader( GraphicsDevice* graphicsDevice, const wchar_t* vShaderFilename, const wchar_t* pShaderFilename );
	~Shader() noexcept = default;
	Shader( const Shader& ) = delete;
	Shader& operator=( const Shader& ) = delete;
	Shader( Shader&& ) noexcept = default;
	Shader& operator=( Shader&& ) noexcept = default;

	void Bind() const noexcept;

private:
	Microsoft::WRL::ComPtr<ID3DBlob> m_vShaderBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> m_pShaderBlob;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pShader;

};
