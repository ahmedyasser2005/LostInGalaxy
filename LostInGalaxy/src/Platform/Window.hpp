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

	[[nodiscard]] std::optional<int> ProcessMessages() noexcept;

	[[nodiscard]] HWND GetHandle() const noexcept { return m_hwnd; }
	[[nodiscard]] UINT GetWidth() const noexcept { return m_width; }
	[[nodiscard]] UINT GetHeight() const noexcept { return m_height; }
	[[nodiscard]] Input& GetInput() noexcept { return m_input; }
	// NOTE: I added an overloaded const function to fix a compilation error
	// that occurred when I tried to pass the window by const reference
	[[nodiscard]] const Input& GetInput() const noexcept { return m_input; }

private:
	static LRESULT CALLBACK HandleMsgSetup( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;
	static LRESULT CALLBACK HandleMsgThunk( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;
	LRESULT HandleMsg( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;

private:
	HWND m_hwnd;
	UINT m_width, m_height;
	Input m_input;

};
