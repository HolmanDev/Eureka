#include "pch.h"
#include "pStructure.h"

#include "engine/core/Time.h"
#include "engine/world/GameObject.h"
#include "game/inventions/invention.h"

pStructure::pStructure() : Part(Part::GenerateSpecification(GetPath()))
{
	std::ifstream f(GetPath());
	json data = json::parse(f);
	m_FaceTex = data["face"];
}

pStructure::~pStructure()
{
}

std::string pStructure::GetPath()
{
	return "res/bits/structure.json";
}

float* pStructure::GetUVs()
{
	float scale = 1.0f / 21.0f;
	static float UVs[2 * 24]{};
	int faceNum = m_FaceTex;
	float x;
	float y;
	for (int i = 0; i < 2 * 24; i += 2)
	{
		x = (float) (faceNum % 21);
		y = (float) (faceNum / 21);
		UVs[i] = (Geometry::BoxUVs[i] + x) * scale;
		UVs[i + 1] = 1.0f - (1.0f - Geometry::BoxUVs[i + 1] + y) * scale;
	}
	return UVs;
}

// Performs an action
void pStructure::Action(Invention* invention) //! hmm
{
}