#pragma once

#include "pch.h"
#include "Scene/Mesh.hpp"
#include "Scene/Material.hpp"
#include "Scene/Transform.hpp"

class Object {
public:
	Object( std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material ) :
		mesh( mesh ), material( material )
	{}

	[[nodiscard]] DirectX::XMMATRIX GetWorldMatrix() noexcept;
	Transform transform = {};

	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Material> material;

private:
	DirectX::XMFLOAT4X4 m_worldMatrixStorage = {};

};
