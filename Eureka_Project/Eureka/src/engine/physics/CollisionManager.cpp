#include "pch.h"
#include "CollisionManager.h"

bool CollisionManager::GJK(const Collider* colliderA, const Collider* colliderB)
{
	glm::vec3 direction = glm::normalize(colliderB->center - colliderA->center);
	glm::vec3 support = Support(colliderA, colliderB, direction); // First point
	Simplex points;
	points.push_front(support); // Add the first point
	direction = -support; // Direction from first point to origin
	
	while (true)
	{
		support = Support(colliderA, colliderB, direction);

		if (glm::dot(support, direction) <= 0)
			return false;

		points.push_front(support);

		if (NextSimplex(points, direction)) // Returns true when tetrahedron checking is done
			return true;
	}
}

glm::vec3 CollisionManager::Support(const Collider* colliderA, const Collider* colliderB, glm::vec3 direction)
{
	// max(D • (A-B)) = max(D • A) - max(D • B)
	return colliderA->FindFurthestPoint(direction) - colliderB->FindFurthestPoint(-direction);
}

bool CollisionManager::NextSimplex(Simplex& points, glm::vec3& direction)
{
	switch (points.size())
	{
		case 2: return Line			(points, direction);
		case 3: return Triangle		(points, direction);
		case 4: return Tetrahedron	(points, direction);
	}

	return false;
}

bool CollisionManager::SameDirection(const glm::vec3& direction, const glm::vec3& AtoO)
{
	return glm::dot(direction, AtoO) > 0;
}

bool CollisionManager::Line(Simplex& points, glm::vec3& direction)
{
	glm::vec3 a = points[0];
	glm::vec3 b = points[1];

	glm::vec3 ab = b - a;
	glm::vec3 ao = -a;

	if (SameDirection(ab, ao))
	{
		// Set search direction pointing perpendicular to the line, toward the origin
		direction = glm::cross(glm::cross(ab, ao), ab);
	}
	else
	{
		// Tries with a line again but without b and in a new direction (toward the origin from a)
		points = { a };
		direction = ao;
	}

	return false;
}

bool CollisionManager::Triangle(Simplex& points, glm::vec3& direction)
{
	glm::vec3 a = points[0];
	glm::vec3 b = points[1];
	glm::vec3 c = points[2];

	glm::vec3 ab = b - a;
	glm::vec3 ac = c - a;
	glm::vec3 ao = -a;

	glm::vec3 abc = glm::cross(ab, ac); // face normal

	if (SameDirection(glm::cross(abc, ac), ao)) // Is the origin outside the triangle on the ac side?
	{
		if (SameDirection(ac, ao))
		{
			// Discard b
			points = { a, c };
			direction = glm::cross(glm::cross(ac, ao), ac); // Away from previous B, toward the origin
		}
		else
		{
			// Discard c
			return Line(points = { a, b }, direction);
		}
	}
	else
	{
		if (SameDirection(glm::cross(ab, abc), ao))
		{
			// Discard c
			return Line(points = { a, b }, direction);
		}
		else
		{
			if (SameDirection(abc, ao))
			{
				// above
				direction = abc;
			}
			else
			{
				// below
				points = { a, c, b };
				direction = -abc;
			}
		}
	}

	return false;
}

bool CollisionManager::Tetrahedron(Simplex& points, glm::vec3& direction)
{
	glm::vec3 a = points[0];
	glm::vec3 b = points[1];
	glm::vec3 c = points[2];
	glm::vec3 d = points[3];

	glm::vec3 ab = b - a;
	glm::vec3 ac = c - a;
	glm::vec3 ad = d - a;
	glm::vec3 ao = -a;

	glm::vec3 abc = glm::cross(ab, ac);
	glm::vec3 acd = glm::cross(ac, ad);
	glm::vec3 adb = glm::cross(ad, ab);

	if (SameDirection(abc, ao))
	{
		return Triangle(points = { a, b, c }, direction);
	}

	if (SameDirection(acd, ao))
	{
		return Triangle(points = { a, c, d }, direction);
	}

	if (SameDirection(adb, ao))
	{
		return Triangle(points = { a, d, b }, direction);
	}

	return true;
}