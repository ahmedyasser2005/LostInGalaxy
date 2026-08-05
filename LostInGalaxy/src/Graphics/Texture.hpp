#pragma once

#include "pch.h"
#include "Graphics/IBind.hpp"

struct Image {
	DirectX::ScratchImage scratchImage = {};
	DirectX::TexMetadata texMetadata = {};
};

class Texture final : public IBind {
public:
	Texture( GraphicsDevice* graphicsDevice, Image&& image, uint16_t slot );
	~Texture() noexcept = default;
	Texture( const Texture& ) = delete;
	Texture& operator=( const Texture& ) = delete;
	Texture( Texture&& ) noexcept = default;
	Texture& operator=( Texture&& ) noexcept = default;

	void Bind() const noexcept override;

	uint16_t GetSlot() const noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_textureBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;
	const uint16_t m_slot;

};
