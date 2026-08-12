#pragma once

#include "pch.h"
#include "Platform/Input.hpp"

// Thanks to ChiliTomatoNoodle, this implementation is derived from his implementation

class Window final {
public:
	Window( const wchar_t* title, UINT width, UINT height );
	~Window();
	Window( const Window& ) = delete;
	Window& operator=( const Window& ) = delete;
	Window( Window&& ) = delete;
	Window& operator=( Window&& ) = delete;

	[[nodiscard]] std::optional<int> ProcessMessages() noexcept(!_DEBUG);

	[[nodiscard]] HWND GetHandle() const noexcept(!_DEBUG) { return m_hwnd; }
	[[nodiscard]] UINT GetWidth() const noexcept(!_DEBUG) { return m_width; }
	[[nodiscard]] UINT GetHeight() const noexcept(!_DEBUG) { return m_height; }
	[[nodiscard]] Input& GetInput() noexcept(!_DEBUG) { return m_input; }
	// NOTE: I added an overloaded const function to fix a compilation error
	// that occurred when I tried to pass the window by const reference
	[[nodiscard]] const Input& GetInput() const noexcept(!_DEBUG) { return m_input; }

private:
	static LRESULT CALLBACK HandleMsgSetup( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept(!_DEBUG);
	static LRESULT CALLBACK HandleMsgThunk( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept(!_DEBUG);
	LRESULT HandleMsg( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept(!_DEBUG);

private:
	HWND m_hwnd;
	UINT m_width, m_height;
	Input m_input;

};
