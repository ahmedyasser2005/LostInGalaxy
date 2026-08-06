#pragma once

#include "pch.h"
#include "Graphics/GraphicsDevice.hpp"
#include "Graphics/VertexBuffer.hpp"
#include "Graphics/IndexBuffer.hpp"

struct Vertex {
	DirectX::XMFLOAT3 position;
	//std::array<uint8_t, 4> color;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 uv;
};

// Holds GPU Vertex & Index buffer.
struct Mesh {
	Mesh( GraphicsDevice* graphicsDevice, std::span<Vertex> vertices, std::span<uint32_t> indices ) :
		vB( VertexBuffer( graphicsDevice, vertices ) ),
		iB( IndexBuffer( graphicsDevice, indices ) )
	{}

	VertexBuffer vB;
	IndexBuffer iB;
};
