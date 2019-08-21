#pragma once
#include "stdafx.h"
#include "MemoryManager.h"
#include <assert.h>

template<typename T>
class CSafeArray
{
public:
	CSafeArray(LONGLONG llSize):
	  szElems(NULL),
	  m_llSize(llSize),
	  m_pMemManager(new CMemoryManager())
	{
		szElems = m_pMemManager->Malloc<T>(llSize);
	}
	~CSafeArray()
	{
		m_pMemManager->Free((void**)&szElems);
		delete m_pMemManager;
	}

public:
	T& operator [] (int nIndex) //throw(std::out_of_range)
	{
		/*if(nIndex >= nSize)
		{
			throw std::out_of_range("Access array violated!\n");
		}
		else
		{
			return szElems[nIndex];
		}*/
		assert(nIndex < m_llSize);
		return szElems[nIndex];
	}

	T* GetBuff()
	{
		return szElems;
	}

	
	/*explicit */operator T*()  { //SmallInt -> int, œ‘ æ
		//std::cout << "Hello, girl, this is SmallInt to int. " << std::endl;
		return szElems;
	}

private:
	CMemoryManager* m_pMemManager;
	LONGLONG m_llSize;
	T* szElems;
};