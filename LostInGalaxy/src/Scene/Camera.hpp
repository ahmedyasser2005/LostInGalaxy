#pragma once

#include "pch.h"
#include "Scene/Transform.hpp"

class Camera {
public:
	void SetFovAngleY( float angle ) noexcept;
	void SetAspectRatio( float ratio ) noexcept;
	void SetAspectRatio( uint16_t width, uint16_t height ) noexcept;
	void SetNearZ( float value ) noexcept;
	void SetFarZ( float value ) noexcept;
	void UpdateProjection( float angle, uint16_t width, uint16_t height, float nearZ, float farZ ) noexcept;

	[[nodiscard]] DirectX::XMMATRIX GetViewProjMatrix() noexcept;
	[[nodiscard]] float GetFovAngleY() const noexcept;
	[[nodiscard]] float GetAspectRatio() const noexcept;
	[[nodiscard]] float GetNearZ() const noexcept;
	[[nodiscard]] float GetFarZ() const noexcept;

private:
	bool UpdateView() noexcept;
	bool UpdateProjection() noexcept;

public:
	Transform transform = {};

private:
	DirectX::XMFLOAT4X4 m_viewProjMatrixStorage = {};
	DirectX::XMFLOAT4X4 m_viewMatrixStorage = {};
	DirectX::XMFLOAT4X4 m_projMatrixStorage = {};

	float m_fovAngleY = 60.0f;
	float m_aspectRatio = 1280.0f / 720.0f;
	float m_nearZ = 0.1f;
	float m_farZ = 100.0f;

	bool m_isProjDirty = true;
};
