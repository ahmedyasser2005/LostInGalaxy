#pragma once

#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Sampler.hpp"

struct Material {
	Material( Shader&& shader, Texture&& texture, Sampler&& sampler ) :
		shader( std::move( shader ) ),
		texture( std::move( texture ) ),
		sampler( std::move( sampler ) )
	{}

	Shader shader;
	Texture texture;
	Sampler sampler;
};
