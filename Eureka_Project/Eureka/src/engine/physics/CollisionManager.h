#pragma once
#include "Collider.h"

#include "Simplex.h"

class CollisionManager
{
public:
	static bool GJK(const Collider* colliderA, const Collider* colliderB);
private:
	static glm::vec3 Support(const Collider* colliderA, const Collider* colliderB, glm::vec3 direction);
	static bool NextSimplex(Simplex& points, glm::vec3& direction);
	static bool SameDirection(const glm::vec3& direction, const glm::vec3& AtoO);
	static bool Line(Simplex& points, glm::vec3& direction);
	static bool Triangle(Simplex& points, glm::vec3& direction);
	static bool Tetrahedron(Simplex& points, glm::vec3& direction);
};