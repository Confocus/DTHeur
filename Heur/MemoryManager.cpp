#include "MemoryManager.h"

CMemoryManager::CMemoryManager(void)
{
}

CMemoryManager::~CMemoryManager(void)
{
}

void CMemoryManager::Free(void** p)
{
	if(*p)
	{
		free(*p);
	}
	*p = NULL;
}

void CMemoryManager::Copy(void* pDst, int nDstSize, void* pSrc, int nSrcSize)
{
	if(!pDst || !pSrc)
	{
		return; 
	}

	memcpy_s(pDst, nDstSize, pSrc, nSrcSize);
}

void CMemoryManager::SetZero(void* p, int n)
{
	if(!p)
	{
		return;
	}
	memset(p, 0, n);
}
