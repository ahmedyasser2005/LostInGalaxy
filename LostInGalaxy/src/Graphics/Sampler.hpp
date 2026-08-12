#pragma once

#include "pch.h"
#include "Graphics/IBind.hpp"

class Sampler final : public IBind {
public:
	Sampler( GraphicsDevice* graphicsDevice, uint16_t slot );

	void Bind() const noexcept(!_DEBUG) override;
	uint16_t GetSlot() const noexcept(!_DEBUG);

private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;
	const uint16_t m_slot;

};
