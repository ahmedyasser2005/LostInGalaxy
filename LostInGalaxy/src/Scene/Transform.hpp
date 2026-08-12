#pragma once

#include "pch.h"

class Transform {
public:
	// Setters
	void XYZ( float x, float y, float z ) noexcept(!_DEBUG);
	void XYZ( DirectX::XMVECTOR pos ) noexcept(!_DEBUG);
	void X( float x ) noexcept(!_DEBUG);
	void Y( float y ) noexcept(!_DEBUG);
	void Z( float z ) noexcept(!_DEBUG);
	void PitchYawRoll( float pitch, float yaw, float roll ) noexcept(!_DEBUG);
	void Pitch( float pitch ) noexcept(!_DEBUG);
	void Yaw( float yaw ) noexcept(!_DEBUG);
	void Roll( float roll ) noexcept(!_DEBUG);
	void Scale( float scale ) noexcept(!_DEBUG);
	void CleanDirt() noexcept(!_DEBUG);

	// Getters
	[[nodiscard]] DirectX::XMVECTOR XYZ() noexcept(!_DEBUG);
	[[nodiscard]] float X() const noexcept(!_DEBUG);
	[[nodiscard]] float Y() const noexcept(!_DEBUG);
	[[nodiscard]] float Z() const noexcept(!_DEBUG);

	[[nodiscard]] DirectX::XMVECTOR PitchYawRoll() noexcept(!_DEBUG);
	[[nodiscard]] float Pitch() const noexcept(!_DEBUG);
	[[nodiscard]] float Yaw() const noexcept(!_DEBUG);
	[[nodiscard]] float Roll() const noexcept(!_DEBUG);
	[[nodiscard]] float Scale() const noexcept(!_DEBUG);
	[[nodiscard]] bool IsDirty() const noexcept(!_DEBUG);

private:
	float m_x = 0.0f, m_y = 0.0f, m_z = 0.0f;
	float m_pitch = 0.0f, m_yaw = 0.0f, m_roll = 0.0f;
	float m_scale = 1.0f; // uniform scaling

	bool m_isDirty = true;

};
