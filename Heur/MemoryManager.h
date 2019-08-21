#pragma once
#include "stdafx.h"

//todo 设置成单实例得了
class CMemoryManager
{
public:
	CMemoryManager(void);
	~CMemoryManager(void);

public:
	template<typename T> 
	T* Malloc(LONGLONG llSize);
	

	void Free(void**);

	void Copy(void*, int, void*, int);

	void SetZero(void*, int);
};

template<typename T> 
T* CMemoryManager::Malloc(LONGLONG llSize)
{
	char* p = (char*)malloc(llSize * sizeof(T));
	if(!p)
	{
		return NULL;
	}
	memset(p, 0, llSize * sizeof(T));

	return (T*)p;
}