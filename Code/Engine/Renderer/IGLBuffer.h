#pragma once

namespace sad::rad
{
	class IGLBuffer
	{
	public:
		virtual ~IGLBuffer() { }

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

	protected:
		unsigned int m_RendererId;
	};
}
