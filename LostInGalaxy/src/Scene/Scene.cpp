#include "pch.h"
#include "Scene.hpp"

uint32_t Scene::AddObject( Object& object ) noexcept
{
	m_objects.emplace_back( std::move( object ) );
	return static_cast<uint32_t>(m_objects.size() - 1);
}

uint32_t Scene::ToggleObject() noexcept
{
	m_activeObjectIndex = (m_activeObjectIndex + 1) % (uint32_t)m_objects.size();
	return m_activeObjectIndex;
}

bool Scene::SetActiveObjectIndex( uint32_t index ) noexcept
{
	if( index < 0 || index >= m_objects.size() )
		return false;

	m_activeObjectIndex = index;
	return true;
}

std::vector<Object>& Scene::GetObjects() noexcept
{
	return m_objects;
}

std::optional<uint32_t> Scene::GetActiveObjectIndex() const noexcept
{
	if( m_objects.empty() )
		return std::nullopt;

	return m_activeObjectIndex;
}

Object* Scene::GetActiveObject() noexcept
{
	if( m_objects.empty() )
		return nullptr;

	return &m_objects[m_activeObjectIndex];
}

Object* Scene::GetObjectAt( uint32_t index ) noexcept
{
	if( index < 0 || index >= m_objects.size() )
		return nullptr;

	return &m_objects[index];
}

uint8_t Scene::AddCamera( Camera& camera ) noexcept
{
	m_cameras.emplace_back( std::move( camera ) );
	return static_cast<uint8_t>(m_cameras.size() - 1);
}

uint8_t Scene::ToggleCamera() noexcept
{
	m_activeCameraIndex = (m_activeCameraIndex + 1) % (uint8_t)m_cameras.size();
	return m_activeCameraIndex;
}

bool Scene::SetActiveCameraIndex( uint8_t index ) noexcept
{
	if( index < 0 || index >= m_cameras.size() )
		return false;

	m_activeCameraIndex = index;
	return true;
}

std::vector<Camera>& Scene::GetCameras() noexcept
{
	return m_cameras;
}

std::optional<uint8_t> Scene::GetActiveCameraIndex() const noexcept
{
	if( m_cameras.empty() )
		return std::nullopt;

	return m_activeCameraIndex;
}

Camera* Scene::GetActiveCamera() noexcept
{
	if( m_cameras.empty() )
		return nullptr;

	return &m_cameras[m_activeCameraIndex];
}

Camera* Scene::GetCameraAt( uint8_t index ) noexcept
{
	if( index < 0 || index >= m_cameras.size() )
		return nullptr;

	return &m_cameras[index];
}
