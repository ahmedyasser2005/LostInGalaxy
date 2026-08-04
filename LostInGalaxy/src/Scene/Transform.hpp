#pragma once

#include "pch.h"

class Transform {
public:
	// Setters
	void PosXYZ( float x, float y, float z ) noexcept;
	void X( float value ) noexcept;
	void Y( float value ) noexcept;
	void Z( float value ) noexcept;
	void RotXYZ( float rx, float ry, float rz ) noexcept;
	void RX( float rx ) noexcept;
	void RY( float ry ) noexcept;
	void RZ( float rz ) noexcept;
	void S( float s ) noexcept;
	void CleanDirt() noexcept;

	// Getters
	[[nodiscard]] float X() const noexcept;
	[[nodiscard]] float Y() const noexcept;
	[[nodiscard]] float Z() const noexcept;
	[[nodiscard]] float RX() const noexcept;
	[[nodiscard]] float RY() const noexcept;
	[[nodiscard]] float RZ() const noexcept;
	[[nodiscard]] float S() const noexcept;
	[[nodiscard]] bool IsDirty() const noexcept;

private:
	float m_x = 0.0f, m_y = 0.0f, m_z = 0.0f;
	float m_rx = 0.0f, m_ry = 0.0f, m_rz = 0.0f; // x = yaw(horizontal), y = pitch(vertical), z = roll
	float m_s = 1.0f; // uniform scaling
	bool m_isDirty = true;

};
