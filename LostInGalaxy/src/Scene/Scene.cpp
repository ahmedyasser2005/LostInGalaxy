#include "pch.h"
#include "Scene.hpp"

uint32_t Scene::AddObject( Object& object ) noexcept(!_DEBUG)
{
	m_objects.emplace_back( std::move( object ) );
	return static_cast<uint32_t>(m_objects.size() - 1);
}

uint32_t Scene::ToggleObject() noexcept(!_DEBUG)
{
	m_activeObjectIndex = (m_activeObjectIndex + 1) % (uint32_t)m_objects.size();
	return m_activeObjectIndex;
}

bool Scene::SetActiveObjectIndex( uint32_t index ) noexcept(!_DEBUG)
{
	if( index < 0 || index >= m_objects.size() )
		return false;

	m_activeObjectIndex = index;
	return true;
}

std::vector<Object>& Scene::GetObjects() noexcept(!_DEBUG)
{
	return m_objects;
}

std::optional<uint32_t> Scene::GetActiveObjectIndex() const noexcept(!_DEBUG)
{
	if( m_objects.empty() )
		return std::nullopt;

	return m_activeObjectIndex;
}

Object* Scene::GetActiveObject() noexcept(!_DEBUG)
{
	if( m_objects.empty() )
		return nullptr;

	return &m_objects[m_activeObjectIndex];
}

Object* Scene::GetObjectAt( uint32_t index ) noexcept(!_DEBUG)
{
	if( index < 0 || index >= m_objects.size() )
		return nullptr;

	return &m_objects[index];
}

uint32_t Scene::AddLight( LightSource& light ) noexcept(!_DEBUG)
{
	m_lights.emplace_back( std::move( light ) );
	return static_cast<uint32_t>( m_lights.size() - 1 );
}

uint32_t Scene::ToggleLight() noexcept(!_DEBUG)
{
	m_activeLightIndex = (m_activeLightIndex + 1) % (uint32_t)m_lights.size();
	return m_activeLightIndex;
}

bool Scene::SetActiveLightIndex( uint32_t index ) noexcept(!_DEBUG)
{
	if( index < 0 || index >= m_lights.size() )
		return false;

	m_activeLightIndex = index;
	return true;
}

std::vector<LightSource>& Scene::GetLights() noexcept(!_DEBUG)
{
	return m_lights;
}

std::optional<uint32_t> Scene::GetActiveLightIndex() const noexcept(!_DEBUG)
{
	if( m_lights.empty() )
		return std::nullopt;

	return m_activeLightIndex;
}

LightSource* Scene::GetActiveLight() noexcept(!_DEBUG)
{
	if( m_lights.empty() )
		return nullptr;

	return &m_lights[m_activeLightIndex];
}

LightSource* Scene::GetLightAt( uint32_t index ) noexcept(!_DEBUG)
{
	if( index < 0 || index >= m_lights.size() )
		return nullptr;

	return &m_lights[index];
}


uint8_t Scene::AddCamera( Camera& camera ) noexcept(!_DEBUG)
{
	m_cameras.emplace_back( std::move( camera ) );
	return static_cast<uint8_t>( m_cameras.size() - 1 );
}

uint8_t Scene::ToggleCamera() noexcept(!_DEBUG)
{
	m_activeCameraIndex = (m_activeCameraIndex + 1) % (uint8_t)m_cameras.size();
	return m_activeCameraIndex;
}

bool Scene::SetActiveCameraIndex( uint8_t index ) noexcept(!_DEBUG)
{
	if( index < 0 || index >= m_cameras.size() )
		return false;

	m_activeCameraIndex = index;
	return true;
}

std::vector<Camera>& Scene::GetCameras() noexcept(!_DEBUG)
{
	return m_cameras;
}

std::optional<uint8_t> Scene::GetActiveCameraIndex() const noexcept(!_DEBUG)
{
	if( m_cameras.empty() )
		return std::nullopt;

	return m_activeCameraIndex;
}

Camera* Scene::GetActiveCamera() noexcept(!_DEBUG)
{
	if( m_cameras.empty() )
		return nullptr;

	return &m_cameras[m_activeCameraIndex];
}

Camera* Scene::GetCameraAt( uint8_t index ) noexcept(!_DEBUG)
{
	if( index < 0 || index >= m_cameras.size() )
		return nullptr;

	return &m_cameras[index];
}
