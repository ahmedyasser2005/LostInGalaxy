#pragma once

#include "pch.h"
#include "Graphics/Mesh.hpp"
#include "Graphics/Material.hpp"
#include "Scene/Transform.hpp"

class Object {
public:
	Object( std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material ) :
		mesh( mesh ), material( material )
	{}

	[[nodiscard]] DirectX::XMMATRIX GetWorldMatrix() noexcept(!_DEBUG);

	Transform transform = {};
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Material> material;

private:
	DirectX::XMFLOAT4X4 m_worldMatrixStorage = {};

};
