#include "pch.h"
#include "Scene.hpp"

// This implementation is a complete mess, and I will further refactor and improve it later

uint8_t Scene::AddCamera( Camera* camera ) noexcept
{
	// TODO: Prevent adding the existing cameras.

	if( camera ) m_cameras.emplace_back( camera );
	return (uint8_t)(m_cameras.size() - 1);

}

uint8_t Scene::AddObject( Object* object ) noexcept
{
	// TODO: Prevent adding the existing objects.

	if( object ) m_objects.emplace_back( object );
	return (uint8_t)(m_objects.size() - 1);

}

uint8_t Scene::ToggleCamera() noexcept
{
	return m_activeCameraIndex = (m_activeCameraIndex + 1) % (uint8_t)m_cameras.size();
}

void Scene::SetActiveCameraIndex( uint8_t index ) noexcept
{
	if( index >= 0 && index < m_cameras.size() )
	{
		m_activeCameraIndex = index;
	}
}

const std::vector<Object*>& Scene::GetObjects() const noexcept
{
	return m_objects;
}

const std::vector<Camera*>& Scene::GetCameras() const noexcept
{
	return m_cameras;
}

Camera* Scene::GetActiveCamera() const noexcept
{
	if( m_cameras.empty() ) return nullptr;
	return m_cameras[m_activeCameraIndex];
}

uint8_t Scene::GetActiveCameraIndex() const noexcept
{
	assert( !m_cameras.empty() );
	return m_activeCameraIndex;
}

Camera* Scene::GetCameraAt( uint8_t index ) const noexcept
{
	if( index >= 0 && index < m_cameras.size() )
	{
		return m_cameras[index];
	}
	return nullptr;
}
