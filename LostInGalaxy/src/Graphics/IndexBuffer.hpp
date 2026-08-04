#pragma once

#include "pch.h"
#include "Graphics/IBind.hpp"

class IndexBuffer final : public IBind {
public:
	IndexBuffer( GraphicsDevice* graphicsDevice, std::span<uint32_t> indices );
	~IndexBuffer() noexcept = default;
	IndexBuffer( const IndexBuffer& ) = delete;
	IndexBuffer& operator=( const IndexBuffer& ) = delete;
	IndexBuffer( IndexBuffer&& ) noexcept = default;
	IndexBuffer& operator=( IndexBuffer&& ) noexcept = default;

	void Bind() const noexcept override;
	uint32_t GetCount() const noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	const uint32_t m_count;

};