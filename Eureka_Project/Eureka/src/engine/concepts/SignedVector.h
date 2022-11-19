#pragma once
#include "pch.h"

template<class T>
struct signedVector
{
private:
	std::vector<T> m_PlusElements;
	std::vector<T> m_MinusElements;
	unsigned int m_Length;
public:
	signedVector();
	~signedVector();

	const std::vector<T> GetElements();
	const unsigned int Length();
	const bool InRange(int index);

	void SetAt(const T& element, int index);
	void RemoveAt(int index);
	void Clear();

	T& operator[](int i);
	//! Create function for checking if element exists
};

template<class T>
inline signedVector<T>::signedVector()
	: m_Length(0)
{
}

template<class T>
inline signedVector<T>::~signedVector()
{
	m_PlusElements.clear();
	m_MinusElements.clear();
}

template<class T>
inline const std::vector<T> signedVector<T>::GetElements()
{
	/*std::vector<int> plus = {1, 2, 3};
	std::vector<int> minus = {5, 2, -3};
	std::vector<int> test;
	test.reserve(m_Length);
	test.insert(test.end(), minus.begin(), minus.end());
	test.insert(test.end(), plus.begin(), plus.end());*/

	std::vector<T> elements;
	elements.reserve(m_Length);
	elements.insert(elements.end(), m_MinusElements.begin(), m_MinusElements.end());
	elements.insert(elements.end(), m_PlusElements.begin(), m_PlusElements.end());
	return elements;
}

template<class T>
inline const unsigned int signedVector<T>::Length()
{
	return m_Length;
}

template<class T>
inline const bool signedVector<T>::InRange(int index)
{
	return index < (int) m_PlusElements.size() && index > -(int) m_MinusElements.size() - 1;
}

template<class T>
inline void signedVector<T>::SetAt(const T& element, int index)
{
	if (index < 0)
	{
		int nIndex = -index - 1;
		if (nIndex >= (int)m_MinusElements.size())
		{
			m_MinusElements.resize(nIndex);
			m_MinusElements.insert(m_MinusElements.begin() + nIndex, element);
		}
		else
		{
			m_MinusElements[nIndex] = element;
		}
	}
	else
	{
		if (index >= (int)m_PlusElements.size())
		{
			m_PlusElements.resize(index);
			m_PlusElements.insert(m_PlusElements.begin() + index, element);
		}
		else
		{
			m_PlusElements[index] = element;
		}
	}
	m_Length = m_MinusElements.size() + m_PlusElements.size();
}

template<class T>
inline void signedVector<T>::RemoveAt(int index)
{
	if (index < 0)
		m_MinusElements.erase(-index - 1);
	else
		m_PlusElements.erase(index);
	m_Length = m_MinusElements.size() + m_PlusElements.size();
}

template<class T>
inline void signedVector<T>::Clear()
{
	m_PlusElements.clear();
	m_MinusElements.clear();
	m_Length = 0;
}

template<class T>
inline T& signedVector<T>::operator[](int i)
{
	if (i < 0)
		return m_MinusElements[-i - 1];
	else
		return m_PlusElements[i];
}
