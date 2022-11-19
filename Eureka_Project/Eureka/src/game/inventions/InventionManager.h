#pragma once

#include "engine/concepts/PackedVector.h"
#include "Invention.h"

class InventionManager
{
private:
	static packedVector<std::shared_ptr<Invention>> m_Inventions; // Generate lowest value key that isn't in map.keys
public:
	static unsigned int AddInvention(std::shared_ptr<Invention> invention); // Add invention to map and generate key, which is returned
	static std::shared_ptr<Invention> GetInvention(int key);

	static void Close();
};