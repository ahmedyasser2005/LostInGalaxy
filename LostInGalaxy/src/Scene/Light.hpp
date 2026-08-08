#pragma once

#include "pch.h"

struct alignas(16) LightProps {
	DirectX::XMFLOAT4 tint;
	float intensity;
	float shininess;
};

struct LightSource {
	LightProps props;
	DirectX::XMFLOAT4 position;
};
