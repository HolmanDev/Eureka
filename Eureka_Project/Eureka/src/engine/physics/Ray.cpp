#include "pch.h"
#include "Ray.h"

#include "engine/renderer/Geometry.h"
#include "engine/world/World.h"
#include "CollisionManager.h"

Ray::Ray(glm::vec3 origin, glm::vec3 direction, float distance)
	: m_Origin(origin), m_Distance(distance)
{
	m_Direction = glm::normalize(direction);
}

Ray::~Ray()
{
}

const RayHit Ray::Cast(World* world) const
{
	RayHit hit;
	float closest = std::numeric_limits<float>::max();
	for (std::shared_ptr<GameObject> gameObject : world->GetGameObjects())
	{
		std::vector<std::shared_ptr<MeshCollider>> meshColliders = gameObject->GetAttributes<MeshCollider>();
		for (std::shared_ptr<MeshCollider> meshCollider : meshColliders) 
		{
			RayHit newHit = meshCollider->LineCollision(m_Origin, m_Origin + m_Direction * m_Distance);
			if (newHit.collided)
			{
				if (newHit.distance < closest)
				{
					closest = newHit.distance;
					hit = newHit;
				}
			}
		}
	}
	return hit;
}
