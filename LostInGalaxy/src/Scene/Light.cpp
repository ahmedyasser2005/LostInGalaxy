#include "pch.h"
#include "Light.hpp"

DirectX::XMMATRIX LightSource::GetViewMatrix() noexcept(!_DEBUG)
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

DirectX::XMMATRIX LightSource::GetProjectionMatrix() noexcept(!_DEBUG)
{
	using namespace DirectX;

	if( !m_isProjDirty ) return XMLoadFloat4x4( &m_projMatrixStorage );

	XMMATRIX projMatrix = XMMatrixOrthographicLH( m_width,
												  m_height,
												  m_nearZ,
												  m_farZ );

	XMStoreFloat4x4( &m_projMatrixStorage, projMatrix );

	m_isProjDirty = false;

	return projMatrix;
}