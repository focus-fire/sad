#pragma once

#include "IGLBuffer.h"

namespace sad::rad
{
	class FrameBuffer final : public IGLBuffer 
	{
	public:
		explicit FrameBuffer(unsigned int width, unsigned int height);
		~FrameBuffer() override;

		void Bind() const override;
		void Unbind() const override;
	};
}
