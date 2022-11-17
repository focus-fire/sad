#pragma once

#include "IGLBuffer.h"

namespace sad::rad
{

	class VertexBuffer final : public IGLBuffer
	{
	public:
		explicit VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer() override;

		void Bind() const override;
		void Unbind() const override;
	};
}
