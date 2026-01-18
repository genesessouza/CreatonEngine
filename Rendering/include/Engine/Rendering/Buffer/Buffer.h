#pragma once

#include <cstdint>

#include <memory>

namespace Engine::Rendering::Buffer
{
	class VertexBuffer
	{
	public:
		static std::shared_ptr<VertexBuffer> Create(const void* data, size_t size);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	};

	class IndexBuffer
	{
	public:
		static std::shared_ptr<IndexBuffer> Create(const void* data, size_t size);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual size_t GetCount() = 0;
	};
}