#pragma once

namespace Math
{
	float TetrahedronVolume(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d);
	float TetrahedronSignedVolume(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d);
	bool LineIntersectTriangle(glm::vec3 start, glm::vec3 end, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
}