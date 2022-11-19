#pragma once
#include "pch.h"

//! Use set instead?
template<class T> 
struct packedVector
{
private:
	std::vector<T> m_Elements;
	unsigned int m_Length;
	std::queue<int> m_Gaps;

	unsigned int m_GetFreeIndex();
public:
	packedVector();
	~packedVector();

	const std::vector<T> GetElements();
	const unsigned int Length();

	unsigned int Add(T element);
	void RemoveAt(unsigned int index);
	void Clear();
};

template<class T>
packedVector<T>::packedVector()
	: m_Length(0)
{
}

template<class T>
packedVector<T>::~packedVector()
{
	m_Elements.clear(); //! Is this needed?
	//std::queue<int> empty;
	//std::swap(m_Gaps, empty);
}

template<class T>
unsigned int packedVector<T>::m_GetFreeIndex()
{
	unsigned int index = 0;
	if (m_Gaps.empty()) {
		index = m_Length;
	}
	else {
		index = m_Gaps.front();
		m_Gaps.pop();
	}
	return index;
}

template<class T>
const std::vector<T> packedVector<T>::GetElements()
{
	std::vector<T> elements;
	for (T newElement : m_Elements)
	{
		if (newElement != NULL)
			elements.push_back(newElement);
	}
	return elements;
}

template<class T>
const unsigned int packedVector<T>::Length()
{
	return m_Length;
}


template<class T>
unsigned int packedVector<T>::Add(T element)
{
	unsigned int index = m_GetFreeIndex();
	if (index < m_Elements.size())
	{
		m_Elements[index] = element;
	}
	else
	{
		m_Elements.push_back(element);
	}
	m_Length++;
	return index;
}

template<class T>
void packedVector<T>::RemoveAt(unsigned int index)
{
	m_Elements[index] = NULL;
	m_Gaps.push(index);
	m_Length--;
}

template<class T>
inline void packedVector<T>::Clear()
{
	m_Elements.clear();
}
