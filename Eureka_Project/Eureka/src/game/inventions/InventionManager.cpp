#include "pch.h"
#include "InventionManager.h"

packedVector<std::shared_ptr<Invention>> InventionManager::m_Inventions;

unsigned int InventionManager::AddInvention(std::shared_ptr<Invention> invention)
{
	return m_Inventions.Add(invention);
}

std::shared_ptr<Invention> InventionManager::GetInvention(int key)
{
	return m_Inventions.GetElements()[key];
}

void InventionManager::Close()
{
	m_Inventions.Clear();
}
