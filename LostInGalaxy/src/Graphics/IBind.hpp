#pragma once

#include "Graphics/GraphicsDevice.hpp"

class IBind {
public:
	IBind( GraphicsDevice* graphicsDevice ) noexcept(!_DEBUG) :
		m_device( graphicsDevice )
	{
		assert( graphicsDevice );
	}
	virtual ~IBind() noexcept(!_DEBUG) = default;
	IBind( const IBind& ) = delete;
	IBind& operator=( const IBind& ) = delete;
	IBind( IBind&& ) noexcept(!_DEBUG) = default;
	IBind& operator=( IBind&& ) noexcept(!_DEBUG) = default;

	virtual void Bind() const noexcept(!_DEBUG) = 0;

protected:
	GraphicsDevice* m_device;
};
