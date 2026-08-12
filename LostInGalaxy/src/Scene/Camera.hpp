#pragma once

#include "pch.h"
#include "Scene/Transform.hpp"

class Camera {
public:
	void SetFovAngleY( float angle ) noexcept(!_DEBUG);
	void SetAspectRatio( float ratio ) noexcept(!_DEBUG);
	void SetAspectRatio( uint16_t width, uint16_t height ) noexcept(!_DEBUG);
	void SetNearZ( float value ) noexcept(!_DEBUG);
	void SetFarZ( float value ) noexcept(!_DEBUG);
	void UpdateProjection( float angle, uint16_t width, uint16_t height, float nearZ, float farZ ) noexcept(!_DEBUG);

	[[nodiscard]] DirectX::XMMATRIX GetViewMatrix() noexcept(!_DEBUG);
	[[nodiscard]] DirectX::XMMATRIX GetProjectionMatrix() noexcept(!_DEBUG);

	[[nodiscard]] float GetFovAngleY() const noexcept(!_DEBUG);
	[[nodiscard]] float GetAspectRatio() const noexcept(!_DEBUG);
	[[nodiscard]] float GetNearZ() const noexcept(!_DEBUG);
	[[nodiscard]] float GetFarZ() const noexcept(!_DEBUG);

	// Controls
	void MoveForward( float dt ) noexcept(!_DEBUG);
	void MoveBackward( float dt ) noexcept(!_DEBUG);
	void MoveRight( float dt ) noexcept(!_DEBUG);
	void MoveLeft( float dt ) noexcept(!_DEBUG);
	void MoveUp( float dt ) noexcept(!_DEBUG);
	void MoveDown( float dt ) noexcept(!_DEBUG);
	void Pitch( float delta, float dt ) noexcept(!_DEBUG);
	void Yaw( float delta, float dt ) noexcept(!_DEBUG);
	void Roll( float delta, float dt ) noexcept(!_DEBUG);

private:

public:
	Transform transform = {};
	float speed = 10.0f;
	float sensitivity = 1.0f;

private:
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
