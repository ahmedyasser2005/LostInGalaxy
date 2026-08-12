#pragma once

#include "Platform/Window.hpp"
#include "Platform/Input.hpp"
#include "Platform/Timer.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Texture.hpp"
#include "Scene/Object.hpp"
#include "Scene/Scene.hpp"
#include "AssetManager/AssetManager.hpp"

class Application final {
public:
	Application( const wchar_t* title, uint32_t width, uint32_t height );
	~Application() noexcept(!_DEBUG);
	Application( const Application& ) = delete;
	Application& operator=( const Application& ) = delete;
	Application( Application&& ) = delete;
	Application& operator=( Application&& ) = delete;

	const int Run();
	void Exit() noexcept(!_DEBUG);
	void Update() noexcept(!_DEBUG);
	void Render() noexcept(!_DEBUG);

private:
	// Members
	std::unique_ptr<Window> m_window;
	std::unique_ptr<Renderer> m_renderer;
	std::unique_ptr<AssetManager> m_assetManager;

	std::unique_ptr<Scene> m_scene;
	Timer m_timer;
	Input& m_input;

	// States
	Camera* m_activeCamera;
	Object* m_activeObject;
	LightSource* m_activeLight;

	float m_deltaTime = 0.0f;
	bool m_isRunning = true;
	bool m_isMenuActive = true;
	bool m_rotateMode = true;

	std::optional<int> m_exitCode = {};

};
