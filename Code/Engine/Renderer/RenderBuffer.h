#pragma once

#include "IGLBuffer.h"

namespace sad::rad
{
	class RenderBuffer final : public IGLBuffer
	{
	public:
		explicit RenderBuffer(unsigned int width, unsigned int height);
		~RenderBuffer() override;

		void AttachToFrameBuffer();
		
		void Bind() const override;
		void Unbind() const override;
	};
}
