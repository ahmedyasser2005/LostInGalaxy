#pragma once

#include "pch.h"
#include "Graphics/IBind.hpp"

class VertexBuffer final : public IBind {
public:
	VertexBuffer( GraphicsDevice* graphicsDevice, std::span<struct Vertex> vertices );

	void Bind() const noexcept(!_DEBUG) override;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;

};
