#pragma once

#include "pch.h"

enum MouseButton : uint8_t {
	Left = 0u,
	Right,
	Middle,
	Count, // Number of mouse buttons
};

class Input final {
public:
	[[nodiscard]] bool IsKeyPressed( uint8_t key ) const noexcept(!_DEBUG);
	[[nodiscard]] bool IsKeyTriggered( uint8_t key ) const noexcept(!_DEBUG);

	[[nodiscard]] bool IsMouseDown( MouseButton button ) const noexcept(!_DEBUG);
	[[nodiscard]] bool IsMouseTriggered( MouseButton button ) const noexcept(!_DEBUG);

	[[nodiscard]] int32_t GetMouseX() const noexcept(!_DEBUG) { return m_mouseX; }
	[[nodiscard]] int32_t GetMouseY() const noexcept(!_DEBUG) { return m_mouseY; }
	[[nodiscard]] int32_t GetMouseDeltaX() const noexcept(!_DEBUG) { return m_mouseDeltaX; }
	[[nodiscard]] int32_t GetMouseDeltaY() const noexcept(!_DEBUG) { return m_mouseDeltaY; }
	[[nodiscard]] int16_t GetWheelDelta() const noexcept(!_DEBUG) { return m_wheelDelta; }

	void Reset() noexcept(!_DEBUG);

private:
	void OnKeyDown( uint8_t key ) noexcept(!_DEBUG);
	void OnKeyUp( uint8_t key ) noexcept(!_DEBUG);
	void OnMouseMove( int32_t x, int32_t y ) noexcept(!_DEBUG);
	void OnMouseDown( MouseButton button ) noexcept(!_DEBUG);
	void OnMouseUp( MouseButton button ) noexcept(!_DEBUG);
	void OnMouseWheel( int16_t delta ) noexcept(!_DEBUG);

	friend class Window;

private:
	std::bitset<256> m_keys = {}, m_prevKeys = {};
	std::bitset<MouseButton::Count> m_mouseButtons = {}, m_prevMouseButtons = {};

	int32_t m_mouseX = 0, m_mouseY = 0, m_mouseDeltaX = 0, m_mouseDeltaY = 0;
	int16_t m_wheelDelta = 0;

};
