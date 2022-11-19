#include "pch.h"
#include "math.h"

namespace Math
{
	float TetrahedronVolume(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d)
	{
		return glm::abs(glm::dot(a - d, glm::cross(b - d, c - d))) / 6.0f;
	}

	float TetrahedronSignedVolume(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d)
	{
		return glm::dot(a - d, glm::cross(b - d, c - d)) / 6.0f;
	}

	bool LineIntersectTriangle(glm::vec3 start, glm::vec3 end, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
	{
		bool v1 = Math::TetrahedronSignedVolume(start, p1, p2, p3) > 0;
		bool v2 = Math::TetrahedronSignedVolume(end, p1, p2, p3) > 0;
		if (v1 != v2)
		{
			bool v3 = Math::TetrahedronSignedVolume(start, end, p1, p2) > 0;
			bool v4 = Math::TetrahedronSignedVolume(start, end, p2, p3) > 0;
			bool v5 = Math::TetrahedronSignedVolume(start, end, p3, p1) > 0;

			if ((v3 == v4) && (v4 == v5))
			{
				return true;
				//glm::vec3 normal = glm::cross(p2 - p1, p3 - p1);
				//float t = -glm::dot(start, normal - p1) / glm::dot(start, end - start)
			}
		}
		return false;
	}
}