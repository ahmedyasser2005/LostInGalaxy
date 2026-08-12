#include "pch.h"
#include "Transform.hpp"

void Transform::XYZ( float x, float y, float z ) noexcept(!_DEBUG)
{
	m_x = x, m_y = y, m_z = z;
	m_isDirty = true;
}

void Transform::XYZ( DirectX::XMVECTOR pos ) noexcept(!_DEBUG)
{
	m_x = pos.m128_f32[0], m_y = pos.m128_f32[1], m_z = pos.m128_f32[2];
	m_isDirty = true;
}

void Transform::X( float x ) noexcept(!_DEBUG)
{
	m_x = x;
	m_isDirty = true;
}

void Transform::Y( float y ) noexcept(!_DEBUG)
{
	m_y = y;
	m_isDirty = true;
}

void Transform::Z( float z ) noexcept(!_DEBUG)
{
	m_z = z;
	m_isDirty = true;
}

void Transform::PitchYawRoll( float pitch, float yaw, float roll ) noexcept(!_DEBUG)
{
	m_pitch = pitch, m_yaw = yaw, m_roll = roll;
	m_isDirty = true;
}

void Transform::Pitch( float pitch ) noexcept(!_DEBUG)
{
	m_pitch = pitch;
	m_isDirty = true;
}

void Transform::Yaw( float yaw ) noexcept(!_DEBUG)
{
	m_yaw = yaw;
	m_isDirty = true;
}

void Transform::Roll( float roll ) noexcept(!_DEBUG)
{
	m_roll = roll;
	m_isDirty = true;
}

void Transform::Scale( float scale ) noexcept(!_DEBUG)
{
	m_scale = scale;
	m_isDirty = true;
}

void Transform::CleanDirt() noexcept(!_DEBUG)
{
	m_isDirty = true;
}

DirectX::XMVECTOR Transform::XYZ() noexcept(!_DEBUG)
{
	return DirectX::XMVECTOR( { m_x, m_y, m_z } );
}

DirectX::XMVECTOR Transform::PitchYawRoll() noexcept(!_DEBUG)
{
	return DirectX::XMVECTOR( { m_pitch, m_yaw, m_roll } );
}

float Transform::X() const noexcept(!_DEBUG)
{
	return m_x;
}

float Transform::Y() const noexcept(!_DEBUG)
{
	return m_y;
}

float Transform::Z() const noexcept(!_DEBUG)
{
	return m_z;
}

float Transform::Pitch() const noexcept(!_DEBUG)
{
	return m_pitch;
}

float Transform::Yaw() const noexcept(!_DEBUG)
{
	return m_yaw;
}

float Transform::Roll() const noexcept(!_DEBUG)
{
	return m_roll;
}

float Transform::Scale() const noexcept(!_DEBUG)
{
	return m_scale;
}


bool Transform::IsDirty() const noexcept(!_DEBUG)
{
	return m_isDirty;
}
