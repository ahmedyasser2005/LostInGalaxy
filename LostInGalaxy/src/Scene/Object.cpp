#include "pch.h"
#include "Object.hpp"

DirectX::XMMATRIX Object::GetWorldMatrix() noexcept
{
	using namespace DirectX;

	if( !transform.IsDirty() ) return XMLoadFloat4x4( &m_worldMatrixStorage );

	XMMATRIX s = XMMatrixScaling( transform.S(), transform.S(), transform.S() );
	XMMATRIX r = XMMatrixRotationRollPitchYaw( transform.RX(), transform.RY(), transform.RZ() );
	XMMATRIX t = XMMatrixTranslation( transform.X(), transform.Y(), transform.Z() );

	XMMATRIX worldMatrix = XMMatrixMultiply( XMMatrixMultiply( s, r ), t );

	XMStoreFloat4x4( &m_worldMatrixStorage, worldMatrix );
	transform.CleanDirt();

	return worldMatrix;
}
