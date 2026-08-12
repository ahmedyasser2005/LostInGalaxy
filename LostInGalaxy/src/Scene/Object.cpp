#include "pch.h"
#include "Object.hpp"

DirectX::XMMATRIX Object::GetWorldMatrix() noexcept(!_DEBUG)
{
	using namespace DirectX;

	if( !transform.IsDirty() ) return XMLoadFloat4x4( &m_worldMatrixStorage );

	XMMATRIX s = XMMatrixScaling( transform.Scale(), transform.Scale(), transform.Scale() );
	XMMATRIX r = XMMatrixRotationRollPitchYaw( transform.Pitch(), transform.Yaw(), transform.Roll() );
	XMMATRIX t = XMMatrixTranslation( transform.X(), transform.Y(), transform.Z() );

	XMMATRIX worldMatrix = XMMatrixMultiply( XMMatrixMultiply( s, r ), t );

	XMStoreFloat4x4( &m_worldMatrixStorage, worldMatrix );
	transform.CleanDirt();

	return worldMatrix;
}
