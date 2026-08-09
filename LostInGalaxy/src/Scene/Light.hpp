#pragma once

#include "pch.h"

struct alignas(16) LightSource {
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT3 tint;
	float intensity;
};
