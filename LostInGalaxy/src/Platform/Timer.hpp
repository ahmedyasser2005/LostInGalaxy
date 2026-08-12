#pragma once

#include "pch.h"

class Timer final {
public:
	// Resets the timer and returns elapsed time since the last Mark() call
	[[nodiscard]] float Mark() noexcept(!_DEBUG);
	// Returns elapsed time since the last Mark() call WITHOUT resetting the timer
	[[nodiscard]] float Peek() const noexcept(!_DEBUG);

private:
	std::chrono::steady_clock::time_point m_lastTime = std::chrono::steady_clock::now();
};
