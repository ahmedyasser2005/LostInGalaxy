#pragma once

#include "pch.h"
#include "Graphics/IBind.hpp"

class IndexBuffer final : public IBind {
public:
	IndexBuffer( GraphicsDevice* graphicsDevice, std::span<uint32_t> indices );

	void Bind() const noexcept(!_DEBUG) override;
	uint32_t GetCount() const noexcept(!_DEBUG);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	const uint32_t m_count;

};
