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
	~Application() noexcept;
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
	std::unique_ptr<Scene> m_scene;
	Timer m_timer;
	Input& m_input;

	// Objects
	Camera* m_activeCamera;
	Object* m_activeObject;
	LightSource* m_activeLight;

	// Geometry
	std::shared_ptr<Mesh> m_blockMesh;
	std::shared_ptr<Mesh> m_sphereMesh;

	// Stuff
	std::shared_ptr<Material> m_material;
	std::vector<ShaderFilename> m_availableShaders;

	// States
	float m_deltaTime = 0.0f;
	bool m_isRunning = true;
	bool m_isMenuActive = true;
	bool m_rotateMode = true;

	std::optional<int> m_exitCode = {};

};
