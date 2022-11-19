#include "pch.h"
#include "pLight.h"

#include "game/inventions/invention.h"

pLight::pLight() : Part(Part::GenerateSpecification(GetPath()))
{
	std::ifstream f(GetPath());
	json data = json::parse(f);
	m_Face = data["face"];
	m_PoweredFace = data["poweredFace"];
}

pLight::~pLight()
{
}

std::string pLight::GetPath()
{
	return "res/bits/light.json";
}

float* pLight::GetUVs()
{
	float scale = 1.0f / 21.0f;
	static float UVs[2 * 24]{};
	int faceNum = m_Face;
	if (IsPowered())
		faceNum = m_PoweredFace;
	float x;
	float y;
	for (int i = 0; i < 2 * 24; i += 2)
	{
		x = (float)(faceNum % 21);
		y = (float)(faceNum / 21);
		UVs[i] = (Geometry::BoxUVs[i] + x) * scale;
		UVs[i + 1] = 1.0f - (1.0f - Geometry::BoxUVs[i + 1] + y) * scale;
	}
	return UVs;
}

// Performs an action
void pLight::Action(Invention* invention) //! hmm
{
}

void pLight::OnEnvironmentChange(Invention* invention)
{
	UpdatePower(this, invention); // Find all blocks that are directly powered
	invention->GenerateMesh();
	invention->ApplyMesh();
}
