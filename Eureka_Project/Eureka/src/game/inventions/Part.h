#pragma once

#include "engine/physics/Collider.h"
#include "game/inventions/PartSpecification.h"
#include "engine/renderer/Geometry.h"

class Player;
class Invention;

class Part 
{
private:
	const PartSpecification m_Specification;
	int m_InventionKey;
	glm::vec3 m_Pos;
	glm::vec3 m_Color;
	std::shared_ptr<Collider> m_HitBox;
	bool m_Configuring;
	int m_Size;
public:
	Part();
	Part(const PartSpecification& specification);
	~Part();

	std::string GetPartType() const { return m_Specification.Name; }
	virtual std::string GetPath() = 0;
	virtual float* GetUVs();
	virtual void Action(Invention* invention) = 0;
	virtual void OnEnvironmentChange(Invention* invention);
	void UpdateSurrounding(Invention* invention);
	virtual void OnPlace(Invention* invention);
	virtual void OnRemove(Invention* invention);
	virtual void Interact(Player* player, Invention* invention); //! Fishy..
	virtual void Configure(Player* player, Invention* invention);
	virtual void StopConfiguring(Player* player, Invention* invention);
	bool IsInteractable() const { return m_Specification.Interactable; }
	bool IsConfigurable() const { return m_Specification.Configurable; }
	bool IsBeingConfigured() const { return m_Configuring; }

	const int GetInventionKey() const { return m_InventionKey; }
	std::shared_ptr<Invention> GetInvention() const;
	const glm::vec3 GetPosition() const { return m_Pos; }
	const glm::vec3 GetColor() const { return m_Color; }
	std::shared_ptr<Collider> GetHitBox() const { return m_HitBox; }
	std::vector<Part*> GetConnectingParts(Invention* invention);
	const int GetSize() const { return m_Size; }

	void SetInventionKey(int key);
	void SetPosition(glm::vec3 position);
	void SetColor(glm::vec3 color);
	void SetHitBox(std::shared_ptr<Collider> hitbox);
	void SetSize(int size);

	static const PartSpecification GenerateSpecification(const std::string& path);
	static const std::map<std::string, unsigned int> partTypes;
};