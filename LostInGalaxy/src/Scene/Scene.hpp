#pragma once

#include "Scene/Object.hpp"
#include "Scene/Camera.hpp"

class Scene {
public:
	uint8_t AddCamera( Camera* camera ) noexcept;
	uint8_t AddObject( Object* object ) noexcept;
	uint8_t ToggleCamera() noexcept;

	void SetActiveCameraIndex( uint8_t index ) noexcept;

	[[nodiscard]] const std::vector<Object*>& GetObjects() const noexcept;
	[[nodiscard]] const std::vector<Camera*>& GetCameras() const noexcept;

	[[nodiscard]] Camera* GetActiveCamera() const noexcept;
	[[nodiscard]] uint8_t GetActiveCameraIndex() const noexcept;
	[[nodiscard]] Camera* GetCameraAt( uint8_t index ) const noexcept;


private:
	std::vector<Object*> m_objects;
	std::vector<Camera*> m_cameras;
	uint8_t m_activeCameraIndex = 0;
};
