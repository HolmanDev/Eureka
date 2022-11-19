#pragma once

#include "game/inventions/Part.h"
#include "engine/concepts/Grid3D.h"

class pChunk : public Part
{
private:
	std::vector<Part*> m_Parts;
	Grid3D<Part*> m_PartSpace; // Partspace to be indexed.
	Part* m_SelectedPart;
	Grid3Pos m_SelectedPosition;
	int m_FaceTex;
public:
	pChunk();
	~pChunk();

	std::string GetPath() override;
	float* GetUVs() override;
	void AddPart(Part* part);
	void RemovePart(Part* part);
	void TryPlacePart(const std::string partType);
	void Clear();
	const Part* GetPart() const; //?
	const std::vector<Part*> GetParts() const { return m_Parts; }
	const Part* GetSelectedPart() const { return m_SelectedPart; };
	const Grid3Pos GetSelectedPosition() const { return m_SelectedPosition; }

	void SetSelectedPart(Part* part); // This should maybe not be public
	void SetSelectedPosition(Grid3Pos pos);

	void Action(Invention* invention) override;
	void Configure(Player* player, Invention* invention) override;
};