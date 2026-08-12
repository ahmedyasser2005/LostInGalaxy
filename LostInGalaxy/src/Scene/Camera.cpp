#include "pch.h"
#include "Camera.hpp"

//DirectX::XMMATRIX Camera::GetViewProjMatrix() noexcept(!_DEBUG)
//{
//	using namespace DirectX;
//
//	const bool viewUpdated = UpdateView();
//	const bool projUpdated = UpdateProjection();
//
//	if( viewUpdated || projUpdated )
//	{
//		XMMATRIX view = XMLoadFloat4x4( &m_viewMatrixStorage );
//		XMMATRIX proj = XMLoadFloat4x4( &m_projMatrixStorage );
//
//		XMMATRIX viewProjMatrix = XMMatrixMultiply( view, proj );
//		XMStoreFloat4x4( &m_viewProjMatrixStorage, viewProjMatrix );
//
//		return viewProjMatrix;
//	}
//
//	return XMLoadFloat4x4( &m_viewProjMatrixStorage );
//}

DirectX::XMMATRIX Camera::GetViewMatrix() noexcept(!_DEBUG)
{
	using namespace DirectX;

	if( !transform.IsDirty() ) return XMLoadFloat4x4( &m_viewMatrixStorage );

	XMMATRIX translation = XMMatrixTranslationFromVector( XMVectorNegate( transform.XYZ() ) );
	XMMATRIX rotation = XMMatrixRotationRollPitchYawFromVector( transform.PitchYawRoll() );

	XMStoreFloat3( &m_rightVector, XMVector3Transform( g_XMIdentityR0, rotation ) );
	XMStoreFloat3( &m_upVector, XMVector3Transform( g_XMIdentityR1, rotation ) );
	XMStoreFloat3( &m_forwardVector, XMVector3Transform( g_XMIdentityR2, rotation ) );

	XMMATRIX viewMatrix = translation * XMMatrixTranspose( rotation );

	XMStoreFloat4x4( &m_viewMatrixStorage, viewMatrix );

	transform.CleanDirt();

	return viewMatrix;
}

DirectX::XMMATRIX Camera::GetProjectionMatrix() noexcept(!_DEBUG)
{
	using namespace DirectX;

	if( !m_isProjDirty ) return XMLoadFloat4x4( &m_projMatrixStorage );

	XMMATRIX projMatrix = XMMatrixPerspectiveFovLH( XMConvertToRadians( m_fovAngleY ),
													m_aspectRatio,
													m_nearZ,
													m_farZ );

	XMStoreFloat4x4( &m_projMatrixStorage, projMatrix );

	m_isProjDirty = false;

	return projMatrix;
}

void Camera::UpdateProjection( float angle, uint16_t width, uint16_t height, float nearZ, float farZ ) noexcept(!_DEBUG)
{
	m_fovAngleY = angle;
	m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	m_nearZ = nearZ;
	m_farZ = farZ;

	m_isProjDirty = true;
}

void Camera::SetFovAngleY( float angle ) noexcept(!_DEBUG)
{
	m_fovAngleY = angle;
	m_isProjDirty = true;
}

void Camera::SetAspectRatio( float ratio ) noexcept(!_DEBUG)
{
	m_aspectRatio = ratio;
	m_isProjDirty = true;
}

void Camera::SetAspectRatio( uint16_t width, uint16_t height ) noexcept(!_DEBUG)
{
	m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	m_isProjDirty = true;
}

void Camera::SetNearZ( float value ) noexcept(!_DEBUG)
{
	m_nearZ = value;
	m_isProjDirty = true;
}

void Camera::SetFarZ( float value ) noexcept(!_DEBUG)
{
	m_farZ = value;
	m_isProjDirty = true;
}

float Camera::GetFovAngleY() const noexcept(!_DEBUG)
{
	return m_fovAngleY;
}

float Camera::GetAspectRatio() const noexcept(!_DEBUG)
{
	return m_aspectRatio;
}

float Camera::GetNearZ() const noexcept(!_DEBUG)
{
	return m_nearZ;
}

float Camera::GetFarZ() const noexcept(!_DEBUG)
{
	return m_farZ;
}

// Controls
void Camera::MoveForward( float dt ) noexcept(!_DEBUG)
{
	using namespace DirectX;
	XMVECTOR forward = XMLoadFloat3( &m_forwardVector );
	transform.XYZ( XMVectorAdd( transform.XYZ(), forward * speed * dt ) );
}

void Camera::MoveBackward( float dt ) noexcept(!_DEBUG)
{
	using namespace DirectX;
	XMVECTOR forward = XMLoadFloat3( &m_forwardVector );
	transform.XYZ( XMVectorSubtract( transform.XYZ(), forward * speed * dt ) );
}

void Camera::MoveRight( float dt ) noexcept(!_DEBUG)
{
	using namespace DirectX;
	XMVECTOR right = XMLoadFloat3( &m_rightVector );
	transform.XYZ( XMVectorAdd( transform.XYZ(), right * speed * dt ) );
}

void Camera::MoveLeft( float dt ) noexcept(!_DEBUG)
{
	using namespace DirectX;
	XMVECTOR right = XMLoadFloat3( &m_rightVector );
	transform.XYZ( XMVectorSubtract( transform.XYZ(), right * speed * dt ) );
}

void Camera::MoveUp( float dt ) noexcept(!_DEBUG)
{
	using namespace DirectX;
	XMVECTOR up = XMLoadFloat3( &m_upVector );
	transform.XYZ( XMVectorAdd( transform.XYZ(), up * speed * dt ) );
}

void Camera::MoveDown( float dt ) noexcept(!_DEBUG)
{
	using namespace DirectX;
	XMVECTOR up = XMLoadFloat3( &m_upVector );
	transform.XYZ( XMVectorSubtract( transform.XYZ(), up * speed * dt ) );
}

void Camera::Pitch( float delta, float dt ) noexcept(!_DEBUG)
{
	transform.Pitch( transform.Pitch() + (delta * sensitivity * dt) );
}

void Camera::Yaw( float delta, float dt ) noexcept(!_DEBUG)
{
	transform.Yaw( transform.Yaw() + (delta * sensitivity * dt) );
}

void Camera::Roll( float delta, float dt ) noexcept(!_DEBUG)
{
	transform.Roll( transform.Roll() + (delta * sensitivity * dt) );
}
