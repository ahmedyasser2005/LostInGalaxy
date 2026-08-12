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

	void Bind() const noexcept(!_DEBUG) override;
	uint16_t GetSlot() const noexcept(!_DEBUG);

private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_textureBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;
	const uint16_t m_slot;

};
