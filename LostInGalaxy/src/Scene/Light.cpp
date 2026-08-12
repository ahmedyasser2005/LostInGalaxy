#include "pch.h"
#include "Light.hpp"

DirectX::XMMATRIX LightSource::GetViewMatrix( DirectX::XMVECTOR focusPosition ) noexcept(!_DEBUG)
{
	using namespace DirectX;

	if( !transform.IsDirty() ) return XMLoadFloat4x4( &m_viewMatrixStorage );

	XMMATRIX viewMatrix = XMMatrixLookAtLH( transform.XYZ(), focusPosition, { 0.0f, 1.0f, 0.0f, 0.0f } );

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