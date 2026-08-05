#pragma once

#include "pch.h"
#include "Graphics/IBind.hpp"

class Sampler final : public IBind {
public:
	Sampler( GraphicsDevice* graphicsDevice, uint16_t slot );
	~Sampler() noexcept = default;
	Sampler( const Sampler& ) = delete;
	Sampler& operator=( const Sampler& ) = delete;
	Sampler( Sampler&& ) noexcept = default;
	Sampler& operator=( Sampler&& ) noexcept = default;

	void Bind() const noexcept override;

	uint16_t GetSlot() const noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;
	const uint16_t m_slot;

};
