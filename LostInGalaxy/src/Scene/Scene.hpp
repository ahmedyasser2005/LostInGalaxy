#pragma once

#include "Scene/Object.hpp"
#include "Scene/Camera.hpp"

class Scene {
public:
	// Object
	uint32_t AddObject( Object& object ) noexcept;
	uint32_t ToggleObject() noexcept;
	bool SetActiveObjectIndex( uint32_t index ) noexcept;
	[[nodiscard]] std::vector<Object>& GetObjects() noexcept;
	[[nodiscard]] std::optional<uint32_t> GetActiveObjectIndex() const noexcept;
	[[nodiscard]] Object* GetActiveObject() noexcept;
	[[nodiscard]] Object* GetObjectAt( uint32_t index ) noexcept;

	// Camera
	uint8_t AddCamera( Camera& camera ) noexcept;
	uint8_t ToggleCamera() noexcept;
	bool SetActiveCameraIndex( uint8_t index ) noexcept;
	[[nodiscard]] std::vector<Camera>& GetCameras() noexcept;
	[[nodiscard]] std::optional<uint8_t> GetActiveCameraIndex() const noexcept;
	[[nodiscard]] Camera* GetActiveCamera() noexcept;
	[[nodiscard]] Camera* GetCameraAt( uint8_t index ) noexcept;

private:
	// Object
	std::vector<Object> m_objects;
	uint32_t m_activeObjectIndex = 0;

	// Camera
	std::vector<Camera> m_cameras;
	uint8_t m_activeCameraIndex = 0;

};
