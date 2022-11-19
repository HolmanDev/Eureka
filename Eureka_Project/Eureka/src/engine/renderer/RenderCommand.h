#pragma once

class VertexArray;

class RenderCommand
{
public:
	static void Clear();
	static void Draw(std::shared_ptr<VertexArray> vertexArray);
};