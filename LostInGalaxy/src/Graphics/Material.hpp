#pragma once

#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Sampler.hpp"

struct Material {
	Shader shader;
	Texture texture;
	Sampler sampler;
	DirectX::XMFLOAT3 color = {};
	float shininess = 32.0f;
};
