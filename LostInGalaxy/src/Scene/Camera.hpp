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

	// Controls
	void MoveForward( float dt ) noexcept;
	void MoveBackward( float dt ) noexcept;
	void MoveRight( float dt ) noexcept;
	void MoveLeft( float dt ) noexcept;
	void MoveUp( float dt ) noexcept;
	void MoveDown( float dt ) noexcept;
	void Pitch( float delta, float dt ) noexcept;
	void Yaw( float delta, float dt ) noexcept;
	void Roll( float delta, float dt ) noexcept;

private:
	bool UpdateView() noexcept;
	bool UpdateProjection() noexcept;

public:
	Transform transform = {};
	float speed = 10.0f;
	float sensitivity = 1.0f;

private:
	DirectX::XMFLOAT4X4 m_viewProjMatrixStorage = {};
	DirectX::XMFLOAT4X4 m_viewMatrixStorage = {};
	DirectX::XMFLOAT4X4 m_projMatrixStorage = {};

	DirectX::XMFLOAT3 m_rightVector = {};
	DirectX::XMFLOAT3 m_upVector = {};
	DirectX::XMFLOAT3 m_forwardVector = {};

	float m_fovAngleY = 60.0f;
	float m_aspectRatio = 16.0f / 9.0f;
	float m_nearZ = 0.1f;
	float m_farZ = 100.0f;

	bool m_isProjDirty = true;
};
