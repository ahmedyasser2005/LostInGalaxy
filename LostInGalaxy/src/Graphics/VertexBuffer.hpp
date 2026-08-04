#pragma once

#include "pch.h"
#include "Graphics/IBind.hpp"

class VertexBuffer final : public IBind {
public:
	VertexBuffer( GraphicsDevice* graphicsDevice, std::span<struct Vertex> vertices );
	~VertexBuffer() noexcept = default;
	VertexBuffer( const VertexBuffer& ) = delete;
	VertexBuffer& operator=( const VertexBuffer& ) = delete;
	VertexBuffer( VertexBuffer&& ) noexcept = default;
	VertexBuffer& operator=( VertexBuffer&& ) noexcept = default;

	void Bind() const noexcept override;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;

};