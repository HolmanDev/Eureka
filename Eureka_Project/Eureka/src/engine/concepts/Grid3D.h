#pragma once
#include "pch.h"

#include "SignedVector.h"

struct Grid3Pos
{
	int x;
	int y;
	int z;

	Grid3Pos(int _x, int _y, int _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	Grid3Pos()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	static Grid3Pos FromVec3(glm::vec3 vec)
	{
		return { (int)round(vec.x), (int)round(vec.y), (int)round(vec.z) };
	}

	bool operator==(const Grid3Pos& rhs)
	{
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}

	bool operator!=(const Grid3Pos& rhs)
	{
		return x != rhs.x || y != rhs.y || z != rhs.z;
	}

	Grid3Pos operator+(const Grid3Pos& first) const
	{
		return Grid3Pos(first.x + x, first.y + y, first.z + z);
	}

	Grid3Pos operator+=(const Grid3Pos& first)
	{
		x += first.x;
		y += first.y;
		z += first.z;
		return Grid3Pos(x, y, z);
	}
};

template<class T>
struct Grid3D
{
private:
	signedVector<signedVector<signedVector<T>>> m_Grid;
public:
	Grid3D();
	~Grid3D();

	bool InRange(int x, int y, int z);
	bool InRange(Grid3Pos position);
	T& Set(const T& element, int x, int y, int z);
	T& Set(const T& element, Grid3Pos position);
	T& Access(int x, int y, int z);
	T& Access(Grid3Pos position);
};

template<class T>
inline Grid3D<T>::Grid3D()
{
}

template<class T>
inline Grid3D<T>::~Grid3D()
{
}

template<class T>
inline bool Grid3D<T>::InRange(int x, int y, int z)
{
	if (!m_Grid.InRange(x)) return false;
	if (!m_Grid[x].InRange(y)) return false;
	if (!m_Grid[x][y].InRange(z)) return false;
	return true;
}

template<class T>
inline bool Grid3D<T>::InRange(Grid3Pos position)
{
	return InRange(position.x, position.y, position.z);
}

template<class T>
inline T& Grid3D<T>::Set(const T& element, int x, int y, int z)
{
	if (!m_Grid.InRange(x))
	{
		m_Grid.SetAt(signedVector<signedVector<T>>(), x);
	}
	if (!m_Grid[x].InRange(y))
	{
		m_Grid[x].SetAt(signedVector<T>(), y);
	}
	m_Grid[x][y].SetAt(element, z);
	return m_Grid[x][y][z];
}

template<class T>
inline T& Grid3D<T>::Set(const T& element, Grid3Pos position)
{
	return Set(element, position.x, position.y, position.z);
}

template<class T>
inline T& Grid3D<T>::Access(int x, int y, int z)
{
	return m_Grid[x][y][z];
}

template<class T>
inline T& Grid3D<T>::Access(Grid3Pos position)
{
	return Access(position.x, position.y, position.z);
}
