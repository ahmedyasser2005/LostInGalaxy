#pragma once

#include "Platform/Window.hpp"
#include "Platform/Input.hpp"
#include "Platform/Timer.hpp"
#include "Graphics/Renderer.hpp"
#include "Scene/Object.hpp"
#include "Scene/Scene.hpp"

class Application final {
public:
	Application( const wchar_t* title, uint32_t width, uint32_t height );
	~Application() noexcept = default;
	Application( const Application& ) = delete;
	Application& operator=( const Application& ) = delete;
	Application( Application&& ) = delete;
	Application& operator=( Application&& ) = delete;

	const int Run();
	void Exit() noexcept;
	void Update() noexcept;
	void Render() noexcept;

private:
	// Members
	std::unique_ptr<Window> m_window;
	std::unique_ptr<Renderer> m_renderer;
	Timer m_timer;
	Input& m_input;

	// Objects
	std::unique_ptr<Object> m_cube;
	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<Scene> m_scene;

	std::shared_ptr<Mesh> m_blockMesh;
	std::shared_ptr<Material> m_blockMaterial;

	// States
	float m_deltaTime = 0.0f;
	bool m_isRunning = true;
	std::optional<int> m_exitCode = {};

};
