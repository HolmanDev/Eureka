#pragma once

#include "engine/concepts/Attribute.h"
#include "engine/events/EventSubscriber.h"
#include "engine/attributes/Mesh.h"
#include "Part.h"
#include "engine/physics/collider.h"
#include "engine/concepts/signedVector.h"
#include "engine/concepts/Grid3D.h"

class Player;

class Invention : public Attribute, public EventSubscriber
{
private:
	int m_Key;
	std::vector<std::shared_ptr<Part>> m_Parts; //! Make shared_ptr
	Grid3D<Part*> m_PartSpace;
	std::shared_ptr<Mesh> m_Mesh;
	std::vector<std::shared_ptr<MeshCollider>> m_Colliders;
	Player* m_Player;
public:
	Invention();
	~Invention();

	std::string Identify() override;

	void Update() override;
	
	const int GetKey() const { return m_Key; }
	Player* GetPlayer() const { return m_Player; }

	void SetKey(int key);
	void SetPlayer(Player* player);

	void AddPart(std::shared_ptr<Part> part);
	void AddToPartList(std::shared_ptr<Part> part);
	void RemovePart(std::shared_ptr<Part> part_ptr);
	std::vector<std::shared_ptr<Part>> GetParts() const { return m_Parts; }
	std::shared_ptr<Part> GetPartWithCollider(Collider* collider);
	Grid3D<Part*>& GetPartSpace() { return m_PartSpace; }
	Part* SamplePartSpace(Grid3Pos position);

	void Run();
	std::shared_ptr<Mesh> GenerateMesh();
	std::vector<std::shared_ptr<MeshCollider>> GenerateColliders();
	void ApplyMesh();
	void ApplyColliders();
};