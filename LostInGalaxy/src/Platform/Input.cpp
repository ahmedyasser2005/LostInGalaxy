#include "pch.h"
#include "Input.hpp"

[[nodiscard]] bool Input::IsKeyPressed( uint8_t key ) const noexcept
{
	return m_keys[key];
}

[[nodiscard]] bool Input::IsKeyTriggered( uint8_t key ) const noexcept
{
	return m_keys[key] && !m_prevKeys[key];
}

[[nodiscard]] bool Input::IsMouseDown( MouseButton button ) const noexcept
{
	return m_mouseButtons[button];
}

[[nodiscard]] bool Input::IsMouseTriggered( MouseButton button ) const noexcept
{
	return m_mouseButtons[button] && !m_prevMouseButtons[button];
}

void Input::Reset() noexcept
{
	m_prevKeys = m_keys;
	m_prevMouseButtons = m_mouseButtons;
	m_mouseDeltaX = 0;
	m_mouseDeltaY = 0;
	m_wheelDelta = 0;
}

void Input::OnKeyDown( uint8_t key ) noexcept
{
	m_keys[key] = true;
}

void Input::OnKeyUp( uint8_t key ) noexcept
{
	m_keys[key] = false;
}

void Input::OnMouseMove( int32_t x, int32_t y ) noexcept
{
	m_mouseDeltaX = x - m_mouseX;
	m_mouseDeltaY = y - m_mouseY;
	m_mouseX = x;
	m_mouseY = y;
}

void Input::OnMouseDown( MouseButton button ) noexcept
{
	m_mouseButtons.set( button );
}

void Input::OnMouseUp( MouseButton button ) noexcept
{
	m_mouseButtons.reset( button );
}

void Input::OnMouseWheel( int16_t delta ) noexcept
{
	m_wheelDelta += delta;
}
