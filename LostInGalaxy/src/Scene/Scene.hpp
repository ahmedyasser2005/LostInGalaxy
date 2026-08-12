#pragma once

#include "Scene/Object.hpp"
#include "Scene/Camera.hpp"
#include "Scene/Light.hpp"

// TODO: use templates

class Scene {
public:
	// Object
	uint32_t AddObject( Object& object ) noexcept(!_DEBUG);
	uint32_t ToggleObject() noexcept(!_DEBUG);
	bool SetActiveObjectIndex( uint32_t index ) noexcept(!_DEBUG);
	[[nodiscard]] std::vector<Object>& GetObjects() noexcept(!_DEBUG);
	[[nodiscard]] std::optional<uint32_t> GetActiveObjectIndex() const noexcept(!_DEBUG);
	[[nodiscard]] Object* GetActiveObject() noexcept(!_DEBUG);
	[[nodiscard]] Object* GetObjectAt( uint32_t index ) noexcept(!_DEBUG);

	// Object
	uint32_t AddLight( LightSource& light ) noexcept(!_DEBUG);
	uint32_t ToggleLight() noexcept(!_DEBUG);
	bool SetActiveLightIndex( uint32_t index ) noexcept(!_DEBUG);
	[[nodiscard]] std::vector<LightSource>& GetLights() noexcept(!_DEBUG);
	[[nodiscard]] std::optional<uint32_t> GetActiveLightIndex() const noexcept(!_DEBUG);
	[[nodiscard]] LightSource* GetActiveLight() noexcept(!_DEBUG);
	[[nodiscard]] LightSource* GetLightAt( uint32_t index ) noexcept(!_DEBUG);

	// Camera
	uint8_t AddCamera( Camera& camera ) noexcept(!_DEBUG);
	uint8_t ToggleCamera() noexcept(!_DEBUG);
	bool SetActiveCameraIndex( uint8_t index ) noexcept(!_DEBUG);
	[[nodiscard]] std::vector<Camera>& GetCameras() noexcept(!_DEBUG);
	[[nodiscard]] std::optional<uint8_t> GetActiveCameraIndex() const noexcept(!_DEBUG);
	[[nodiscard]] Camera* GetActiveCamera() noexcept(!_DEBUG);
	[[nodiscard]] Camera* GetCameraAt( uint8_t index ) noexcept(!_DEBUG);

private:
	// Object
	std::vector<Object> m_objects;
	uint32_t m_activeObjectIndex = 0;

	// LightSource
	std::vector<LightSource> m_lights;
	uint32_t m_activeLightIndex = 0;

	// Camera
	std::vector<Camera> m_cameras;
	uint8_t m_activeCameraIndex = 0;

};
