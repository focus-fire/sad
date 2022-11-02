#pragma once

#include "IGLBuffer.h"

namespace sad::rad
{
	class IndexBuffer final : public IGLBuffer
	{
	public:
		explicit IndexBuffer(const void* data, unsigned int count);
		~IndexBuffer() override;

		void Bind() const override;
		void Unbind() const override;

		unsigned int GetIndexCount() const { return m_IndexCount; }

	private:
		unsigned int m_IndexCount;
	};

}
