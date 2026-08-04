#pragma once

#include "Graphics/GraphicsDevice.hpp"

class IBind {
public:
	IBind( GraphicsDevice* graphicsDevice ) : m_device( graphicsDevice )
	{
		assert( graphicsDevice );
	}

	virtual ~IBind() noexcept = default;
	virtual void Bind() const noexcept = 0;

protected:
	GraphicsDevice* m_device;
};