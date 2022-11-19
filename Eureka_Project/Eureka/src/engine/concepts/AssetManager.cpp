#include "pch.h"
#include "AssetManager.h"

std::map<std::string, std::shared_ptr<Shader>> AssetManager::s_Shaders;

void AssetManager::Close()
{
	s_Shaders.clear();
}

std::shared_ptr<Shader> AssetManager::GetShader(std::string shaderName)
{
	return s_Shaders[shaderName];
}

void AssetManager::AddShader(const std::shared_ptr<Shader>& shader, const std::string name)
{
	s_Shaders[name] = shader;
}
