#include "pch.h"
#include "Transform.hpp"

void Transform::PosXYZ( float x, float y, float z ) noexcept
{
	m_x = x, m_y = y, m_z = z;
	m_isDirty = true;
}

void Transform::X( float x ) noexcept
{
	m_x = x;
	m_isDirty = true;
}

void Transform::Y( float y ) noexcept
{
	m_y = y;
	m_isDirty = true;
}

void Transform::Z( float z ) noexcept
{
	m_z = z;
	m_isDirty = true;
}

void Transform::RotXYZ( float rx, float ry, float rz ) noexcept
{
	m_rx = rx, m_ry = ry, m_rz = rz;
	m_isDirty = true;
}

void Transform::RX( float rx ) noexcept
{
	m_rx = rx;
	m_isDirty = true;
}

void Transform::RY( float ry ) noexcept
{
	m_ry = ry;
	m_isDirty = true;
}

void Transform::RZ( float rz ) noexcept
{
	m_rz = rz;
	m_isDirty = true;
}

void Transform::S( float s ) noexcept
{
	m_s = s;
	m_isDirty = true;
}

void Transform::CleanDirt() noexcept
{
	m_isDirty = true;
}

float Transform::X() const noexcept
{
	return m_x;
}

float Transform::Y() const noexcept
{
	return m_y;
}

float Transform::Z() const noexcept
{
	return m_z;
}

float Transform::RX() const noexcept
{
	return m_rx;
}

float Transform::RY() const noexcept
{
	return m_ry;
}

float Transform::RZ() const noexcept
{
	return m_rz;
}

float Transform::S() const noexcept
{
	return m_s;
}


bool Transform::IsDirty() const noexcept
{
	return m_isDirty;
}
