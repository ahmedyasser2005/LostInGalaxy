#pragma once

#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Sampler.hpp"

struct Material {
	std::shared_ptr<VertexShader> vShader;
	std::shared_ptr<PixelShader> pShader;
	std::shared_ptr<Texture> texture;
	std::shared_ptr<Sampler> sampler;

	DirectX::XMFLOAT3 color = {};
	float shininess = 32.0f;
};
