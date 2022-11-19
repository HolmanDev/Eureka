#include "pch.h"
#include "Direction.h"

glm::vec3 DirectionHandler::ToVec3(Direction direction)
{
	switch (direction)
	{
	case Direction::Right:		return glm::vec3( 1,  0,  0); break;
	case Direction::Left:		return glm::vec3(-1,  0,  0); break;
	case Direction::Up:			return glm::vec3( 0,  1,  0); break;
	case Direction::Down:		return glm::vec3( 0, -1,  0); break;
	case Direction::Forward:	return glm::vec3( 0,  0,  1); break;
	case Direction::Backward:	return glm::vec3( 0,  0, -1); break;
	}
	assert(false);
	return glm::vec3(0);
}

Direction DirectionHandler::ToDirection(int n)
{
	switch (n)
	{
	case 0:	return Direction::Right; break;
	case 1:	return Direction::Left; break;
	case 2:	return Direction::Up; break;
	case 3:	return Direction::Down; break;
	case 4:	return Direction::Forward; break;
	case 5:	return Direction::Backward; break;
	}
	assert(false);
	return Direction::Right;
}
