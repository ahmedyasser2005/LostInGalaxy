#pragma once

#include "pch.h"
#include "Transform.hpp"

class LightSource final {
public:
	[[nodiscard]] DirectX::XMMATRIX GetViewMatrix() noexcept(!_DEBUG);
	[[nodiscard]] DirectX::XMMATRIX GetProjectionMatrix() noexcept(!_DEBUG);

public:
	Transform transform = {};
	DirectX::XMFLOAT3 tint = { 1.0f, 1.0f, 1.0f };
	float intensity = 1.0f;

private:
	DirectX::XMFLOAT4X4 m_viewMatrixStorage = {};
	DirectX::XMFLOAT4X4 m_projMatrixStorage = {};

	DirectX::XMFLOAT3 m_rightVector = {};
	DirectX::XMFLOAT3 m_upVector = {};
	DirectX::XMFLOAT3 m_forwardVector = {};

	float m_width = 30.0f;
	float m_height = 30.0f;
	float m_nearZ = 0.1f;
	float m_farZ = 100.0f;

	bool m_isProjDirty = true;
};
