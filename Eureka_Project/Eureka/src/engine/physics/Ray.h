#pragma once

struct Collider;
class World;

struct RayHit
{
	bool collided = false;
	float distance = 0;
	glm::vec3 position = glm::vec3(0);
	Collider* collider = nullptr;
};

class Ray
{
private:
	glm::vec3 m_Origin;
	glm::vec3 m_Direction;
	float m_Distance;
public:
	Ray(glm::vec3 origin, glm::vec3 direction, float distance);
	~Ray();

	const RayHit Cast(World* world) const;
};