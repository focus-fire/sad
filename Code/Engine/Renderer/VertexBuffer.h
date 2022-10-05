#pragma once

namespace sad::rad
{
	class GLBuffer
	{
	public:
		virtual ~GLBuffer() { }

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

	protected:
		unsigned int m_RendererId;
	};

	class VertexBuffer final : public GLBuffer
	{
	public:
		explicit VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer() override;

		void Bind() const override;
		void Unbind() const override;
	};

	class IndexBuffer final : public GLBuffer
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

	class FrameBuffer final : public GLBuffer 
	{
	public:
		explicit FrameBuffer(unsigned int width, unsigned int height);
		~FrameBuffer() override;

		void Bind() const override;
		void Unbind() const override;
	};

	class RenderBuffer final : public GLBuffer
	{
	public:
		explicit RenderBuffer(unsigned int width, unsigned int height);
		~RenderBuffer() override;

		void AttachToFrameBuffer();
		
		void Bind() const override;
		void Unbind() const override;
	};
}
