#include "pch.h"
#include "Timer.hpp"

float Timer::Mark() noexcept
{
	const auto oldTime = m_lastTime;
	m_lastTime = std::chrono::steady_clock::now();
	const std::chrono::duration<float> frameTime = m_lastTime - oldTime;
	return frameTime.count();
}

float Timer::Peek() const noexcept
{
	const std::chrono::duration<float> elapsedTime = std::chrono::steady_clock::now() - m_lastTime;
	return elapsedTime.count();
}
