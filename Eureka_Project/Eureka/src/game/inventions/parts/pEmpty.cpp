#include "pch.h"
#include "pEmpty.h"

#include "game/inventions/invention.h"

pEmpty::pEmpty() : Part(GenerateSpecification(GetPath()))
{
	SetColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
}

pEmpty::~pEmpty()
{
}

std::string pEmpty::GetPath()
{
	return "res/bits/empty.json";
}

// Performs an action
void pEmpty::Action(Invention* invention) //! hmm
{
}