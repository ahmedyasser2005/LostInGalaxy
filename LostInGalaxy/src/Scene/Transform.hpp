#pragma once

#include "pch.h"

class Transform {
public:
	// Setters
	void XYZ( float x, float y, float z ) noexcept;
	void XYZ( DirectX::XMVECTOR pos ) noexcept;
	void X( float x ) noexcept;
	void Y( float y ) noexcept;
	void Z( float z ) noexcept;
	void PitchYawRoll( float pitch, float yaw, float roll ) noexcept;
	void Pitch( float pitch ) noexcept;
	void Yaw( float yaw ) noexcept;
	void Roll( float roll ) noexcept;
	void Scale( float scale ) noexcept;
	void CleanDirt() noexcept;

	// Getters
	[[nodiscard]] DirectX::XMVECTOR XYZ() noexcept;
	[[nodiscard]] float X() const noexcept;
	[[nodiscard]] float Y() const noexcept;
	[[nodiscard]] float Z() const noexcept;

	[[nodiscard]] DirectX::XMVECTOR PitchYawRoll() noexcept;
	[[nodiscard]] float Pitch() const noexcept;
	[[nodiscard]] float Yaw() const noexcept;
	[[nodiscard]] float Roll() const noexcept;
	[[nodiscard]] float Scale() const noexcept;
	[[nodiscard]] bool IsDirty() const noexcept;

private:
	float m_x = 0.0f, m_y = 0.0f, m_z = 0.0f;
	float m_pitch = 0.0f, m_yaw = 0.0f, m_roll = 0.0f;
	float m_scale = 1.0f; // uniform scaling

	bool m_isDirty = true;

};
