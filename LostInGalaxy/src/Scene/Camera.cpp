#include "pch.h"
#include "Camera.hpp"

DirectX::XMMATRIX Camera::GetViewProjMatrix() noexcept
{
	using namespace DirectX;

	const bool viewUpdated = UpdateView();
	const bool projUpdated = UpdateProjection();

	if( viewUpdated || projUpdated )
	{
		XMMATRIX view = XMLoadFloat4x4( &m_viewMatrixStorage );
		XMMATRIX proj = XMLoadFloat4x4( &m_projMatrixStorage );

		XMMATRIX viewProjMatrix = XMMatrixMultiply( view, proj );
		XMStoreFloat4x4( &m_viewProjMatrixStorage, viewProjMatrix );

		return viewProjMatrix;
	}

	return XMLoadFloat4x4( &m_viewProjMatrixStorage );
}

bool Camera::UpdateView() noexcept
{
	using namespace DirectX;

	if( !transform.IsDirty() ) return false; // false = no update needed

	XMMATRIX rotation = XMMatrixRotationRollPitchYaw( transform.RX(), transform.RY(), transform.RZ() );
	XMVECTOR forward = XMVector3TransformNormal( XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f ), rotation ); // vec3 * mat3x3

	// IMPORTANT NOTE:
	// I was trying to fix the relative axis movements
	// so, for some reason, I tried to multiply the eye vector by the rotation matrix
	// and I got an ORBIT camera!! what an accident :D

	XMVECTOR eye = XMVectorSet( transform.X(), transform.Y(), transform.Z(), 1.0f );
	XMVECTOR at = XMVectorAdd( eye, forward );
	XMVECTOR up = XMVector3TransformNormal( XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f ), rotation );

	XMMATRIX viewMatrix = XMMatrixLookAtLH( eye, at, up );

	XMStoreFloat4x4( &m_viewMatrixStorage, viewMatrix );

	transform.CleanDirt();

	return true; // true = updated
}

bool Camera::UpdateProjection() noexcept
{
	using namespace DirectX;

	if( !m_isProjDirty ) return false; // false = no update needed

	XMMATRIX projMatrix = XMMatrixPerspectiveFovLH( XMConvertToRadians( m_fovAngleY ),
													m_aspectRatio,
													m_nearZ,
													m_farZ );


	XMStoreFloat4x4( &m_projMatrixStorage, projMatrix );

	m_isProjDirty = false;

	return true;  // true = updated
}

void Camera::UpdateProjection( float angle, uint16_t width, uint16_t height, float nearZ, float farZ ) noexcept
{
	m_fovAngleY = angle;
	m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	m_nearZ = nearZ;
	m_farZ = farZ;

	m_isProjDirty = true;
}

void Camera::SetFovAngleY( float angle ) noexcept
{
	m_fovAngleY = angle;
	m_isProjDirty = true;
}

void Camera::SetAspectRatio( float ratio ) noexcept
{
	m_aspectRatio = ratio;
	m_isProjDirty = true;
}

void Camera::SetAspectRatio( uint16_t width, uint16_t height ) noexcept
{
	m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	m_isProjDirty = true;
}

void Camera::SetNearZ( float value ) noexcept
{
	m_nearZ = value;
	m_isProjDirty = true;
}

void Camera::SetFarZ( float value ) noexcept
{
	m_farZ = value;
	m_isProjDirty = true;
}

float Camera::GetFovAngleY() const noexcept
{
	return m_fovAngleY;
}

float Camera::GetAspectRatio() const noexcept
{
	return m_aspectRatio;
}

float Camera::GetNearZ() const noexcept
{
	return m_nearZ;
}

float Camera::GetFarZ() const noexcept
{
	return m_farZ;
}
