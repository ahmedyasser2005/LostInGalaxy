#pragma once

#include "Graphics/Shader.hpp"
//#include "Graphics/Texture.hpp"
//#include "Graphics/Sampler.hpp"

struct Material {
	Material( Shader* shader ) : shader( shader ) {}

	Shader* shader;
	//Texture* texture;
	//Sampler* sampler;
};
