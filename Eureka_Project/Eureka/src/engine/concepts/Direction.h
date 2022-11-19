#pragma once

enum class Direction { Right, Left, Up, Down, Forward, Backward };

struct DirectionHandler
{
	static glm::vec3 ToVec3(Direction direction);
	static Direction ToDirection(int n);
};