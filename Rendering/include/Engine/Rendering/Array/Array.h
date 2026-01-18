#pragma once

#include "Engine/Rendering/Buffer/Buffer.h"
#include "Engine/Rendering/Buffer/VertexBufferLayout.h"

#include <memory>
#include <vector>

namespace Engine::Rendering::Array
{
	class VertexArray
	{
	public:
		static std::shared_ptr<VertexArray> Create();

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const std::shared_ptr<Engine::Rendering::Buffer::VertexBuffer>& vertexBuffer, const Engine::Rendering::Buffer::VertexBufferLayout& layout) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<Engine::Rendering::Buffer::IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<std::shared_ptr<Engine::Rendering::Buffer::VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const std::shared_ptr<Engine::Rendering::Buffer::IndexBuffer>& GetIndexBuffer() const = 0;
	};
}