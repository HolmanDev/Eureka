#pragma once

#include "engine/renderer/Shader.h"

class AssetManager
{
private:
	static std::map<std::string, std::shared_ptr<Shader>> s_Shaders;
public:
	static void Close();

	static std::shared_ptr<Shader> GetShader(std::string shaderName);
	static void AddShader(const std::shared_ptr<Shader>& shader, const std::string name);
};